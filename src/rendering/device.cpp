#include <eavulkan/common/common.hpp>
#include <eavulkan/rendering/device.hpp>
#include <eavulkan/rendering/physical_device.hpp>
#include <eavulkan/rendering/queue.hpp>

namespace Rendering {

Device::Device( const PhysicalDevice& physical_device )
  : _pPhysicalDevice( &physical_device ), _upQueues( std::make_unique<Queues>( physical_device ) ) {
  initDeviceExtensionProperties();

#if __APPLE__
  enableDeviceExtension( "VK_KHR_portability_subset" );
#endif
  enableDeviceExtension( "VK_KHR_swapchain" );
  enableDeviceExtension( "VK_KHR_maintenance1" );
  enableDeviceExtension( "VK_EXT_pipeline_creation_feedback" );

  initDevice();
}

Device::~Device() { vkDestroyDevice( _device, nullptr ); }

auto Device::SubmitQueue( const VkSubmitInfo& submit_into, VkQueueFlagBits queue_flags, VkFence fence ) -> void {
  auto& queue = _upQueues->GetQueue( queue_flags );
  vkCheck( vkQueueSubmit( queue.GetHandle(), 1, &submit_into, fence ), "Failed to submit queue." );
}

auto Device::PresentQueue( const VkPresentInfoKHR& present_info, VkQueueFlagBits queue_flags ) -> void {
  auto& queue = _upQueues->GetQueue( queue_flags );
  vkCheck( vkQueuePresentKHR( queue.GetHandle(), &present_info ), "Failed to present queue." );
}

auto Device::initDevice() -> void {
  auto& mapQueues = _upQueues->GetQueues();
  _deviceQueueCreateInfos.resize( mapQueues.size() );
  _queuePriorities.resize( mapQueues.size() );

  for ( uint32_t idx = 0; auto [queueFlag, queue] : mapQueues ) {
    _queuePriorities[idx].push_back( 1.0 );
    const VkDeviceQueueCreateInfo deviceQueueCreateInfo{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                                         nullptr,
                                                         0,
                                                         queue.GetIndex(),
                                                         queue.GetCount(),
                                                         _queuePriorities[idx].data() };
    _deviceQueueCreateInfos[idx++] = deviceQueueCreateInfo;
  }
  VkDeviceCreateInfo deviceCreateInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, nullptr, 0 };
  deviceCreateInfo.queueCreateInfoCount = toU32( _deviceQueueCreateInfos.size() );
  deviceCreateInfo.pQueueCreateInfos = _deviceQueueCreateInfos.data();
  deviceCreateInfo.enabledExtensionCount = toU32( _enabledExtensionNames.size() );
  deviceCreateInfo.ppEnabledExtensionNames = _enabledExtensionNames.data();
  deviceCreateInfo.pEnabledFeatures = &_pPhysicalDevice->GetPhysicalDeviceProperties().physicalDeviceFeatures;

  vkCheck( vkCreateDevice( _pPhysicalDevice->GetHandle(), &deviceCreateInfo, nullptr, &_device ),
           "Failed to create device." );

  for ( auto& [_, queue] : mapQueues ) {
    vkGetDeviceQueue( GetHandle(), queue.GetIndex(), 0, &queue.GetHandle() );
  }
}

auto Device::initDeviceExtensionProperties() -> void {
  uint32_t extensionCount{};
  vkEnumerateDeviceExtensionProperties( _pPhysicalDevice->GetHandle(), nullptr, &extensionCount, nullptr );
  _deviceExtensionProperties.resize( extensionCount );
  vkEnumerateDeviceExtensionProperties( _pPhysicalDevice->GetHandle(), nullptr, &extensionCount,
                                        _deviceExtensionProperties.data() );
}

auto Device::enableDeviceExtension( const char* extension_name ) -> void {
  for ( const auto& extensionProperties : _deviceExtensionProperties ) {
    if ( strcmp( extension_name, std::span( extensionProperties.extensionName ).data() ) == 0 ) {
      _enabledExtensionNames.push_back( extension_name );
      return;
    }
  }
  spdlog::info( "Failed to enable extension {}.", extension_name );
}

auto Device::GetHandle() const -> VkDevice { return _device; }

auto Device::GetPhysicalDevice() const -> const PhysicalDevice& { return *_pPhysicalDevice; }

auto Device::GetQueue( VkQueueFlagBits flags ) const -> const Queue& { return _upQueues->GetQueue( flags ); }

auto Device::GetEnabledExtensions() const -> const std::vector<const char*>& { return _enabledExtensionNames; }

auto Device::FindMemoryPropertyIndex( uint32_t memory_type_bits_requirement,
                                      VkMemoryPropertyFlags required_memory_flags ) const -> uint32_t {
  const auto& memoryProperties = _pPhysicalDevice->GetPhysicalDeviceProperties().memoryProperties;
  for ( uint32_t idx = 0; idx < memoryProperties.memoryTypeCount; ++idx ) {
    const bool isRequiredMemoryType = ( memory_type_bits_requirement & ( 1 << idx ) ) != 0U;
    const auto propertyFlags = gsl::at( memoryProperties.memoryTypes, idx ).propertyFlags;
    const bool hasRequiredProperties = ( propertyFlags & required_memory_flags ) == required_memory_flags;

    if ( isRequiredMemoryType && hasRequiredProperties ) {
      return idx;
    }
  }
  throw std::runtime_error( "Failed to find required memory property index." );
}

}  // namespace Rendering
