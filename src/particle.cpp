#include "particle.h"

ParticleGenerator::ParticleGenerator(const char *t_path, GLuint amount)
    : shader_("src/particle.vs.glsl", "src/particle.fs.glsl"), amount_(amount) {
  this->init();
  loadTexture(t_path);
}

void ParticleGenerator::Update(GLfloat dt, Context &context,
                               GLuint newParticles, glm::vec3 offset) {
  // Add new particles
  for (GLuint i = 0; i < newParticles; ++i) {
    int unusedParticle = this->firstUnusedParticle();
    this->respawnParticle(this->particles[unusedParticle], context, offset);
  }
  // Update all particles
  for (GLuint i = 0; i < this->amount_; ++i) {
    dt *= ((rand() % 1000 - 500) / 1000.0 + 1);
    Particle &p = this->particles[i];
    p.Life -= dt;         // reduce life
    if (p.Life > 0.0f) {  // particle is alive, thus update
      p.Position -= p.Velocity * dt;
      p.Color.a -= dt * 2.5;
    }
  }
}

// Render all particles
void ParticleGenerator::Draw(Context & context) {
  auto model = context.kClockPosition;
  model = glm::translate(model, glm::vec3(0, 0.3, 0.0));
  auto view = context.camera_.GetViewMatrix();
  auto projection = glm::perspective(glm::radians(context.camera_.zoom_),
    context.Ratio(), 0.1f, 100.0f);
  // Use additive blending to give it a 'glow' effect
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  this->shader_.Use();
  for (Particle particle : this->particles) {
    if (particle.Life > 0.0f) {
      this->shader_.SetVec3("offset", particle.Position);
      this->shader_.SetVec4("color", particle.Color);
      this->shader_.SetMat4("projection", projection);
      shader_.SetMat4("model", model);
      shader_.SetMat4("view", view);
      glBindTexture(GL_TEXTURE_2D, texture_id_);
      glBindVertexArray(this->VAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }
  }
  // Don't forget to reset to default blending mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init() {
  // Set up mesh and attribute properties
  GLuint VBO;
  GLfloat particle_quad[] = {
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(this->VAO);
  // Fill mesh buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad,
               GL_STATIC_DRAW);
  // Set mesh attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (GLvoid *)0);
  glBindVertexArray(0);

  // Create this->amount default particle instances
  for (GLuint i = 0; i < this->amount_; ++i)
    this->particles.push_back(Particle());
}

void ParticleGenerator::loadTexture(const char *t_path) {
  // Create texture object.
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  // Load image from file.
  int width, height, channels;
  unsigned char *data = stbi_load(t_path, &width, &height, &channels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Error: Failed to load texture." << std::endl;
  }
  stbi_image_free(data);

  // Set parameters of texture object.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// Stores the index of the last particle used (for quick access to next dead
// particle)
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle() {
  // First search from last used particle, this will usually return almost
  // instantly
  for (GLuint i = lastUsedParticle; i < this->amount_; ++i) {
    if (this->particles[i].Life <= 0.0f) {
      lastUsedParticle = i;
      return i;
    }
  }
  // Otherwise, do a linear search
  for (GLuint i = 0; i < lastUsedParticle; ++i) {
    if (this->particles[i].Life <= 0.0f) {
      lastUsedParticle = i;
      return i;
    }
  }
  // All particles are taken, override the first one (note that if it repeatedly
  // hits this case, more particles should be reserved)
  lastUsedParticle = 0;
  return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, Context &context,
                                        glm::vec3 offset) {
  GLfloat random = ((rand() % 100) - 50) / 1000.0f;
  GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
  particle.Position.x = context.jupiter_pos_.x + random + offset.x;
  random = ((rand() % 100) - 50) / 1000.0f;
  particle.Position.y = context.jupiter_pos_.y + random + offset.y;
  random = ((rand() % 100) - 50) / 1000.0f;
  particle.Position.z = context.jupiter_pos_.z + random + offset.z;
  particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.Life = 1.0f;
  particle.Velocity = context.jupiter_velocity_ * 0.1f;
}