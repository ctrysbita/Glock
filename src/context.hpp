#pragma once

#include "camera.hpp"
#include "optional"
#include "shader.hpp"
#include "time.hpp"

class Context {
 private:
  // Window properties for perspective projection.

  GLsizei window_width_ = 1280;
  GLsizei window_height_ = 720;

  // Camera.

  Camera camera_;

  // Depth map and depth map shader to generate shadow texture.

  unsigned int depth_map_frame_ = 0;
  unsigned int depth_map_texture_ = 0;
  std::optional<Shader> depth_map_shader_;

 public:
  // The resolution of depth map. Higher resolution can provide a better shadow.
  const GLsizei kDepthMapResolution = 5120;
  // Rotate the clock dial and planets to vertical position.
  const glm::mat4 kClockPosition =
      glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f),
                              glm::vec3(1.0, 0.0, 0.0)),
                  glm::radians(3.0f), glm::vec3(0.0, 1.0, 0.0));

  // Public const getters.

  const GLsizei& window_width = window_width_;
  const GLsizei& window_height = window_height_;

  const unsigned int& depth_map_frame = depth_map_frame_;
  const unsigned int& depth_map_texture = depth_map_texture_;

  // Public properties.

  glm::vec3 light_position_ = glm::vec3(2.0f, 2.0f, 2.0f);
  bool enable_shadow_ = true;

  glm::vec3 earth_pos_;
  glm::vec3 mars_pos_;

  Context() : camera_(glm::vec3(0.0f, 0.0f, 3.0f)) {}
  ~Context() {}

  inline Camera& get_camera() { return camera_; }
  inline Shader& get_depth_map_shader() { return depth_map_shader_.value(); }

  /**
   * @brief Utility for rotating planet's equator to produce obliquity of the
   * ecliptic.
   *
   * @param origin Original model matrix.
   * @return glm::mat4 Rotated model matrix.
   */
  static glm::mat4 RotateEquator(glm::mat4 origin) {
    return glm::rotate(origin, glm::radians(-30.0f),
                       glm::vec3(1.0f, 0.0f, 0.0f));
  }

  /**
   * @brief Utility for rotating planet itself.
   *
   * @param origin Original model matrix.
   * @param speed Rotation speed.
   * @return glm::mat4 Rotated model matrix.
   */
  static glm::mat4 RotatePlanet(glm::mat4 origin, float speed) {
    return glm::rotate(
        origin,
        glm::radians(float(-(Time::Seconds() + Time::Milliseconds() / 1000.0) *
                           speed * 360)),
        glm::vec3(0.0f, 1.0f, 0.0f));
  }

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
  inline float Ratio() { return (float)window_width_ / window_height_; }

  /**
   * @brief Initialize depth map frame buffer, texture and shader.
   */
  void InitDepthMap() {
    // Generate frame buffer and texture for depth map.
    glGenFramebuffers(1, &depth_map_frame_);
    glGenTextures(1, &depth_map_texture_);

    // Bind to texture and configure parameters.
    glBindTexture(GL_TEXTURE_2D, depth_map_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, kDepthMapResolution,
                 kDepthMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Attach depth texture as frame buffer object's depth buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_frame_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depth_map_texture_, 0);

    // Disable color rendering for depth map.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Release frame buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create shader for depth map.
    depth_map_shader_.emplace(Shader("src/depth.vs.glsl", "src/depth.fs.glsl"));
  }
};