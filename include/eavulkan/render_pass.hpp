#pragma once

#include <eavulkan/common/includes.hpp>
#include <eavulkan/resource.hpp>

namespace Rendering {

class RenderPass : public DeviceResource<VkRenderPass> {
  std::vector<VkAttachmentDescription> _attachmentDescriptions{};
  std::vector<VkSubpassDescription> _subpassDescriptions{};
  std::vector<VkSubpassDependency> _subpassDependencies{};

 public:
  RenderPass(const Device& device, std::vector<VkAttachmentDescription> attachment_descriptions,
             std::vector<VkSubpassDescription> subpass_descriptions,
             std::vector<VkSubpassDependency> subpass_dependencies);

  [[nodiscard]] auto GetAttachmentDescriptions() const -> const std::vector<VkAttachmentDescription>&;

  auto Cleanup() -> void override;
};

}  // namespace Rendering
