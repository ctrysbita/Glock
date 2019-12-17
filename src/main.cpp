#include <glad/glad.h>
// Include glad first.
#include <GLFW/glfw3.h>

#include "context.hpp"
#include "dial.h"
#include "earth.h"
#include "jupiter.h"
#include "mars.h"
#include "sky.h"
#include "particle.h"

Context context;

void Reshape(GLFWwindow *window, GLsizei width, GLsizei height) {
  glViewport(0, 0, width, height);
  context.HandleReshape(width, height);
}

void MouseMovementCallback(GLFWwindow *window, double xpos, double ypos) {
  context.camera_.ProcessMouseMovement(xpos, ypos);
}

void MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  context.camera_.ProcessMouseScroll(yoffset);
}

void Display(SkyBox &sky, Earth &earth, Dial &dial, Mars &mars,
             Jupiter &jupiter, ParticleGenerator &particle) {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, 1024, 1024);
  glBindFramebuffer(GL_FRAMEBUFFER, context.depth_map_frame_);

  dial.DrawDepthMap(context);
  earth.DrawDepthMap(context);
  mars.DrawDepthMap(context);
  jupiter.DrawDepthMap(context);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, context.window_width_, context.window_height_);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, context.depth_map_texture_);

  sky.Draw(context);
  dial.Draw(context);
  earth.Draw(context);
  mars.Draw(context);
  jupiter.Draw(context);
  particle.Update(0.01, context, 10);
  particle.Draw(context);
  

  // glUseProgram(0);
  // glBegin(GL_QUADS);
  // glColor3f(0.0, 0.0, 0.0);
  // glVertex3f(0.1, 0.1, 0.0);
  // glVertex3f(0.9, 0.1, 0.0);
  // glVertex3f(0.9, 0.9, 0.0);
  // glVertex3f(0.1, 0.9, 0.0);
  // glEnd();
  // glFlush();
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
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  context.InitDepthMap();

  auto sky = SkyBox();
  sky.LoadCubeMap();
  auto earth = Earth();
  auto dial = Dial();
  auto mars = Mars();
  auto jupiter = Jupiter();
  auto particle = ParticleGenerator("resources/textures/particle.jpg", 500);

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
    context.camera_.ProcessKeyboard(window);

    Display(sky, earth, dial, mars, jupiter, particle);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}