#include <eavulkan/rendering/descriptor_pool.hpp>
#include <eavulkan/rendering/device.hpp>
#include <eavulkan/shared/utils.hpp>

namespace EA::Rendering {

DescriptorPool::DescriptorPool( const Device& device, std::vector<VkDescriptorPoolSize> pool_sizes, uint32_t max_sets )
  : DeviceResource( device ), _poolSizes( std::move( pool_sizes ) ), _maxSets( max_sets ) {
  VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{
    VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    nullptr,
    0,
  };
  descriptorPoolCreateInfo.maxSets = _maxSets;
  descriptorPoolCreateInfo.poolSizeCount = toU32( _poolSizes.size() );
  descriptorPoolCreateInfo.pPoolSizes = _poolSizes.data();

  vkCheck( vkCreateDescriptorPool( device.GetHandle(), &descriptorPoolCreateInfo, nullptr, &GetHandle() ),
           "Failed to create descriptor pool." );
}

void DescriptorPool::Cleanup() {
  if ( GetHandle() != nullptr ) {
    vkDestroyDescriptorPool( GetDevice().GetHandle(), GetHandle(), nullptr );
  }
}

}  // namespace EA::Rendering