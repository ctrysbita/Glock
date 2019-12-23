#pragma once

#include <glad/glad.h>
// Include glad first.
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Default values of camera.
constexpr float kDefaultCameraYaw = -90.0;
constexpr float kDefaultCameraPitch = 0.0;
constexpr float kDefaultCameraSpeed = 1.2;
constexpr float kDefaultCameraZoom = 45.0;

/**
 * @brief Utilities for camera controlling.
 */
class Camera {
 private:
  double last_keyboard_time_ = 0;
  bool mouse_moved_ = false;
  float last_mouse_xpos_;
  float last_mouse_ypos_;

  /**
   * @brief Calculates the front, right and up vector from the Camera's
   * euler angles.
   */
  void UpdateCamera() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
  }

 public:
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 world_up_;

  float yaw_;
  float pitch_;

  float speed_ = kDefaultCameraSpeed;
  float zoom_ = kDefaultCameraZoom;

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = kDefaultCameraYaw, float pitch = kDefaultCameraPitch)
      : position_(position), world_up_(up), yaw_(yaw), pitch_(pitch) {
    UpdateCamera();
  }

  /**
   * @brief Get view matrix of current camera.
   *
   * @return glm::mat4 View matrix of current camera.
   */
  glm::mat4 GetViewMatrix() {
    return glm::lookAt(position_, position_ + front_, up_);
  }

  /**
   * @brief Process keyboard acitvity.
   *
   * - W = Go ahead.
   * - S = Go back.
   * - A = Go left.
   * - D = Go right.
   * - SPACE = Go up.
   * - LEFT SHIFT = Go down.
   *
   * @param window GLFW window.
   */
  void ProcessKeyboard(GLFWwindow *window) {
    auto current_time = glfwGetTime();
    auto delta = current_time - last_keyboard_time_;
    last_keyboard_time_ = current_time;
    // Calculate moving distance from speed and delta time.
    float movement = speed_ * (float)delta;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      position_ += front_ * movement;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      position_ -= front_ * movement;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      position_ -= right_ * movement;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      position_ += right_ * movement;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      position_ -= up_ * movement;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      position_ += up_ * movement;
  }

  /**
   * @brief Process mouse movement event.
   *
   * @param xpos X position of mouse.
   * @param ypos Y position of mouse.
   * @param constrain_pitch Enable pitch boundary to prevent flip of screen.
   */
  void ProcessMouseMovement(float xpos, float ypos,
                            bool constrain_pitch = true) {
    if (!mouse_moved_) {
      last_mouse_xpos_ = xpos;
      last_mouse_ypos_ = ypos;
      mouse_moved_ = true;
    }

    float xoffset = xpos - last_mouse_xpos_;
    // Reversed since y goes from bottom to top.
    float yoffset = last_mouse_ypos_ - ypos;

    last_mouse_xpos_ = xpos;
    last_mouse_ypos_ = ypos;

    xoffset *= 0.1f;
    yoffset *= 0.1f;

    yaw_ += xoffset;
    pitch_ += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped.
    if (constrain_pitch) {
      if (pitch_ > 89.0f) pitch_ = 89.0f;
      if (pitch_ < -89.0f) pitch_ = -89.0f;
    }

    UpdateCamera();
  }

  /**
   * @brief Process mouse scroll event and change zoom of camera.
   *
   * @param yoffset Offset of scroll.
   */
  void ProcessMouseScroll(float yoffset) {
    if (zoom_ >= 1.0f && zoom_ <= 45.0f) zoom_ -= yoffset;
    if (zoom_ <= 1.0f) zoom_ = 1.0f;
    if (zoom_ >= 45.0f) zoom_ = 45.0f;
  }
};
