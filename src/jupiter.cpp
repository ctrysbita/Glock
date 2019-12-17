#include "jupiter.h"

#include "../resources/models/sphere.inc.h"
#include "time.hpp"

Jupiter::Jupiter()
    : ModelComponent("src/sphere.vs.glsl", "src/sphere.fs.glsl",
                     sizeof(vertices), &vertices, sizeof(indices), indices,
                     "resources/textures/jupiter.jpg") {}

void Jupiter::Draw(Context& context) {
  auto model = context.clock_position_;
  model = glm::rotate(
      model,
      glm::radians(float(-(Time::Seconds() + Time::Milliseconds() / 1000.0) /
                         60.0 * 360)),
      glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::translate(model, glm::vec3(0, 0.3, -0.9));
  model = Context::RotateEcliptic(model);
  model = Context::PlanetRotate(model, 0.13f);
  model = glm::scale(model, glm::vec3(1.75, 1.75, 1.75));

  auto view = context.camera_.GetViewMatrix();
  auto projection = glm::perspective(glm::radians(context.camera_.zoom_),
                                     context.Ratio(), 0.1f, 100.0f);

  glBindTexture(GL_TEXTURE_2D, texture_id_);
  shader_.Use();
  shader_.SetMat4("Model", model);
  shader_.SetMat4("View", view);
  shader_.SetMat4("Projection", projection);
  shader_.SetVec3("ViewPos", context.camera_.position_);
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 11904, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}