#include <eavulkan/rendering/command_buffer.hpp>
#include <eavulkan/rendering/command_pool.hpp>
#include <eavulkan/rendering/device.hpp>
#include <eavulkan/shared/utils.hpp>

namespace EA::Rendering {
CommandBuffer::CommandBuffer( const CommandPool& command_pool, VkCommandBufferLevel level )
  : DeviceResource<VkCommandBuffer>( command_pool.GetDevice() ),
    _commandPool( &command_pool ),
    _isPrimary( level == VK_COMMAND_BUFFER_LEVEL_PRIMARY ),
    _state( State::INITIAL ) {
  const VkCommandBufferAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, nullptr,
                                                  _commandPool->GetHandle(), level, 1 };

  vkCheck( vkAllocateCommandBuffers( GetDevice().GetHandle(), &allocateInfo, &GetHandle() ),
           "Failed to allocate command buffers" );
}

auto CommandBuffer::SetState( CommandBuffer::State state ) -> void { _state = state; }

auto CommandBuffer::IsRecording() const -> bool { return _state == State::RECORDING; }

auto CommandBuffer::IsPending() const -> bool { return _state == State::PENDING; }

auto CommandBuffer::BeginCmd() -> VkResult {
  if ( IsPending() ) {
    spdlog::error( "Command buffer is in pending state." );
    return VK_NOT_READY;
  }
  if ( IsRecording() ) {
    spdlog::error( "Command buffer is already recording." );
    return VK_NOT_READY;
  }

  SetState( State::RECORDING );

  const VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, nullptr,
                                            VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr };

  return vkBeginCommandBuffer( GetHandle(), &beginInfo );
}

auto CommandBuffer::EndCmd() -> VkResult {
  if ( !IsRecording() ) {
    spdlog::error( "Command Buffer should be in Recording state" );
    return VK_NOT_READY;
  }

  SetState( State::EXECUTABLE );

  return vkEndCommandBuffer( GetHandle() );
}

}  // namespace EA::Rendering
