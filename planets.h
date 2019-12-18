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