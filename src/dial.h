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

  /**
   * @brief Draw a clock dial.
   *
   * @param context Information for model position and camera.
   */
  void Draw(Context& context) override;
};
