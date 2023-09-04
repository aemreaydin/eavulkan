#include <eavulkan/rendering/physical_device.hpp>
#include <eavulkan/rendering/queue.hpp>
#include <eavulkan/shared/utils.hpp>

namespace EA::Rendering {

Queue::Queue( const PhysicalDevice& physical_device, uint32_t queue_family_index,
              VkQueueFamilyProperties queue_family_properties )
  : _pPhysicalDevice( &physical_device ),
    _queueFamilyIndex( queue_family_index ),
    _queueFamilyProperties( queue_family_properties ) {}

auto Queue::GetHandle() -> VkQueue& { return _queue; }

auto Queue::GetIndex() const -> uint32_t { return _queueFamilyIndex; }

auto Queue::GetCount() const -> uint32_t { return _queueFamilyProperties.queueCount; }

Queues::Queues( const PhysicalDevice& physical_device ) : _pPhysicalDevice( &physical_device ) { initQueues(); }

auto Queues::initQueues() -> void {
  _mapQueue.insert( { VK_QUEUE_GRAPHICS_BIT, findQueueFamilyIndex( VK_QUEUE_GRAPHICS_BIT ) } );
  _mapQueue.insert( { VK_QUEUE_COMPUTE_BIT, findQueueFamilyIndex( VK_QUEUE_COMPUTE_BIT ) } );
  _mapQueue.insert( { VK_QUEUE_TRANSFER_BIT, findQueueFamilyIndex( VK_QUEUE_TRANSFER_BIT ) } );
}

auto Queues::findQueueFamilyIndex( VkQueueFlagBits queue_flag ) const -> Queue {
  const auto& queueFamilyProperties = _pPhysicalDevice->GetPhysicalDeviceProperties().queueFamilyProperties;
  if ( ( queue_flag & VK_QUEUE_COMPUTE_BIT ) != 0 ) {
    for ( uint32_t idx = 0; idx != toU32( queueFamilyProperties.size() ); ++idx ) {
      const auto graphicsIt = _mapQueue.find( VK_QUEUE_GRAPHICS_BIT );
      const auto transferIt = _mapQueue.find( VK_QUEUE_TRANSFER_BIT );
      if ( ( graphicsIt != _mapQueue.end() && graphicsIt->second.GetIndex() == idx ) ||
           ( transferIt != _mapQueue.end() && transferIt->second.GetIndex() == idx ) ) {
        continue;
      }
      if ( ( queueFamilyProperties[idx].queueFlags & queue_flag ) != 0 ) {
        return Queue( *_pPhysicalDevice, idx, queueFamilyProperties[idx] );
      }
    }
  }

  // Try to find a dedicated transfer index
  if ( ( queue_flag & VK_QUEUE_TRANSFER_BIT ) != 0 ) {
    for ( uint32_t idx = 0; idx != toU32( queueFamilyProperties.size() ); ++idx ) {
      if ( _mapQueue.find( VK_QUEUE_GRAPHICS_BIT )->second.GetIndex() == idx ||
           _mapQueue.find( VK_QUEUE_COMPUTE_BIT )->second.GetIndex() == idx ) {
        continue;
      }
      if ( ( queueFamilyProperties[idx].queueFlags & queue_flag ) != 0 ) {
        return Queue( *_pPhysicalDevice, idx, queueFamilyProperties[idx] );
      }
    }
  }

  // Get the first index that matches the flagf
  for ( uint32_t idx = 0; idx != toU32( queueFamilyProperties.size() ); ++idx ) {
    if ( ( queueFamilyProperties[idx].queueFlags & queue_flag ) != 0 ) {
      return Queue( *_pPhysicalDevice, idx, queueFamilyProperties[idx] );
    }
  }

  throw std::runtime_error( "Failed to find a queue family index" );
}

auto Queues::GetQueue( VkQueueFlagBits queue_flag ) -> Queue& {
  const auto found = _mapQueue.find( queue_flag );
  if ( found == _mapQueue.end() ) {
    throw std::runtime_error( "Failed to find a queue with the given VkQueueFlagBits." );
  }
  return found->second;
}

auto Queues::GetQueue( VkQueueFlagBits queue_flag ) const -> const Queue& {
  const auto found = _mapQueue.find( queue_flag );
  if ( found == _mapQueue.end() ) {
    throw std::runtime_error( "Failed to find a queue with the given VkQueueFlagBits." );
  }
  return found->second;
}

auto Queues::GetQueues() -> QueueMap& { return _mapQueue; }

}  // namespace EA::Rendering
