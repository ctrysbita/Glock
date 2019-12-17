#pragma once

#include <stb_image.h>

#include "context.hpp"
#include "shader.hpp"

/**
 * @brief base class for models.
 *
 */
class ModelComponent {
 protected:
  unsigned int vao_;

  unsigned int vbo_;

  unsigned int ebo_;

  unsigned int texture_id_ = 0;

  Shader shader_;

  /**
   * @brief load texture to `texture_id_`
   *
   * @param t_path Path of texture.
   */
  void LoadTexture(const char* t_path) {
    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    int width, height, channels;
    unsigned char* data = stbi_load(t_path, &width, &height, &channels, 0);
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

 public:
  /**
   * @brief Construct a new Model Component object.
   *
   * @param v_path Vertex shader path.
   * @param f_path Fragment shader path.
   * @param vertices_size Number of vertices.
   * @param vertices_data Vertex data of model.
   * @param indices_size Number of indices.
   * @param indices_data Index data of model.
   * @param t_path Path of texture.
   * @param has_uv whether the model has UV map.
   */
  ModelComponent(std::string&& v_path, std::string&& f_path,
                 GLsizeiptr vertices_size, const void* vertices_data,
                 GLsizeiptr indices_size, const void* indices_data,
                 const char* t_path = nullptr, bool has_uv = true)
      : shader_(v_path.c_str(), f_path.c_str()) {
    // Create buffer for model vertices
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    // Create storage for vertex data format
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    // load model vertices
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices_data, GL_STATIC_DRAW);
    // Create buffer for model incidies
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    // load model incidies
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices_data,
                 GL_STATIC_DRAW);
    // Set vertex data format
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          (has_uv ? 8 : 6) * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          (has_uv ? 8 : 6) * sizeof(float),
                          (void*)(3 * sizeof(float)));
    if (has_uv)
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                            (void*)(6 * sizeof(float)));
    // Enable vertex array attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    if (has_uv) glEnableVertexAttribArray(2);
    // Load model texture
    if (t_path) LoadTexture(t_path);
  }

  virtual ~ModelComponent() {}

  /**
   * @brief Get the shader object
   *
   * @return Shader& reference of the shader object.
   */
  Shader& get_shader() { return shader_; }
  /**
   * @brief draw the object.
   *
   * @param camera camera information for drawing.
   */
  virtual void Draw(Context& camera) = 0;
};
