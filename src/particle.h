#pragma once
#include <glad/glad.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <vector>

#include "context.hpp"
#include "shader.hpp"

// class for particles, stores its properties.
class Particle {
 public:
  glm::vec3 position_, velocity_;
  glm::vec4 color_;
  GLfloat life_;
  Particle() : position_(0.0f), velocity_(0.0f), color_(1.0f), life_(0.0f) {}
};

// Class for generating and organizing particles.
class ParticleGenerator {
 public:
  ParticleGenerator(GLuint amount, GLfloat init_life, GLfloat init_velocity,
                    glm::vec3& planet_pos);
  // Update states of all particles
  void Update(GLfloat dt, Context& context, GLuint newParticles,
              GLboolean respawn,
              glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
  // Draw all particles
  void Draw(Context& context);
  void DrawDepthMap(Context& context);

 private:
  // Hold all particles
  std::vector<Particle> particles_;
  GLuint amount_;
  // Particle generation param
  GLfloat init_life_;
  GLfloat init_velocity_;
  // Render state
  Shader shader_;
  GLuint vao_;
  glm::vec3& planet_pos_;
  // Returns the index of first dead particle, 0 if all particles are alive.
  GLuint firstUnusedParticle();
  // Respawns a particle
  void respawnParticle(Particle& particle,
                       glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
};