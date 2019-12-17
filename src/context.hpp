#pragma once

#include "camera.hpp"

class Context {
 private:
  GLsizei window_width_;
  GLsizei window_height_;

 public:
  Camera camera_;

  Context() : camera_(glm::vec3(0.0f, 0.0f, 0.0f)) {}
  ~Context() {}

  void HandleReshape(GLsizei width, GLsizei height) {
    window_width_ = width;
    window_height_ = height;
  }
};