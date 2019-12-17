#pragma once

#include "component.h"

class Dial : public ModelComponent {
 public:
  Dial();

  virtual ~Dial() {}

  void Draw(Context& context) override;
};
