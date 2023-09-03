#pragma once

#include <eavulkan/common/includes.hpp>
#include <eavulkan/resource.hpp>

namespace Rendering {

class Device;
struct VMvp;
class VBuffer : public DeviceResource<VkBuffer> {
  VkDeviceSize _deviceSize{};
  VkDeviceMemory _deviceMemory{};
  VkBufferUsageFlags _flags{};
  std::vector<uint32_t> _queueFamilyIndices{};
  VkMemoryRequirements _memoryRequirements{};

 public:
  VBuffer( const Device& device, VkDeviceSize size, VkBufferUsageFlags flags,
           std::vector<uint32_t> queue_family_indices = {} );
  auto AllocateBufferMemory( VkMemoryPropertyFlags memory_property_flags ) -> void;
  auto Map( void* data ) -> void;
  auto Map( const VMvp& mvp ) -> void;
  auto Cleanup() -> void override;
};

}  // namespace Rendering
