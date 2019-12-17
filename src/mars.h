#pragma once

#include "component.h"

class Mars : public ModelComponent {
 public:
  Mars();

  virtual ~Mars() {}

  void Draw(Context& context) override;
};
