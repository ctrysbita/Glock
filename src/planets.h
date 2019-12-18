#pragma once

#include "component.h"

/**
 * @brief Class for the Earth.
 */
class Earth : public ModelComponent {
 public:
  Earth();

  virtual ~Earth() {}

  glm::mat4 ModelTransformation(Context& context) override;
  void Draw(Context& context) override;
  void DrawDepthMap(Context& context) override;
};

/**
 * @brief Class for the Mars.
 */
class Mars : public ModelComponent {
 public:
  Mars();

  virtual ~Mars() {}

  glm::mat4 ModelTransformation(Context& context) override;
  void Draw(Context& context) override;
  void DrawDepthMap(Context& context) override;
};

/**
 * @brief Class for the Jupiter.
 */
class Jupiter : public ModelComponent {
 public:
  Jupiter();

  virtual ~Jupiter() {}

  glm::mat4 ModelTransformation(Context& context) override;
  void Draw(Context& context) override;
  void DrawDepthMap(Context& context) override;
};