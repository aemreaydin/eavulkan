#include <eavulkan/command_pool.hpp>
#include <eavulkan/common/common.hpp>
#include <eavulkan/device.hpp>
#include <eavulkan/queue.hpp>

namespace Rendering {

VCommandPool::VCommandPool(const Device& device, VkQueueFlagBits queue_flags) : DeviceResource(device) {
  const VkCommandPoolCreateInfo commandPoolCreateInfo{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, nullptr,
                                                      VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                                                      device.GetQueue(queue_flags).GetIndex()};

  vkCheck(vkCreateCommandPool(device.GetHandle(), &commandPoolCreateInfo, nullptr, &GetHandle()),
          "Failed to create command pool.");
}

auto VCommandPool::Cleanup() -> void { vkDestroyCommandPool(GetDevice().GetHandle(), GetHandle(), nullptr); }

}  // namespace Rendering