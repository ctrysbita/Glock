#pragma once

#include "component.h"

/**
 * @brief Class for the Earth object.
 *
 */
class Earth : public ModelComponent {
 public:
  Earth();

  virtual ~Earth() {}

  glm::mat4 ModelTransformation(Context& context) override;

  void Draw(Context& context) override;

  void DrawDepthMap(Context& context) override;
};
