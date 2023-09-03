#pragma once

#include <eavulkan/common/includes.hpp>

namespace Rendering {

enum class QueueType { GRAPHICS, COMPUTE, TRANSFER };
class PhysicalDevice;
class Queue {
  const PhysicalDevice* _pPhysicalDevice{nullptr};
  VkQueue _queue{nullptr};
  uint32_t _queueFamilyIndex{0};
  VkQueueFamilyProperties _queueFamilyProperties{};

  explicit Queue(const PhysicalDevice& physical_device, uint32_t queue_family_index,
                 VkQueueFamilyProperties queue_family_properties);

 public:
  [[nodiscard]] auto GetHandle() -> VkQueue&;
  [[nodiscard]] auto GetIndex() const -> uint32_t;
  [[nodiscard]] auto GetCount() const -> uint32_t;

  friend class Queues;
};

using QueueMap = std::map<VkQueueFlagBits, Queue>;
class Queues {
  PhysicalDevice const* _pPhysicalDevice{nullptr};
  std::map<VkQueueFlagBits, Queue> _mapQueue;

  auto initQueues() -> void;
  [[nodiscard]] auto findQueueFamilyIndex(VkQueueFlagBits queue_flag) const -> Queue;

 public:
  explicit Queues(const PhysicalDevice& physical_device);
  [[nodiscard]] auto GetQueue(VkQueueFlagBits queue_flag) -> Queue&;
  [[nodiscard]] auto GetQueue(VkQueueFlagBits queue_flag) const -> const Queue&;
  [[nodiscard]] auto GetQueues() -> QueueMap&;
};

}  // namespace Rendering
