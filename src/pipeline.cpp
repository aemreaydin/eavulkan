#include <eavulkan/common/common.hpp>
#include <eavulkan/device.hpp>
#include <eavulkan/pipeline.hpp>

namespace Rendering {

Pipeline::Pipeline(const Device& device, const PipelineState& state) : DeviceResource<VkPipeline>(device) {
  vkCheck(vkCreatePipelineLayout(GetDevice().GetHandle(), &state.pipelineLayoutCreateInfo, nullptr, &_pipelineLayout),
          "Failed to create pipeline layout.");
}

auto Pipeline::getLayout() -> VkPipelineLayout& { return _pipelineLayout; }

auto Pipeline::Cleanup() -> void {
  vkDestroyPipelineLayout(GetDevice().GetHandle(), _pipelineLayout, nullptr);
  vkDestroyPipeline(GetDevice().GetHandle(), GetHandle(), nullptr);
}

PipelineStateBuilder::PipelineStateBuilder(const Device& device) : _device(&device) {
  // TODO(emreaydn) Use these in the future
  MultisampleState().DepthStencilState();
}

auto PipelineStateBuilder::GetState() const -> const PipelineState& { return _state; }

auto PipelineStateBuilder::VertexInputState(std::vector<VkVertexInputBindingDescription> bindings,
                                            std::vector<VkVertexInputAttributeDescription> attributes)
    -> PipelineStateBuilder& {
  _state.vertexInputBindingDescriptions = std::move(bindings);
  _state.vertexInputAttributeDescriptions = std::move(attributes);
  _state.vertexInputStateCreateInfo = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                                       nullptr,
                                       0,
                                       toU32(_state.vertexInputBindingDescriptions.size()),
                                       _state.vertexInputBindingDescriptions.data(),
                                       toU32(_state.vertexInputAttributeDescriptions.size()),
                                       _state.vertexInputAttributeDescriptions.data()};
  return *this;
}

auto PipelineStateBuilder::InputAssemblyState(VkPrimitiveTopology topology, VkBool32 primitive_restart_enable)
    -> PipelineStateBuilder& {
  _state.inputAssemblyStateCreateInfo = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, nullptr, 0,
                                         topology, primitive_restart_enable};
  return *this;
}

auto PipelineStateBuilder::ShaderStage(VkShaderStageFlagBits shader_stage, const std::vector<uint32_t>& shader_code)
    -> PipelineStateBuilder& {
  VkShaderModule shaderModule{nullptr};
  const VkShaderModuleCreateInfo shaderModuleCreateInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, nullptr, 0,
                                                        shader_code.size(), shader_code.data()};
  vkCheck(vkCreateShaderModule(_device->GetHandle(), &shaderModuleCreateInfo, nullptr, &shaderModule),
          "Failed to create shader module.");
  _state.shaderModules.push_back(shaderModule);
  const VkPipelineShaderStageCreateInfo shaderStageCreateInfo{
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, shader_stage, shaderModule, "main", nullptr};
  _state.shaderStageCreateInfos.push_back(shaderStageCreateInfo);

  return *this;
}

auto PipelineStateBuilder::ViewportState(const std::vector<VkViewport>& viewports,
                                         const std::vector<VkRect2D>& scissors) -> PipelineStateBuilder& {
  _state.viewportStateCreateInfo = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                                    nullptr,
                                    0,
                                    toU32(viewports.size()),
                                    viewports.data(),
                                    toU32(scissors.size()),
                                    scissors.data()};

  return *this;
}

auto PipelineStateBuilder::RasterizationState(VkBool32 depth_clamp, VkPolygonMode polygon_mode,
                                              VkCullModeFlags cull_mode, VkFrontFace front_face)
    -> PipelineStateBuilder& {
  _state.rasterizationStateCreateInfo = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                                         nullptr,
                                         0,
                                         depth_clamp,
                                         VK_FALSE,
                                         polygon_mode,
                                         cull_mode,
                                         front_face,
                                         VK_FALSE,
                                         0.0,
                                         0.0,
                                         0.0,
                                         1.0};

  return *this;
}

auto PipelineStateBuilder::MultisampleState() -> PipelineStateBuilder& {
  _state.multisampleStateCreateInfo = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                                       nullptr,
                                       0,
                                       VK_SAMPLE_COUNT_1_BIT,
                                       VK_FALSE,
                                       0.0,
                                       nullptr,
                                       VK_FALSE,
                                       VK_FALSE};
  return *this;
}

auto PipelineStateBuilder::DepthStencilState() -> PipelineStateBuilder& {
  _state.depthStencilStateCreateInfo = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                                        nullptr,
                                        0,
                                        VK_FALSE,
                                        VK_FALSE,
                                        VK_COMPARE_OP_ALWAYS,
                                        VK_FALSE,
                                        VK_FALSE,
                                        {},
                                        {},
                                        0.0,
                                        0.0};
  return *this;
}

auto PipelineStateBuilder::ColorBlendState(
    const std::vector<VkPipelineColorBlendAttachmentState>& color_blend_attachments) -> PipelineStateBuilder& {
  _state.colorBlendStateCreateInfo = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                                      nullptr,
                                      0,
                                      VK_FALSE,
                                      VK_LOGIC_OP_COPY,
                                      toU32(color_blend_attachments.size()),
                                      color_blend_attachments.data(),
                                      {0.0, 0.0, 0.0, 0.0}};
  return *this;
}

auto PipelineStateBuilder::DynamicState(const std::vector<VkDynamicState>& dynamic_states) -> PipelineStateBuilder& {
  _state.dynamicStateCreateInfo = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, nullptr, 0,
                                   toU32(dynamic_states.size()), dynamic_states.data()};
  return *this;
}

auto PipelineStateBuilder::PipelineLayout(const std::vector<VkDescriptorSetLayout>& set_layouts,
                                          const std::vector<VkPushConstantRange>& push_constants)
    -> PipelineStateBuilder& {
  _state.pipelineLayoutCreateInfo = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                                     nullptr,
                                     0,
                                     toU32(set_layouts.size()),
                                     set_layouts.data(),
                                     toU32(push_constants.size()),
                                     push_constants.data()};
  return *this;
}

}  // namespace Rendering