#include "earth.h"

#include "../resources/models/sphere.inc.h"
#include "time.hpp"

Earth::Earth()
    : ModelComponent("src/earth.vs.glsl", "src/earth.fs.glsl", sizeof(vertices),
                     &vertices, sizeof(indices), indices,
                     "resources/textures/earth.jpg") {}

void Earth::Draw(Context& context) {
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  shader_.Use();
  auto model = context.GetInitMat();

  auto hour = Time::Hours() + 8;
  if (hour >= 12) hour -= 12;
  model = glm::rotate(
      model, glm::radians(float(-(hour + Time::Minutes() / 60.0) / 12.0 * 360)),
      glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::translate(model, glm::vec3(0, 0.3, -0.3));
  model = Context::RotateEcliptic(model);
  model = Context::PlanetRotate(model, 0.05);

  auto view = context.camera_.GetViewMatrix();
  auto projection = glm::perspective(glm::radians(context.camera_.zoom_),
                                     context.Ratio(), 0.1f, 100.0f);
  shader_.SetMat4("Model", model);
  shader_.SetMat4("View", view);
  shader_.SetMat4("Projection", projection);
  shader_.SetVec3("ViewPos", context.camera_.position_);
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 11904, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}