#pragma once

#include <eavulkan/rendering/resource.hpp>
#include <eavulkan/shared/includes.hpp>

namespace EA::Rendering {

class RenderPass;
class Swapchain;

class Framebuffer : public DeviceResource<VkFramebuffer> {
  const RenderPass* _pRenderPass{ nullptr };

 public:
  Framebuffer( const Device& device, const RenderPass& renderPass, const std::vector<VkImageView>& attachments,
               VkExtent2D extent );

  auto Cleanup() -> void override;
};

}  // namespace EA::Rendering
