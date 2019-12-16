#pragma once

#include "component.h"

class Dial : public ModelComponent {
 public:
  Dial();

  virtual ~Dial() {}

  void Draw(Camera& camera) override;
};
