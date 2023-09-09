#include <eavulkan/rendering/command_pool.hpp>
#include <eavulkan/rendering/device.hpp>
#include <eavulkan/rendering/queue.hpp>
#include <eavulkan/shared/utils.hpp>

namespace EA::Rendering {

CommandPool::CommandPool( const Device& device, VkQueueFlagBits queue_flags ) : DeviceResource( device ) {
  const VkCommandPoolCreateInfo commandPoolCreateInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, nullptr,
                                                       VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                                                       device.GetQueue( queue_flags ).GetIndex() };

  vkCheck( vkCreateCommandPool( device.GetHandle(), &commandPoolCreateInfo, nullptr, &GetHandle() ),
           "Failed to create command pool." );
}

auto CommandPool::Cleanup() -> void { vkDestroyCommandPool( GetDevice().GetHandle(), GetHandle(), nullptr ); }

}  // namespace EA::Rendering
