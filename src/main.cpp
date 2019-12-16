#include <glad/glad.h>
// Include glad first.
#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "dial.h"
#include "earth.h"
#include "sky.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void Reshape(GLFWwindow* window, GLsizei width, GLsizei height) {
  glViewport(0, 0, width, height);
}

void MouseMovementCallback(GLFWwindow* window, double xpos, double ypos) {
  camera.ProcessMouseMovement(xpos, ypos);
}

void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}

void Display(SkyBox& sky, Earth& earth, Dial& dial) {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // glMatrixMode(GL_MODELVIEW);

  sky.Draw(camera);
  earth.Draw(camera);
  dial.Draw(camera);

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

int main(int argc, char** argv) {
  // Initialization of GLFW.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  auto window = glfwCreateWindow(1280, 720, "Glock", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Error: Failed to create window." << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  // Register callbacks.
  glfwSetFramebufferSizeCallback(window, Reshape);
  glfwSetCursorPosCallback(window, MouseMovementCallback);
  glfwSetScrollCallback(window, MouseScrollCallback);

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

  while (!glfwWindowShouldClose(window)) {
    camera.ProcessKeyboard(window);

    Display(sky, earth, dial);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}