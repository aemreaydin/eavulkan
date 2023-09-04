#include <eavulkan/rendering/buffer.hpp>
#include <eavulkan/rendering/mesh.hpp>
#include <eavulkan/rendering/texture.hpp>
#include <eavulkan/rendering/vertex.hpp>
#include <eavulkan/shared/utils.hpp>

namespace EA::Rendering {

Mesh::MeshInfo::MeshInfo( std::string name, std::string location )
  : name( std::move( name ) ), location( std::move( location ) ) {}

Mesh::Mesh( MeshInfo mesh_info, std::vector<Vertex> vertices, std::vector<uint32_t> indices, const Texture2D& texture )
  : meshInfo( std::move( mesh_info ) ),
    vertices( std::move( vertices ) ),
    indices( std::move( indices ) ),
    texture( std::make_shared<Texture2D>( texture ) ) {}

auto Mesh::FromFile( const std::string& name, const std::string& file_name ) -> Mesh {
  const std::filesystem::path filePath( file_name );
  const auto location = filePath.parent_path();

  tinyobj::ObjReader reader;
  if ( !reader.ParseFromFile( file_name ) ) {
    if ( !reader.Error().empty() ) {
      spdlog::error( "TinyObjReader: {}", reader.Error() );
    }
    throw std::runtime_error( "Failed to load mesh from file." );
  }
  if ( !reader.Warning().empty() ) {
    spdlog::warn( "TinyObjReader: {}", reader.Warning() );
  }

  // TODO(emreaydn): Handle multiple shapes
  const auto& attrib = reader.GetAttrib();
  const auto& shapes = reader.GetShapes();
  const auto& materials = reader.GetMaterials();

  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  for ( const auto& shape : shapes ) {
    size_t indexOffset = 0;
    for ( size_t faceInd = 0; faceInd != shape.mesh.num_face_vertices.size(); ++faceInd ) {
      const auto faceVertCount = shape.mesh.num_face_vertices[faceInd];
      for ( size_t faceVertInd = 0; faceVertInd != faceVertCount; ++faceVertInd ) {
        const tinyobj::index_t idx = shape.mesh.indices[indexOffset + faceVertInd];
        Vertex vertex{};
        vertex.position = { attrib.vertices[3 * idx.vertex_index + 0], attrib.vertices[3 * idx.vertex_index + 1],
                            attrib.vertices[3 * idx.vertex_index + 2] };
        if ( idx.normal_index >= 0 ) {
          vertex.normal = { attrib.normals[3 * idx.normal_index + 0], attrib.normals[3 * idx.normal_index + 1],
                            attrib.normals[3 * idx.normal_index + 2] };
        }
        if ( idx.texcoord_index >= 0 ) {
          vertex.texCoord = { attrib.texcoords[2 * idx.texcoord_index + 0],
                              1.F - attrib.texcoords[2 * idx.texcoord_index + 1] };
        }
        vertices.push_back( vertex );
        indices.push_back( toU32( indices.size() ) );
      }
      indexOffset += faceVertCount;
    }
  }

  const auto meshInfo = MeshInfo{ name, location };
  const auto texture = Texture2D::FromFile( location / materials[0].diffuse_texname );
  return { meshInfo, vertices, indices, texture };
}

auto Mesh::CreateBuffers( const Device& device ) -> void {
  vertexBuffer =
    std::make_unique<Buffer>( device, sizeof( Vertex ) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
  vertexBuffer->AllocateBufferMemory( VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
  vertexBuffer->Map( vertices.data() );

  indexBuffer =
    std::make_unique<Buffer>( device, sizeof( uint32_t ) * indices.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
  indexBuffer->AllocateBufferMemory( VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
  indexBuffer->Map( indices.data() );
}

auto Mesh::BindBuffers( VkCommandBuffer cmd ) const -> void {
  static std::array<VkDeviceSize, 1> offsets{ 0 };
  vkCmdBindVertexBuffers( cmd, 0, 1, &vertexBuffer->GetHandle(), offsets.data() );
  vkCmdBindIndexBuffer( cmd, indexBuffer->GetHandle(), 0, VK_INDEX_TYPE_UINT32 );
}

auto Mesh::Render( VkCommandBuffer cmd ) const -> void {
  BindBuffers( cmd );
  vkCmdDrawIndexed( cmd, toU32( indices.size() ), 1, 0, 0, 0 );
}

auto Mesh::Cleanup() const -> void {
  vertexBuffer->Cleanup();
  indexBuffer->Cleanup();
}

}  // namespace EA::Rendering