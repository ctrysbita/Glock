#pragma once

#include "component.h"

/**
 * @brief Class for the Mars object.
 *
 */
class Mars : public ModelComponent {
 public:
  Mars();

  virtual ~Mars() {}

  glm::mat4 ModelTransformation(Context& context) override;

  void Draw(Context& context) override;

  void DrawDepthMap(Context& context) override;
};
