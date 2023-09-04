#include <eavulkan/rendering/buffer.hpp>
#include <eavulkan/rendering/device.hpp>
#include <eavulkan/shared/mvp.hpp>
#include <eavulkan/shared/utils.hpp>

namespace EA::Rendering {

Buffer::Buffer( const Device& device, VkDeviceSize size, VkBufferUsageFlags flags,
                std::vector<uint32_t> queue_family_indices )
  : DeviceResource<VkBuffer>( device ),
    _deviceSize( size ),
    _flags( flags ),
    _queueFamilyIndices( std::move( queue_family_indices ) ) {
  VkBufferCreateInfo bufferCreateInfo{
    VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr, 0, _deviceSize, _flags, VK_SHARING_MODE_EXCLUSIVE, 0, nullptr
  };
  if ( _queueFamilyIndices.size() >= 2 ) {
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
    bufferCreateInfo.queueFamilyIndexCount = toU32( _queueFamilyIndices.size() );
    bufferCreateInfo.pQueueFamilyIndices = _queueFamilyIndices.data();
  }

  vkCheck( vkCreateBuffer( GetDevice().GetHandle(), &bufferCreateInfo, nullptr, &GetHandle() ),
           "Failed to create buffer" );
}

auto Buffer::AllocateBufferMemory( VkMemoryPropertyFlags memory_property_flags ) -> void {
  vkGetBufferMemoryRequirements( GetDevice().GetHandle(), GetHandle(), &_memoryRequirements );

  const VkMemoryAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, _memoryRequirements.size,
                                           GetDevice().FindMemoryPropertyIndex( _memoryRequirements.memoryTypeBits,
                                                                                memory_property_flags ) };
  vkCheck( vkAllocateMemory( GetDevice().GetHandle(), &allocateInfo, nullptr, &_deviceMemory ),
           "Failed to allocate buffer memory." );

  vkCheck( vkBindBufferMemory( GetDevice().GetHandle(), GetHandle(), _deviceMemory, 0 ),
           "Failed to bind buffer memory." );
}

auto Buffer::Map( void* data ) -> void {
  void* mappedMemory{ nullptr };
  vkCheck( vkMapMemory( GetDevice().GetHandle(), _deviceMemory, 0, _deviceSize, 0, &mappedMemory ),
           "Failed to map memory." );
  memcpy( mappedMemory, data, static_cast<size_t>( _deviceSize ) );
  vkUnmapMemory( GetDevice().GetHandle(), _deviceMemory );
}

auto Buffer::Map( const ModelViewProjection& mvp ) -> void {
  void* mappedMemory{ nullptr };
  vkCheck( vkMapMemory( GetDevice().GetHandle(), _deviceMemory, 0, _deviceSize, 0, &mappedMemory ),
           "Failed to map memory." );
  memcpy( mappedMemory, &mvp, static_cast<size_t>( _deviceSize ) );
  vkUnmapMemory( GetDevice().GetHandle(), _deviceMemory );
}

auto Buffer::Cleanup() -> void {
  vkDestroyBuffer( GetDevice().GetHandle(), GetHandle(), nullptr );
  vkFreeMemory( GetDevice().GetHandle(), _deviceMemory, nullptr );
}

}  // namespace EA::Rendering
