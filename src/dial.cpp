#include "dial.h"

#include "../resources/models/dial.inc.h"

Dial::Dial()
    : ModelComponent("src/dial.vs.glsl", "src/dial.fs.glsl", sizeof(vertices),
                     &vertices, sizeof(indices), indices,
                     "resources/textures/dial.jpg") {}

void Dial::Draw(Context& context) {
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  shader_.Use();
  auto model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
  auto view = context.camera_.GetViewMatrix();
  auto projection = glm::perspective(glm::radians(context.camera_.zoom_),
                                     context.Ratio(), 0.1f, 100.0f);
  shader_.SetMat4("Model", model);
  shader_.SetMat4("View", view);
  shader_.SetMat4("Projection", projection);
  shader_.SetVec3("ViewPos", context.camera_.position_);
  auto aa = glGetError();
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 11904, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
