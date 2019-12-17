#pragma once

#include "component.h"

class Jupiter : public ModelComponent {
 public:
  Jupiter();

  virtual ~Jupiter() {}

  void Draw(Context& context) override;
};
