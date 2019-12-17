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

  /**
   * @brief Draw an Earth object.
   *
   * @param context Information for model position and camera.
   */
  void Draw(Context& context) override;
};
