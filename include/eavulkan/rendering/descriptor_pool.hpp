#pragma once

#include <eavulkan/rendering/resource.hpp>
#include <eavulkan/shared/includes.hpp>

namespace EA::Rendering {

class DescriptorPool : public DeviceResource<VkDescriptorPool> {
  static constexpr uint32_t kMaxDescriptorSets = 1000;

  std::vector<VkDescriptorPoolSize> _poolSizes;
  uint32_t _maxSets;

 public:
  DescriptorPool( const Device& device, std::vector<VkDescriptorPoolSize> pool_sizes,
                  uint32_t max_sets = kMaxDescriptorSets );
  auto Cleanup() -> void override;
};

}  // namespace EA::Rendering
