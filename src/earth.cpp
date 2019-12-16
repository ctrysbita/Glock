#include "earth.h"

#include "../resources/models/sphere.inc.h"
#include "time.hpp"

float ang = 0;

Earth::Earth()
    : ModelComponent("src/earth.vs.glsl", "src/earth.fs.glsl", sizeof(vertices),
                     &vertices, sizeof(indices), indices,
                     "resources/textures/earth.jpg") {}

void Earth::Draw(Camera &camera) {
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  shader_.Use();
  auto model = glm::mat4(1.0f);
  ang += 0.1;

  model = glm::rotate(
      model,
      glm::radians(
          float(-(Time::Seconds() + Time::Subseconds() / 10000 / 1000.0) /
                60.0 * 360)),
      glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::translate(model, glm::vec3(0, 0.5, -0.75));
  //  model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
  auto view = camera.GetViewMatrix();
  auto projection = glm::perspective(glm::radians(camera.zoom_),
                                     (float)1280 / (float)720, 0.1f, 100.0f);
  shader_.SetMat4("Model", model);
  shader_.SetMat4("View", view);
  shader_.SetMat4("Projection", projection);
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 11904, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}