#pragma once

#include <eavulkan/rendering/resource.hpp>
#include <eavulkan/shared/includes.hpp>

namespace EA::Rendering {

class Device;
struct ModelViewProjection;

class Buffer : public DeviceResource<VkBuffer> {
  VkDeviceSize _deviceSize{};
  VkDeviceMemory _deviceMemory{};
  VkBufferUsageFlags _flags{};
  std::vector<uint32_t> _queueFamilyIndices{};
  VkMemoryRequirements _memoryRequirements{};

 public:
  Buffer( const Device& device, VkDeviceSize size, VkBufferUsageFlags flags,
          std::vector<uint32_t> queue_family_indices = {} );
  auto AllocateBufferMemory( VkMemoryPropertyFlags memory_property_flags ) -> void;
  auto Map( void* data ) -> void;
  auto Map( const ModelViewProjection& mvp ) -> void;
  auto Cleanup() -> void override;
};

}  // namespace EA::Rendering
