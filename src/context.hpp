#pragma once

#include "camera.hpp"

class Context {
 private:
  GLsizei window_width_;
  GLsizei window_height_;
  glm::mat4 global_init_mat_;

 public:
  Camera camera_;

  Context()
      : camera_(glm::vec3(0.0f, 0.0f, 0.0f)),
        window_width_(1280),
        window_height_(720),
        global_rotation_mat_(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f),
                                         glm::vec3(1.0, 0.0, 0.0))) {}
  ~Context() {}

  void HandleReshape(GLsizei width, GLsizei height) {
    window_width_ = width;
    window_height_ = height;
  }

  float Ratio() { return (float)window_width_ / window_height_; }
  glm::mat4 GetInitMat() { return global_init_mat_; }
};