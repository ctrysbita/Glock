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
  /**
   * @brief Draw a Mars object.
   *
   * @param context Information for model position and camera.
   */
  void Draw(Context& context) override;
};
