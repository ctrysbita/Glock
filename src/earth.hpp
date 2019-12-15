#pragma once

#include <stb_image.h>

#include "../resources/models/earth.inc.h"
#include "camera.hpp"
#include "shader.hpp"

class Earth {
 private:
  unsigned int vao_;

  unsigned int vbo_;

  unsigned int ebo_;

  unsigned int texture_id_ = 0;

  Shader shader_;

 public:
  Earth() : shader_("src/earth.vs.glsl", "src/earth.fs.glsl") {
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glBufferData(GL_ARRAY_BUFFER, sizeof(modelVertices), &modelVertices,
                 GL_STATIC_DRAW);

    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(modelIndices), modelIndices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex),
                          (void*)sizeof(ModelVertex::position));
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex),
        (void*)(sizeof(ModelVertex::position) + sizeof(ModelVertex::normal)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
  }

  ~Earth() {}

  Shader& get_shader() { return shader_; }

  void LoadTexture() {
    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    int width, height, channels;
    unsigned char* data = stbi_load("resources/textures/earth.jpg", &width,
                                    &height, &channels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cerr << "Error: Failed to load texture." << std::endl;
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  void Draw(Camera& camera) {
    shader_.Use();
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 0, -10));
    auto view = camera.GetViewMatrix();
    auto projection = glm::perspective(glm::radians(camera.zoom_),
                                       (float)1280 / (float)720, 0.1f, 100.0f);
    shader_.SetMat4("Model", model);
    shader_.SetMat4("View", view);
    shader_.SetMat4("Projection", projection);
    auto aa = glGetError();
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 11904, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};
