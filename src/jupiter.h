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

  /**
   * @brief Draw an Jupiter object.
   *
   * @param context Information for model position and camera.
   */
  void Draw(Context& context) override;
};
