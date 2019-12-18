#include <glad/glad.h>
// Include glad first.
#include <GLFW/glfw3.h>

#include "context.hpp"
#include "dial.h"
#include "particle.h"
#include "planets.h"
#include "sky.h"

Context context;

/**
 * @brief Reshape callback to handle size change of window.
 *
 * @param window GLFW window.
 * @param width New window width.
 * @param height New window height.
 */
void Reshape(GLFWwindow *window, GLsizei width, GLsizei height) {
  glViewport(0, 0, width, height);
  context.HandleReshape(width, height);
}

/**
 * @brief Callback to handle mouse movement event.
 *
 * @param window GLFW window.
 * @param xpos X position of mouse.
 * @param ypos Y position of mouse.
 */
void MouseMovementCallback(GLFWwindow *window, double xpos, double ypos) {
  context.get_camera().ProcessMouseMovement(xpos, ypos);
}

/**
 * @brief Callback to handle mouse scroll event.
 *
 * @param window  GLFW window.
 * @param xoffset X offset of scroll ball.
 * @param yoffset Y offset of scroll ball.
 */
void MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  context.get_camera().ProcessMouseScroll(yoffset);
}

/**
 * @brief Display function that render the whole scene.
 *
 * @param sky Sky box.
 * @param dial Clock dial.
 * @param earth Earth. (hour)
 * @param mars Mars. (minute)
 * @param jupiter Jupyter. (jupyter)
 * @param particle Particle effect.
 */
void Display(SkyBox &sky, Dial &dial, Earth &earth, Mars &mars,
             Jupiter &jupiter, ParticleGenerator &particle) {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, 1024, 1024);
  glBindFramebuffer(GL_FRAMEBUFFER, context.depth_map_frame);
  glClear(GL_DEPTH_BUFFER_BIT);

  dial.DrawDepthMap(context);
  earth.DrawDepthMap(context);
  mars.DrawDepthMap(context);
  jupiter.DrawDepthMap(context);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, context.window_width, context.window_height);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, context.depth_map_texture);

  sky.Draw(context);
  dial.Draw(context);
  earth.Draw(context);
  mars.Draw(context);
  jupiter.Draw(context);
  particle.Update(0.01, context, 10);
  particle.Draw(context);
}

int main(int argc, char **argv) {
  // Initialization of GLFW.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  auto window = glfwCreateWindow(1280, 720, "Glock", NULL, NULL);
  if (window == NULL) {
    auto err_code = glfwGetError(NULL);
    std::cerr << "Error: Failed to create window. code: " << err_code
              << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Register callbacks.
  glfwSetFramebufferSizeCallback(window, Reshape);
  glfwSetCursorPosCallback(window, MouseMovementCallback);
  glfwSetScrollCallback(window, MouseScrollCallback);

  // Capture mouse.
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Initialization of GLAD.
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Initialization of GL.
  glEnable(GL_DEPTH_TEST);

  // Initialization of depth map.
  context.InitDepthMap();

  // Creating objects.
  auto sky = SkyBox();
  sky.LoadCubeMap();
  auto earth = Earth();
  auto dial = Dial();
  auto mars = Mars();
  auto jupiter = Jupiter();
  auto particle = ParticleGenerator("resources/textures/particle.jpg", 500);

  while (!glfwWindowShouldClose(window)) {
    // Process keyboard events.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
    context.get_camera().ProcessKeyboard(window);

    // Render scene.
    Display(sky, dial, earth, mars, jupiter, particle);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}