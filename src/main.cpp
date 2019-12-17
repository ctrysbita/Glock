#include <glad/glad.h>
// Include glad first.
#include <GLFW/glfw3.h>

#include "context.hpp"
#include "dial.h"
#include "earth.h"
#include "jupiter.h"
#include "mars.h"
#include "sky.h"

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
             Jupiter &jupiter) {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // glMatrixMode(GL_MODELVIEW);

  sky.Draw(context);
  earth.Draw(context);
  dial.Draw(context);
  mars.Draw(context);
  jupiter.Draw(context);
  // glUseProgram(0);
  // glBegin(GL_QUADS);
  // glColor3f(0.0, 0.0, 0.0);

  // glVertex3f(0.1, 0.1, 0.0);
  // glVertex3f(0.9, 0.1, 0.0);
  // glVertex3f(0.9, 0.9, 0.0);
  // glVertex3f(0.1, 0.9, 0.0);
  // glEnd();

  glFlush();
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

  auto sky = SkyBox();
  sky.LoadCubeMap();
  auto earth = Earth();
  auto dial = Dial();
  auto mars = Mars();
  auto jupiter = Jupiter();

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
    context.camera_.ProcessKeyboard(window);

    Display(sky, earth, dial, mars, jupiter);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}