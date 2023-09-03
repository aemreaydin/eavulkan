#pragma once

#include <eavulkan/common/includes.hpp>
#include <eavulkan/resource.hpp>

namespace Rendering {

class DescriptorPool;
class DescriptorSetLayout;
class DescriptorSet : public DeviceResource<VkDescriptorSet> {
  const DescriptorPool* _descriptorPool{ nullptr };

 public:
  DescriptorSet( const DescriptorPool& descriptor_pool, const DescriptorSetLayout& layouts );
};

class DescriptorSets : public DeviceResource<std::vector<VkDescriptorSet>> {
  const DescriptorPool* _descriptorPool{ nullptr };

 public:
  DescriptorSets( const DescriptorPool& descriptor_pool, const std::vector<VkDescriptorSetLayout>& layouts );
};

}  // namespace Rendering
