#include "particle.h"

ParticleGenerator::ParticleGenerator(GLuint amount, GLfloat init_life,
                                     GLfloat init_velocity,
                                     glm::vec3 &planet_pos)
    : shader_("src/particle.vs.glsl", "src/particle.fs.glsl"),
      amount_(amount),
      init_life_(init_life),
      init_velocity_(init_velocity),
      planet_pos_(planet_pos) {
  // Set up mesh property
  GLuint vbo;
  // Particle vertices
  GLfloat particle_quad[] = {0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                             0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f};
  glGenVertexArrays(1, &this->vao_);
  glGenBuffers(1, &vbo);
  glBindVertexArray(this->vao_);
  // Load mesh buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad,
               GL_STATIC_DRAW);
  // Set mesh attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                        (GLvoid *)0);
  glBindVertexArray(0);

  // Initialize particles
  for (GLuint i = 0; i < this->amount_; ++i)
    this->particles_.push_back(Particle());
}

void ParticleGenerator::Update(GLfloat dt, Context &context,
                               GLuint new_particles, GLboolean respawn,
                               glm::vec3 offset) {
  // Create new particles if asked
  if (respawn) {
    for (GLuint i = 0; i < new_particles; ++i) {
      int unusedParticle = this->firstUnusedParticle();
      this->respawnParticle(this->particles_[unusedParticle], offset);
    }
  }
  // Update all particles' state
  for (GLuint i = 0; i < this->amount_; ++i) {
    Particle &p = this->particles_[i];
    // kill particles which have reached the center
    if (abs(p.position_.x) < abs(p.velocity_.x * dt) ||
        abs(p.position_.y) < abs(p.velocity_.y * dt))
      p.life_ = 0;
    else
      p.life_ -= dt;  // reduce life of particles
    // update particle if alive
    if (p.life_ > 0.0f) {
      p.position_ -= p.velocity_ * dt;
      p.color_.a -= dt * 5;
    }
  }
}

// Draw all particles
void ParticleGenerator::Draw(Context &context) {
  // calculate model matrix
  auto model = context.kClockPosition;
  model = glm::translate(model, glm::vec3(0, 0.3, 0.0));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

  auto view = context.get_camera().GetViewMatrix();
  auto projection = glm::perspective(glm::radians(context.get_camera().zoom_),
                                     context.Ratio(), 0.1f, 100.0f);

  shader_.Use();
  for (Particle particle : this->particles_) {
    // Draw particle if alive
    if (particle.life_ > 0.0f) {
      shader_.SetVec3("Offset", particle.position_);
      shader_.SetMat4("Projection", projection);
      shader_.SetMat4("Model", model);
      shader_.SetMat4("View", view);
      shader_.SetVec4("Color", particle.color_);
      glBindVertexArray(this->vao_);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }
  }
}

// Cache the last used particle for quicker search for dead particles.
GLuint last_used_particle = 0;
GLuint ParticleGenerator::firstUnusedParticle() {
  // Search from last used particle first.
  for (GLuint i = last_used_particle; i < this->amount_; ++i) {
    if (this->particles_[i].life_ <= 0.0f) {
      last_used_particle = i;
      return i;
    }
  }
  // If failed, perform linear search
  for (GLuint i = 0; i < last_used_particle; ++i) {
    if (this->particles_[i].life_ <= 0.0f) {
      last_used_particle = i;
      return i;
    }
  }
  // If all particles are alive, set to the first particle.
  last_used_particle = 0;
  return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, glm::vec3 offset) {
  // Randomly generate particle position and color.
  GLfloat random = 1 - abs(((rand() % 1000) - 500) / 500);
  GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
  particle.position_.x = planet_pos_.x * random + offset.x;
  particle.position_.y = planet_pos_.y * random + offset.y;
  random = ((rand() % 100) - 50) / 1500.0f;
  particle.position_.x -= random;
  random = ((rand() % 100) - 50) / 1500.0f;
  particle.position_.y -= random;
  random = ((rand() % 100) - 50) / 1500.0f;
  particle.position_.z = planet_pos_.z + random + offset.z;
  particle.color_ = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.life_ = init_life_;
  // set velocity pointing to the clock center
  particle.velocity_ = glm::normalize(particle.position_) * init_velocity_;
}

// Implement method for generating shadow
void ParticleGenerator::DrawDepthMap(Context &context) {
  // calculate model matrix
  auto model = context.kClockPosition;
  model = glm::translate(model, glm::vec3(0, 0.3, 0.0));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
  // set view to light position
  auto light_view = glm::lookAt(context.light_position_, glm::vec3(0.0f),
                                glm::vec3(0.0, 1.0, 0.0));
  auto light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);
  auto light_space = light_projection * light_view;

  glBindVertexArray(vao_);
  for (Particle particle : particles_) {
    // Draw depth buffer for every particle.
    if (particle.life_ > 0.0f) {
      context.get_depth_map_shader().Use();
      context.get_depth_map_shader().SetMat4("LightSpace", light_space);
      context.get_depth_map_shader().SetMat4("Model", model);
      context.get_depth_map_shader().SetFloat("PosFactor", 0.016f);
      context.get_depth_map_shader().SetBool("IsParticle", true);
      context.get_depth_map_shader().SetVec3("Offset", particle.position_);

      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  }

  glBindVertexArray(0);
  // restore mode for depth map shader.
  context.get_depth_map_shader().SetBool("IsParticle", false);
}