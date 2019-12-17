#pragma once

#include "component.h"

/**
 * @brief Class for the clock dial.
 *
 */
class Dial : public ModelComponent {
 public:
  Dial();

  virtual ~Dial() {}

  glm::mat4 ModelTransformation(Context& context) override;

  void Draw(Context& context) override;

  void DrawDepthMap(Context& context) override;
};
