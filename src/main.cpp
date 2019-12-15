#include <glad/glad.h>
// Include glad first.

#include <GLFW/glfw3.h>

#include "sky.hpp"

void reshape(GLFWwindow* window, GLsizei width, GLsizei height) {
  glViewport(0, 0, width, height);
}

void display() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glColor3f(1, 0, 0);

  auto sky = SkyBox();
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
  glfwSetFramebufferSizeCallback(window, reshape);

  // Initialization of GLAD.
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Initialization of GL.
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  while (!glfwWindowShouldClose(window)) {
    display();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}