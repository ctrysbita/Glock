#pragma once

#include "shader.hpp"

class SkyBox {
 private:
  Shader shader_;

 public:
  SkyBox() : shader_("src/sky.vs.glsl", "src/sky.fs.glsl") {}

  ~SkyBox() {}

  Shader& get_shader() { return shader_; }
};
