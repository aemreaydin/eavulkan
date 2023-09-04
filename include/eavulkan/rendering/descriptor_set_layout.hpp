#pragma once

#include <eavulkan/common/includes.hpp>
#include <eavulkan/rendering/resource.hpp>

namespace Rendering {

class DescriptorPool;
class DescriptorSetLayout : public DeviceResource<VkDescriptorSetLayout> {
  std::vector<VkDescriptorSetLayoutBinding> _bindings;

 public:
  DescriptorSetLayout( const Device& device, std::vector<VkDescriptorSetLayoutBinding> bindings );
  auto Cleanup() -> void override;
};

}  // namespace Rendering
