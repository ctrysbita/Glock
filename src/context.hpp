#pragma once

#include "camera.hpp"
#include "shader.hpp"
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

  // Depth map to generate shadow texture.
  unsigned int depth_map_frame_ = 0;
  unsigned int depth_map_texture_ = 0;
  Shader depth_map_shader_;

  Context()
      : camera_(glm::vec3(0.0f, 0.0f, 0.0f)),
        depth_map_shader_("src/depth.vs.glsl", "src/depth.vs.glsl") {
    // Init depth map frame buffer and texture.
    // InitDepthMap();
  }
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

  void InitDepthMap() {
    glGenFramebuffers(1, &depth_map_frame_);
    glGenTextures(1, &depth_map_texture_);

    glBindTexture(GL_TEXTURE_2D, depth_map_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Attach depth texture as FBO's depth buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_frame_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depth_map_texture_, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  static glm::mat4 PlanetRotate(glm::mat4 origin, float speed) {
    return glm::rotate(
        origin,
        glm::radians(float(-(Time::Seconds() + Time::Milliseconds() / 1000.0) *
                           speed * 360)),
        glm::vec3(0.0f, 1.0f, 0.0f));
  }
};