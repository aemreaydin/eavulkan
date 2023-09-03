#include <eavulkan/common/common.hpp>
#include <eavulkan/device.hpp>
#include <eavulkan/graphics_pipeline.hpp>
#include <eavulkan/render_pass.hpp>

namespace Rendering {

GraphicsPipeline::GraphicsPipeline( const Device& device, const RenderPass& render_pass, const PipelineState& state )
  : Pipeline( device, state ) {
  VkPipelineCreationFeedback pipelineCreationFeedback{};
  std::vector<VkPipelineCreationFeedback> pipelineStageCreationFeedbacks( state.shaderModules.size() );
  const VkPipelineCreationFeedbackCreateInfo pipelineCreationFeedbackCreateInfo{
    VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO, nullptr, &pipelineCreationFeedback,
    toU32( pipelineStageCreationFeedbacks.size() ), pipelineStageCreationFeedbacks.data()
  };

  VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{ VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, nullptr,
                                                           0 };

  if ( std::find( device.GetEnabledExtensions().begin(), device.GetEnabledExtensions().end(),
                  "VK_EXT_pipeline_creation_feedback" ) != device.GetEnabledExtensions().end() ) {
    graphicsPipelineCreateInfo.pNext = &pipelineCreationFeedbackCreateInfo;
  }

  graphicsPipelineCreateInfo.layout = getLayout();
  graphicsPipelineCreateInfo.renderPass = render_pass.GetHandle();
  graphicsPipelineCreateInfo.stageCount = toU32( state.shaderStageCreateInfos.size() );
  graphicsPipelineCreateInfo.pStages = state.shaderStageCreateInfos.data();
  graphicsPipelineCreateInfo.pInputAssemblyState = &state.inputAssemblyStateCreateInfo;
  graphicsPipelineCreateInfo.pVertexInputState = &state.vertexInputStateCreateInfo;
  graphicsPipelineCreateInfo.pTessellationState = &state.tesselationStateCreateInfo;
  graphicsPipelineCreateInfo.pViewportState = &state.viewportStateCreateInfo;
  graphicsPipelineCreateInfo.pRasterizationState = &state.rasterizationStateCreateInfo;
  graphicsPipelineCreateInfo.pMultisampleState = &state.multisampleStateCreateInfo;
  graphicsPipelineCreateInfo.pDepthStencilState = &state.depthStencilStateCreateInfo;
  graphicsPipelineCreateInfo.pColorBlendState = &state.colorBlendStateCreateInfo;
  graphicsPipelineCreateInfo.pDynamicState = &state.dynamicStateCreateInfo;

  vkCheck( vkCreateGraphicsPipelines( GetDevice().GetHandle(), nullptr, 1, &graphicsPipelineCreateInfo, nullptr,
                                      &GetHandle() ),
           "Failed to create graphics pipeline" );

  // Cleanup shader modules after pipeline creation
  for ( VkShaderModule shaderModule : state.shaderModules ) {
    vkDestroyShaderModule( GetDevice().GetHandle(), shaderModule, nullptr );
  }
}

}  // namespace Rendering