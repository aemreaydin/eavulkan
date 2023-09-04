#pragma once

#include <eavulkan/shared/includes.hpp>

namespace EA {

struct ModelViewProjection {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;

  explicit ModelViewProjection( glm::mat4 model = glm::mat4( 1.F ), glm::mat4 view = glm::mat4( 1.F ),
                                glm::mat4 projection = glm::mat4( 1.F ) );
};

}  // namespace EA