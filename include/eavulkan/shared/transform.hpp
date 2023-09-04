#pragma once

#include <eavulkan/shared/includes.hpp>

namespace EA {

struct Transform {
  glm::vec3 location;
  glm::vec3 scale;
  glm::vec3 rotation;

  explicit Transform( glm::vec3 translate = glm::vec3( 0.F ), glm::vec3 scale = glm::vec3( 1.F ),
                      glm::vec3 rotate = glm::vec3( 0.F ) );
};

}  // namespace EA