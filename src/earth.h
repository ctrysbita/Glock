#pragma once

#include "component.h"

class Earth : public ModelComponent {
 public:
  Earth();

  virtual ~Earth() {}

  void Draw(Camera& camera) override;
};
