#include <eavulkan/model.hpp>
#include <eavulkan/rendering/mesh.hpp>

namespace EA {

EA::Model::Model( std::shared_ptr<Rendering::Mesh> mesh, Transform transform )
  : mesh( std::move( mesh ) ), transform( transform ) {}

auto Model::Render( VkCommandBuffer cmd ) const -> void { mesh->Render( cmd ); }

}  // namespace EA