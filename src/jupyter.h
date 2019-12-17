#pragma once

#include "component.h"

class Jupyter : public ModelComponent {
 public:
  Jupyter();

  virtual ~Jupyter() {}

  void Draw(Camera& camera) override;
};
