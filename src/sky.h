#pragma once

#include <array>
#include <glm/glm.hpp>
#include <string>

#include "context.hpp"
#include "shader.hpp"

/**
 * @brief Class for sky box.
 */
class SkyBox {
 private:
  // 6(v per face) * 6(faces) * 3(axis) = 108
  static const float kVertices[108];

  // Texture path for each face.
  static const std::array<std::string, 6> kTextures;

  unsigned int vao_;

  unsigned int vbo_;

  unsigned int texture_id_ = 0;

  Shader shader_;

 public:
  SkyBox();

  ~SkyBox();

  /**
   * @brief Load cube map texture from file.
   */
  void LoadCubeMap();

  /**
   * @brief Draw sky box.
   *
   * @param context Render context.
   */
  void Draw(Context& context);
};
