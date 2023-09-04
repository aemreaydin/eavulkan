#pragma once

#include <eavulkan/common/includes.hpp>

namespace Rendering {

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};

  // Vertex() = default;
  explicit Vertex( glm::vec3 position );

  Vertex( glm::vec3 position, glm::vec3 normal, glm::vec2 tex_coord );

  static auto GetInputBindingDescription() -> std::vector<VkVertexInputBindingDescription>;

  static auto GetAttributeBindingDescriptions() -> std::vector<VkVertexInputAttributeDescription>;
};

}  // namespace Rendering
