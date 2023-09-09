#pragma once

#include <eavulkan/rendering/resource.hpp>
#include <eavulkan/shared/includes.hpp>

namespace EA::Rendering {

class CommandPool : public DeviceResource<VkCommandPool> {
 public:
  CommandPool( const Device& device, VkQueueFlagBits queue_flags );
  auto Cleanup() -> void override;
};

}  // namespace EA::Rendering
