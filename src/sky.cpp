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
    "resources/textures/sky/px.jpg", "resources/textures/sky/nx.jpg",
    "resources/textures/sky/py.jpg", "resources/textures/sky/ny.jpg",
    "resources/textures/sky/pz.jpg", "resources/textures/sky/nz.jpg",
};

SkyBox::SkyBox() : shader_("src/sky.vs.glsl", "src/sky.fs.glsl") {
  // Create Vertex Buffer Object
  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  // Create Vertex Array Object.
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  // Load all vertices.
  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), &kVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

  glEnableVertexAttribArray(0);
}

SkyBox::~SkyBox() {
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
}

void SkyBox::LoadCubeMap() {
  // Generate texture of cube map format.
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);

  // Load texture from file for each face.
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

  // Set texture parameters.
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void SkyBox::Draw(Context& context) {
  // Make sure to pass depth test when depth buffer is equal 1.0.
  glDepthFunc(GL_LEQUAL);

  // Calculate view and projection.
  auto view = glm::mat4(glm::mat3(context.get_camera().GetViewMatrix()));
  auto projection = glm::perspective(glm::radians(context.get_camera().zoom_),
                                     context.Ratio(), 0.1f, 100.0f);

  // Activate shader and set uniform variables.
  shader_.Use();
  shader_.SetMat4("View", view);
  shader_.SetMat4("Projection", projection);

  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(0);
  glDepthFunc(GL_LESS);
}
