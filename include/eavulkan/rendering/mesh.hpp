#pragma once

#include <eavulkan/rendering/device.hpp>
#include <eavulkan/shared/includes.hpp>

namespace EA::Rendering {

struct Vertex;
struct Texture2D;
class Buffer;

struct Mesh {
  struct MeshInfo {
    std::string name{};
    std::string location{};

    MeshInfo( std::string name, std::string location );
  };

  MeshInfo meshInfo;

  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  std::shared_ptr<Texture2D> texture{ nullptr };

  std::unique_ptr<Buffer> vertexBuffer{ nullptr };
  std::unique_ptr<Buffer> indexBuffer{ nullptr };

  Mesh( MeshInfo mesh_info, std::vector<Vertex> vertices, std::vector<uint32_t> indices, const Texture2D& texture );

  static auto FromFile( const std::string& name, const std::string& file_name ) -> Mesh;

  auto CreateBuffers( const Device& device ) -> void;

  auto BindBuffers( VkCommandBuffer cmd ) const -> void;

  auto Render( VkCommandBuffer cmd ) const -> void;

  auto Cleanup() const -> void;
};

}  // namespace EA::Rendering