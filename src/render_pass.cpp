#include <eavulkan/common/common.hpp>
#include <eavulkan/device.hpp>
#include <eavulkan/render_pass.hpp>

namespace Rendering {

RenderPass::RenderPass(const Device& device, std::vector<VkAttachmentDescription> attachment_descriptions,
                       std::vector<VkSubpassDescription> subpass_descriptions,
                       std::vector<VkSubpassDependency> subpass_dependencies)
    : DeviceResource<VkRenderPass>(device),
      _attachmentDescriptions(std::move(attachment_descriptions)),
      _subpassDescriptions(std::move(subpass_descriptions)),
      _subpassDependencies(std::move(subpass_dependencies)) {
  const VkRenderPassCreateInfo renderPassCreateInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                                                    nullptr,
                                                    0,
                                                    toU32(_attachmentDescriptions.size()),
                                                    _attachmentDescriptions.data(),
                                                    toU32(_subpassDescriptions.size()),
                                                    _subpassDescriptions.data(),
                                                    toU32(_subpassDependencies.size()),
                                                    _subpassDependencies.data()};

  vkCheck(vkCreateRenderPass(GetDevice().GetHandle(), &renderPassCreateInfo, nullptr, &GetHandle()),
          "Failed to create render pass.");
}

auto RenderPass::GetAttachmentDescriptions() const -> const std::vector<VkAttachmentDescription>& {
  return _attachmentDescriptions;
}

void RenderPass::Cleanup() { vkDestroyRenderPass(GetDevice().GetHandle(), GetHandle(), nullptr); }

}  // namespace Rendering