#pragma once

#include <eavulkan/common/includes.hpp>
#include <eavulkan/resource.hpp>

namespace Rendering {

class VCommandPool : public DeviceResource<VkCommandPool> {
 public:
  VCommandPool( const Device& device, VkQueueFlagBits queue_flags );
  auto Cleanup() -> void override;
};

}  // namespace Rendering
