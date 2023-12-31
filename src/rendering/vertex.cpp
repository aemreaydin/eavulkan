#include <eavulkan/rendering/vertex.hpp>

namespace EA::Rendering {

Vertex::Vertex( glm::vec3 position ) : position( position ) {}

Vertex::Vertex( glm::vec3 position, glm::vec3 normal, glm::vec2 tex_coord )
  : position( position ), normal( normal ), texCoord( tex_coord ) {}

auto Vertex::GetInputBindingDescription() -> std::vector<VkVertexInputBindingDescription> {
  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = 0;
  bindingDescription.stride = sizeof( Vertex );
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return { bindingDescription };
}

auto Vertex::GetAttributeBindingDescriptions() -> std::vector<VkVertexInputAttributeDescription> {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions( 3 );
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof( Vertex, position );

  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof( Vertex, normal );

  attributeDescriptions[2].binding = 0;
  attributeDescriptions[2].location = 2;
  attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[2].offset = offsetof( Vertex, texCoord );

  return attributeDescriptions;
}

}  // namespace EA::Rendering
