#include "earth.h"

#include "../resources/models/sphere.inc.h"
#include "time.hpp"

Earth::Earth()
    : ModelComponent("src/sphere.vs.glsl", "src/sphere.fs.glsl",
                     sizeof(vertices), &vertices, sizeof(indices), indices,
                     "resources/textures/earth.jpg") {}

glm::mat4 Earth::ModelTransformation(Context& context) {
  auto model = context.clock_position_;
  auto hour = Time::Hours() + 8;
  if (hour >= 12) hour -= 12;
  model = glm::rotate(
      model, glm::radians(float(-(hour + Time::Minutes() / 60.0) / 12.0 * 360)),
      glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::translate(model, glm::vec3(0, 0.3, -0.3));
  model = Context::RotateEcliptic(model);
  model = Context::PlanetRotate(model, 0.05f);
  return model;
}

void Earth::Draw(Context& context) {
  auto model = ModelTransformation(context);
  auto view = context.camera_.GetViewMatrix();
  auto projection = glm::perspective(glm::radians(context.camera_.zoom_),
                                     context.Ratio(), 0.1f, 100.0f);

  shader_.Use();
  shader_.SetMat4("Model", model);
  shader_.SetMat4("View", view);
  shader_.SetMat4("Projection", projection);
  shader_.SetVec3("ViewPos", context.camera_.position_);

  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 11904, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Earth::DrawDepthMap(Context& context) {
  auto model = ModelTransformation(context);
  auto light_view = glm::lookAt(context.light_position_, glm::vec3(0.0f),
                                glm::vec3(0.0, 1.0, 0.0));
  auto light_projection =
      glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
  auto light_space = light_projection * light_view;

  context.get_depth_map_shader().Use();
  context.get_depth_map_shader().SetMat4("LightSpace", light_space);
  context.get_depth_map_shader().SetMat4("Model", model);
  context.get_depth_map_shader().SetFloat("PosFactor", 0.016f);

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 11904, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}