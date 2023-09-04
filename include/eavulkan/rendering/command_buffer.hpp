#pragma once

#include <eavulkan/common/includes.hpp>
#include <eavulkan/rendering/resource.hpp>

namespace Rendering {

class VCommandPool;
class CommandBuffer : public DeviceResource<VkCommandBuffer> {
  enum class State { INITIAL, RECORDING, EXECUTABLE, PENDING, INVALID };

  const VCommandPool* _commandPool{ nullptr };
  bool _isPrimary{ false };
  State _state{ State::INVALID };

 public:
  explicit CommandBuffer( const VCommandPool& command_pool, VkCommandBufferLevel level );

  auto SetState( State state ) -> void;

  [[nodiscard]] auto IsRecording() const -> bool;
  [[nodiscard]] auto IsPending() const -> bool;

  auto BeginCmd() -> VkResult;
  auto EndCmd() -> VkResult;
};

}  // namespace Rendering
