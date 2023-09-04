#pragma once

#include <eavulkan/rendering/pipeline.hpp>
#include <eavulkan/shared/includes.hpp>

namespace EA::Rendering {

class RenderPass;
class GraphicsPipeline : public Pipeline {
 public:
  GraphicsPipeline( const Device& device, const RenderPass& render_pass, const PipelineState& state );
};

}  // namespace EA::Rendering
