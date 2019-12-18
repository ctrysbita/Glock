#pragma once
#include <glad/glad.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <vector>

#include "context.hpp"
#include "shader.hpp"

// Represents a single particle and its state
class Particle {
 public:
  glm::vec3 Position, Velocity;
  glm::vec4 Color;
  GLfloat Life;
  Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};

// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator {
 public:
  // Constructor
  ParticleGenerator(const char* t_path, GLuint amount, GLfloat init_life,
                    GLfloat init_velocity, glm::vec3& planet_pos);
  // Update all particles
  void Update(GLfloat dt, Context& context, GLuint newParticles,
              glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
  // Render all particles
  void Draw(Context& context);
  void DrawDepthMap(Context& context);

 private:
  // State
  std::vector<Particle> particles;
  GLuint amount_;
  // Particle param
  GLfloat init_life_;
  GLfloat init_velocity_;
  // Render state
  Shader shader_;
  unsigned int texture_id_ = 0;
  GLuint VAO;
  glm::vec3& planet_pos_;
  // Initializes buffer and vertex attributes
  void init();

  void loadTexture(const char* t_path);
  // Returns the first Particle index that's currently unused e.g. Life <= 0.0f
  // or 0 if no particle is currently inactive
  GLuint firstUnusedParticle();
  // Respawns particle
  void respawnParticle(Particle& particle,
                       glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
};