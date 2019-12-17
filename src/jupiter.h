#pragma once

#include "component.h"

/**
 * @brief Class for the Jupiter object.
 *
 */
class Jupiter : public ModelComponent {
 public:
  Jupiter();

  virtual ~Jupiter() {}

  glm::mat4 ModelTransformation(Context& context) override;

  void Draw(Context& context) override;

  void DrawDepthMap(Context& context) override;
};
