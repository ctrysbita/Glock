#include "../resources/models/sphere.inc.h"
#include "planets.h"
#include "time.hpp"

Mars::Mars()
    : ModelComponent("src/sphere.vs.glsl", "src/sphere.fs.glsl",
                     sizeof(vertices), &vertices, sizeof(indices), indices,
                     "resources/textures/mars.jpg") {}

glm::mat4 Mars::ModelTransformation(Context& context) {
  auto angle = glm::radians(
      float((Time::Minutes() +
             (Time::Seconds() + Time::Milliseconds() / 1000.0) / 60.0) /
            60.0 * 360));
  float radius = 0.6;
  auto pos_percent = 0.9f;
  auto model = context.kClockPosition;
  context.mars_pos_ =
      glm::vec3(sin(angle) * radius, cos(angle) * radius, 0.0f) * pos_percent;

  model = glm::rotate(model, -angle, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::translate(model, glm::vec3(0, 0.3, -radius));
  model = Context::RotateEquator(model);
  model = Context::RotatePlanet(model, 0.05f);
  model = glm::scale(model, glm::vec3(.75, .75, .75));
  return model;
}

void Mars::Draw(Context& context) {
  auto model = ModelTransformation(context);
  auto view = context.get_camera().GetViewMatrix();
  auto projection = glm::perspective(glm::radians(context.get_camera().zoom_),
                                     context.Ratio(), 0.1f, 100.0f);

  shader_.Use();
  shader_.SetMat4("Model", model);
  shader_.SetMat4("View", view);
  shader_.SetMat4("Projection", projection);
  shader_.SetVec3("ViewPos", context.get_camera().position_);
  shader_.SetVec3("LightPos", context.light_position_);

  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 11904, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mars::DrawDepthMap(Context& context) {
  auto model = ModelTransformation(context);
  auto light_view = glm::lookAt(context.light_position_, glm::vec3(0.0f),
                                glm::vec3(0.0, 1.0, 0.0));
  auto light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);
  auto light_space = light_projection * light_view;

  context.get_depth_map_shader().Use();
  context.get_depth_map_shader().SetMat4("LightSpace", light_space);
  context.get_depth_map_shader().SetMat4("Model", model);
  context.get_depth_map_shader().SetFloat("PosFactor", 0.016f);

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 11904, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}