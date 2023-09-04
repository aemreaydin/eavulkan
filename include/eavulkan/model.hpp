#pragma once

#include <eavulkan/shared/includes.hpp>
#include <eavulkan/shared/transform.hpp>

namespace EA {

namespace Rendering {
struct Mesh;
}

struct Model {
  std::shared_ptr<Rendering::Mesh> mesh;
  Transform transform;

  explicit Model( std::shared_ptr<Rendering::Mesh> mesh, Transform transform );

  auto Render( VkCommandBuffer cmd ) const -> void;
};

}  // namespace EA