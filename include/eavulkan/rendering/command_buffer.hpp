#pragma once

#include <eavulkan/rendering/resource.hpp>
#include <eavulkan/shared/includes.hpp>

namespace EA::Rendering {

class CommandPool;

class CommandBuffer : public DeviceResource<VkCommandBuffer> {
  enum class State { INITIAL, RECORDING, EXECUTABLE, PENDING, INVALID };

  const CommandPool* _commandPool{ nullptr };
  bool _isPrimary{ false };
  State _state{ State::INVALID };

 public:
  explicit CommandBuffer( const CommandPool& command_pool, VkCommandBufferLevel level );

  auto SetState( State state ) -> void;

  [[nodiscard]] auto IsRecording() const -> bool;
  [[nodiscard]] auto IsPending() const -> bool;

  auto BeginCmd() -> VkResult;
  auto EndCmd() -> VkResult;
};

}  // namespace EA::Rendering
