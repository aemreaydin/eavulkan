#include <eavulkan/common/common.hpp>
#include <eavulkan/device.hpp>
#include <eavulkan/frame_buffer.hpp>
#include <eavulkan/render_pass.hpp>

namespace Rendering {

Framebuffer::Framebuffer( const Device& device, const RenderPass& render_pass,
                          const std::vector<VkImageView>& attachments, VkExtent2D extent )
  : DeviceResource<VkFramebuffer>( device ), _pRenderPass( &render_pass ) {
  Expects( _pRenderPass->GetAttachmentDescriptions().size() == attachments.size() );

  const VkFramebufferCreateInfo framebufferCreateInfo{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                                                       nullptr,
                                                       0,
                                                       _pRenderPass->GetHandle(),
                                                       toU32( _pRenderPass->GetAttachmentDescriptions().size() ),
                                                       attachments.data(),
                                                       extent.width,
                                                       extent.height,
                                                       1 };

  vkCheck( vkCreateFramebuffer( GetDevice().GetHandle(), &framebufferCreateInfo, nullptr, &GetHandle() ),
           "Failed to create framebuffer." );
}

auto Framebuffer::Cleanup() -> void { vkDestroyFramebuffer( GetDevice().GetHandle(), GetHandle(), nullptr ); }

}  // namespace Rendering