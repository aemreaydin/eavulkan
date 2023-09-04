#include <eavulkan/rendering/descriptor_pool.hpp>
#include <eavulkan/rendering/descriptor_set.hpp>
#include <eavulkan/rendering/descriptor_set_layout.hpp>
#include <eavulkan/rendering/device.hpp>
#include <eavulkan/shared/utils.hpp>

namespace EA::Rendering {

DescriptorSet::DescriptorSet( const DescriptorPool& descriptor_pool, const DescriptorSetLayout& layouts )
  : DeviceResource( descriptor_pool.GetDevice() ) {
  VkDescriptorSetAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, nullptr };
  allocateInfo.descriptorPool = descriptor_pool.GetHandle();
  // allocateInfo.descriptorSetCount = toU32( layouts.size() ); // TODO(emreaydn) use multiple
  allocateInfo.descriptorSetCount = 1;
  allocateInfo.pSetLayouts = &layouts.GetHandle();

  vkCheck( vkAllocateDescriptorSets( GetDevice().GetHandle(), &allocateInfo, &GetHandle() ),
           "Failed to allocate descriptor sets." );
}

DescriptorSets::DescriptorSets( const DescriptorPool& descriptor_pool,
                                const std::vector<VkDescriptorSetLayout>& layouts )
  : DeviceResource( descriptor_pool.GetDevice() ) {
  VkDescriptorSetAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, nullptr };
  allocateInfo.descriptorPool = descriptor_pool.GetHandle();
  allocateInfo.descriptorSetCount = toU32( layouts.size() );  // TODO(emreaydn) use multiple
  allocateInfo.pSetLayouts = layouts.data();

  GetHandle().resize( layouts.size() );
  vkCheck( vkAllocateDescriptorSets( GetDevice().GetHandle(), &allocateInfo, GetHandle().data() ),
           "Failed to allocate descriptor sets." );
}

}  // namespace EA::Rendering
