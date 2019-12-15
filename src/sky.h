#pragma once

#include <array>
#include <glm/glm.hpp>
#include <string>

#include "camera.hpp"
#include "shader.hpp"

class SkyBox {
 private:
  // 6(v per face) * 6(faces) * 3(axis) = 108
  static const float kVertices[108];

  static const std::array<std::string, 6> kTextures;

  unsigned int vao_;

  unsigned int vbo_;

  unsigned int texture_id_ = 0;

  Shader shader_;

 public:
  SkyBox() : shader_("src/sky.vs.glsl", "src/sky.fs.glsl") {
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), &kVertices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glEnableVertexAttribArray(0);
  }

  ~SkyBox() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vao_);
  }

  Shader& get_shader() { return shader_; }

  void LoadCubeMap();

  void Draw(Camera& camera);
};
