#pragma once

#include <glad/glad.h>
// Include glad first.
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Default values.
constexpr float kDefaultCameraYaw = -90.0;
constexpr float kDefaultCameraPitch = 0.0;
constexpr float kDefaultCameraSpeed = 1.5;
constexpr float kDefaultCameraZoom = 45.0;

class Camera {
 private:
  double last_keyboard_time_ = 0;
  bool mouse_moved_ = false;
  float last_mouse_xpos_;
  float last_mouse_ypos_;

  // Calculates the front vector from the Camera's (updated) Euler Angles
  void UpdateCamera() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    // Also re-calculate the Right and Up vector
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

  glm::mat4 GetViewMatrix() {
    return glm::lookAt(position_, position_ + front_, up_);
  }

  void ProcessKeyboard(GLFWwindow *window) {
    auto current_time = glfwGetTime();
    auto delta = current_time - last_keyboard_time_;
    last_keyboard_time_ = current_time;

    float movement = speed_ * delta;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      position_ += front_ * movement;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      position_ -= front_ * movement;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      position_ -= right_ * movement;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      position_ += right_ * movement;
  }

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

    xoffset *= 0.1;
    yoffset *= 0.1;

    yaw_ += xoffset;
    pitch_ += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped.
    if (constrain_pitch) {
      if (pitch_ > 89.0f) pitch_ = 89.0f;
      if (pitch_ < -89.0f) pitch_ = -89.0f;
    }

    UpdateCamera();
  }

  void ProcessMouseScroll(float yoffset) {
    if (zoom_ >= 1.0f && zoom_ <= 45.0f) zoom_ -= yoffset;
    if (zoom_ <= 1.0f) zoom_ = 1.0f;
    if (zoom_ >= 45.0f) zoom_ = 45.0f;
  }
};
