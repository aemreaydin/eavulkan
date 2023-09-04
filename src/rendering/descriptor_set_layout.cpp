#include <eavulkan/common/common.hpp>
#include <eavulkan/rendering/descriptor_set_layout.hpp>
#include <eavulkan/rendering/device.hpp>

namespace Rendering {

DescriptorSetLayout::DescriptorSetLayout( const Device& device, std::vector<VkDescriptorSetLayoutBinding> bindings )
  : DeviceResource( device ), _bindings( std::move( bindings ) ) {
  VkDescriptorSetLayoutCreateInfo layoutInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, nullptr, 0 };
  layoutInfo.bindingCount = toU32( _bindings.size() );
  layoutInfo.pBindings = _bindings.data();

  vkCheck( vkCreateDescriptorSetLayout( device.GetHandle(), &layoutInfo, nullptr, &GetHandle() ),
           "Failed to create descriptor set layout." );
}

auto DescriptorSetLayout::Cleanup() -> void {
  if ( GetHandle() != nullptr ) {
    vkDestroyDescriptorSetLayout( GetDevice().GetHandle(), GetHandle(), nullptr );
  }
}

}  // namespace Rendering
