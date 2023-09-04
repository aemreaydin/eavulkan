#pragma once

#include <eavulkan/rendering/resource.hpp>
#include <eavulkan/shared/includes.hpp>

namespace EA::Rendering {

class DescriptorPool;

class DescriptorSetLayout : public DeviceResource<VkDescriptorSetLayout> {
  std::vector<VkDescriptorSetLayoutBinding> _bindings;

 public:
  DescriptorSetLayout( const Device& device, std::vector<VkDescriptorSetLayoutBinding> bindings );
  auto Cleanup() -> void override;
};

}  // namespace EA::Rendering
