#pragma once

#include <eavulkan/common/includes.hpp>
#include <eavulkan/pipeline.hpp>

namespace Rendering {

class RenderPass;
class GraphicsPipeline : public Pipeline {
 public:
  GraphicsPipeline( const Device& device, const RenderPass& render_pass, const PipelineState& state );
};

}  // namespace Rendering