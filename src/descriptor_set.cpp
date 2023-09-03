#include <eavulkan/common/common.hpp>
#include <eavulkan/descriptor_pool.hpp>
#include <eavulkan/descriptor_set.hpp>
#include <eavulkan/descriptor_set_layout.hpp>
#include <eavulkan/device.hpp>

namespace Rendering {

DescriptorSet::DescriptorSet(const DescriptorPool& descriptor_pool, const DescriptorSetLayout& layouts)
    : DeviceResource(descriptor_pool.GetDevice()) {
  VkDescriptorSetAllocateInfo allocateInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, nullptr};
  allocateInfo.descriptorPool = descriptor_pool.GetHandle();
  // allocateInfo.descriptorSetCount = toU32( layouts.size() ); // TODO(emreaydn) use multiple
  allocateInfo.descriptorSetCount = 1;
  allocateInfo.pSetLayouts = &layouts.GetHandle();

  vkCheck(vkAllocateDescriptorSets(GetDevice().GetHandle(), &allocateInfo, &GetHandle()),
          "Failed to allocate descriptor sets.");
}

DescriptorSets::DescriptorSets(const DescriptorPool& descriptor_pool, const std::vector<VkDescriptorSetLayout>& layouts)
    : DeviceResource(descriptor_pool.GetDevice()) {
  VkDescriptorSetAllocateInfo allocateInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, nullptr};
  allocateInfo.descriptorPool = descriptor_pool.GetHandle();
  allocateInfo.descriptorSetCount = toU32(layouts.size());  // TODO(emreaydn) use multiple
  allocateInfo.pSetLayouts = layouts.data();

  GetHandle().resize(layouts.size());
  vkCheck(vkAllocateDescriptorSets(GetDevice().GetHandle(), &allocateInfo, GetHandle().data()),
          "Failed to allocate descriptor sets.");
}

}  // namespace Rendering
