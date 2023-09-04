#include <eavulkan/shared/mvp.hpp>

namespace EA {

EA::ModelViewProjection::ModelViewProjection( glm::mat4 model, glm::mat4 view, glm::mat4 projection )
  : model( model ), view( view ), projection( projection ) {}

}  // namespace EA