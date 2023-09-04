#pragma once

#include <eavulkan/common/includes.hpp>

namespace Rendering {

class PhysicalDevice;
class Queue;
class Queues;
class Device {
  PhysicalDevice const* _pPhysicalDevice{ nullptr };
  VkDevice _device{ nullptr };
  std::unique_ptr<Queues> _upQueues{ nullptr };

  std::vector<VkDeviceQueueCreateInfo> _deviceQueueCreateInfos;
  std::vector<std::vector<float>> _queuePriorities;
  std::vector<VkExtensionProperties> _deviceExtensionProperties;

  std::vector<const char*> _enabledExtensionNames;

  auto initDevice() -> void;
  auto initDeviceExtensionProperties() -> void;
  auto enableDeviceExtension( const char* extension_name ) -> void;

 public:
  explicit Device( const PhysicalDevice& physical_device );
  ~Device();
  Device( const Device& ) = delete;
  Device( const Device&& ) = delete;
  auto operator=( const Device& ) -> Device& = delete;
  auto operator=( const Device&& ) -> Device& = delete;

  auto SubmitQueue( const VkSubmitInfo& submit_into, VkQueueFlagBits queue_flags, VkFence fence ) -> void;
  auto PresentQueue( const VkPresentInfoKHR& present_info, VkQueueFlagBits queue_flags ) -> void;

  [[nodiscard]] auto GetHandle() const -> VkDevice;
  [[nodiscard]] auto GetPhysicalDevice() const -> const PhysicalDevice&;
  [[nodiscard]] auto GetQueue( VkQueueFlagBits flags ) const -> const Queue&;
  [[nodiscard]] auto GetEnabledExtensions() const -> const std::vector<const char*>&;
  [[nodiscard]] auto FindMemoryPropertyIndex( uint32_t memory_type_bits_requirement,
                                              VkMemoryPropertyFlags required_memory_flags ) const -> uint32_t;
};

}  // namespace Rendering
