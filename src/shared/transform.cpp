#include <eavulkan/shared/transform.hpp>

namespace EA {

Transform::Transform( glm::vec3 translate, glm::vec3 scale, glm::vec3 rotate )
  : location( translate ), scale( scale ), rotation( rotate ) {}

}  // namespace EA
