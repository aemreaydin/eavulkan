#pragma once

#include <eavulkan/rendering/resource.hpp>
#include <eavulkan/shared/includes.hpp>

namespace EA::Rendering {

struct PipelineState {
  VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{};
  std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos{};
  VkPipelineTessellationStateCreateInfo tesselationStateCreateInfo{};
  VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
  VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
  VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
  VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo{};
  VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{};
  VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};

  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};

  std::vector<VkShaderModule> shaderModules{};
  std::vector<VkVertexInputBindingDescription> vertexInputBindingDescriptions{};
  std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions{};
};

class PipelineStateBuilder {
  const Device* _device;
  PipelineState _state;

 public:
  explicit PipelineStateBuilder( const Device& device );

  [[nodiscard]] auto GetState() const -> const PipelineState&;

  auto VertexInputState( std::vector<VkVertexInputBindingDescription> bindings,
                         std::vector<VkVertexInputAttributeDescription> attributes ) -> PipelineStateBuilder&;

  auto InputAssemblyState( VkPrimitiveTopology topology, VkBool32 primitive_restart_enable ) -> PipelineStateBuilder&;

  auto ShaderStage( VkShaderStageFlagBits shader_stage, const std::vector<uint32_t>& shader_code )
    -> PipelineStateBuilder&;

  auto ViewportState( const std::vector<VkViewport>& viewports, const std::vector<VkRect2D>& scissors )
    -> PipelineStateBuilder&;

  auto RasterizationState( VkBool32 depth_clamp, VkPolygonMode polygon_mode, VkCullModeFlags cull_mode,
                           VkFrontFace front_face ) -> PipelineStateBuilder&;

  auto MultisampleState() -> PipelineStateBuilder&;
  auto DepthStencilState() -> PipelineStateBuilder&;
  auto ColorBlendState( const std::vector<VkPipelineColorBlendAttachmentState>& color_blend_attachments )
    -> PipelineStateBuilder&;
  auto DynamicState( const std::vector<VkDynamicState>& dynamic_states ) -> PipelineStateBuilder&;

  auto PipelineLayout( const std::vector<VkDescriptorSetLayout>& set_layouts,
                       const std::vector<VkPushConstantRange>& push_constants ) -> PipelineStateBuilder&;
};

class Pipeline : public DeviceResource<VkPipeline> {
  VkPipelineLayout _pipelineLayout{ nullptr };
  VkPipelineCreationFeedback _pipelineCreationFeedback{};
  std::vector<VkPipelineCreationFeedback> _pipelineStageCreationFeedbacks{};

 protected:
  auto getLayout() -> VkPipelineLayout&;

 public:
  explicit Pipeline( const Device& device, const PipelineState& state );

  auto Cleanup() -> void override;
};

}  // namespace EA::Rendering
