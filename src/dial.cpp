#include "dial.h"

#include "../resources/models/dial.inc.h"

Dial::Dial()
    : ModelComponent("src/dial.vs.glsl", "src/dial.fs.glsl", sizeof(vertices),
                     &vertices, sizeof(indices), indices,
                     "resources/textures/dial.jpg") {}

glm::mat4 Dial::ModelTransformation(Context& context) {
  auto model = context.kClockPosition;
  model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
  return model;
}

void Dial::Draw(Context& context) {
  auto model = ModelTransformation(context);
  auto view = context.camera_.GetViewMatrix();
  auto projection = glm::perspective(glm::radians(context.camera_.zoom_),
                                     context.Ratio(), 0.1f, 100.0f);

  auto light_view = glm::lookAt(context.light_position_, glm::vec3(0.0f),
                                glm::vec3(0.0, 1.0, 0.0));
  auto light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);
  auto light_space = light_projection * light_view;
  // Pass information to shader.
  shader_.Use();
  shader_.SetMat4("Model", model);
  shader_.SetMat4("View", view);
  shader_.SetMat4("Projection", projection);
  shader_.SetVec3("ViewPos", context.camera_.position_);
  shader_.SetMat4("LightSpace", light_space);
  shader_.SetInt("Texture", 0);
  shader_.SetInt("ShadowMap", 1);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 2682, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Dial::DrawDepthMap(Context& context) {
  auto model = ModelTransformation(context);
  auto light_view = glm::lookAt(context.light_position_, glm::vec3(0.0f),
                                glm::vec3(0.0, 1.0, 0.0));
  auto light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);
  auto light_space = light_projection * light_view;

  context.get_depth_map_shader().Use();
  context.get_depth_map_shader().SetMat4("LightSpace", light_space);
  context.get_depth_map_shader().SetMat4("Model", model);
  context.get_depth_map_shader().SetFloat("PosFactor", 1.5f);

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 2682, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}