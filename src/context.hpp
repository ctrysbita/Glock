#pragma once

#include "camera.hpp"
#include "time.hpp"

class Context {
 private:
  GLsizei window_width_ = 1280;
  GLsizei window_height_ = 720;

 public:
  const glm::mat4 clock_position_ = glm::rotate(
      glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));

  Camera camera_;

  glm::vec3 light_position_ = glm::vec3(3, 3, 3);

  Context() : camera_(glm::vec3(0.0f, 0.0f, 0.0f)) {}
  ~Context() {}

  /**
   * @brief Update width and height in a Context object.
   *
   * @param width The new window width.
   * @param height The new window height.
   */
  void HandleReshape(GLsizei width, GLsizei height) {
    window_width_ = width;
    window_height_ = height;
  }

  /**
   * @brief Get the ratio of window.
   *
   * @return float Ratio of window width and height.
   */
  float Ratio() { return (float)window_width_ / window_height_; }

  static glm::mat4 RotateEcliptic(glm::mat4 origin) {
    return glm::rotate(origin, glm::radians(-30.0f),
                       glm::vec3(1.0f, 0.0f, 0.0f));
  }

  static glm::mat4 PlanetRotate(glm::mat4 origin, float speed) {
    return glm::rotate(
        origin,
        glm::radians(float(-(Time::Seconds() + Time::Milliseconds() / 1000.0) *
                           speed * 360)),
        glm::vec3(0.0f, 1.0f, 0.0f));
  }
};