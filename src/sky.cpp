#define STB_IMAGE_IMPLEMENTATION

#include "sky.h"

#include <stb_image.h>

const float SkyBox::kVertices[108] = {
    -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

    1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,
};

const std::array<std::string, 6> SkyBox::kTextures = {
    "resources/textures/crossing/px.jpg", "resources/textures/crossing/nx.jpg",
    "resources/textures/crossing/py.jpg", "resources/textures/crossing/ny.jpg",
    "resources/textures/crossing/pz.jpg", "resources/textures/crossing/nz.jpg",
};

void SkyBox::LoadCubeMap() {
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);

  int width, height, channels;
  for (size_t i = 0; i < 6; i++) {
    unsigned char* data =
        stbi_load(kTextures[i].c_str(), &width, &height, &channels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cerr << "Error: Cube map texture failed to load at path: "
                << kTextures[i] << std::endl;
      stbi_image_free(data);
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void SkyBox::Draw(Context& context) {
  glDepthFunc(GL_LEQUAL);

  shader_.Use();
  auto view = glm::mat4(glm::mat3(context.get_camera().GetViewMatrix()));
  auto projection = glm::perspective(glm::radians(context.get_camera().zoom_),
                                     context.Ratio(), 0.1f, 100.0f);
  shader_.SetMat4("View", view);
  shader_.SetMat4("Projection", projection);

  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glBindVertexArray(vao_);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(0);
  glDepthFunc(GL_LESS);
}
