#pragma once

#include <eavulkan/rendering/resource.hpp>
#include <eavulkan/shared/includes.hpp>

namespace EA::Rendering {

class VCommandPool : public DeviceResource<VkCommandPool> {
 public:
  VCommandPool( const Device& device, VkQueueFlagBits queue_flags );
  auto Cleanup() -> void override;
};

}  // namespace EA::Rendering
