#pragma once

#include <glad/glad.h>

#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
 private:
  unsigned int shader_id_;

  void CheckCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "Error: Shader compile error:" << type << "\n"
                  << infoLog << std::endl;
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "Error: Shader program linking error:" << type << "\n"
                  << infoLog << std::endl;
      }
    }
  }

 public:
  Shader(const char *v_path, const char *f_path) {
    std::string v_code_string;
    std::string f_code_string;
    std::ifstream v_file;
    std::ifstream f_file;

    try {
      v_file.open(v_path);
      f_file.open(f_path);

      std::stringstream v_stream, f_stream;
      v_stream << v_file.rdbuf();
      f_stream << f_file.rdbuf();

      v_file.close();
      f_file.close();

      v_code_string = v_stream.str();
      f_code_string = f_stream.str();
    } catch (std::ifstream::failure e) {
      std::cerr << "Error: Failed to read shader program." << std::endl;
    }
    const char *v_code = v_code_string.c_str();
    const char *f_code = f_code_string.c_str();

    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v_code, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f_code, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");
    // shader Program
    shader_id_ = glCreateProgram();
    glAttachShader(shader_id_, vertex);
    glAttachShader(shader_id_, fragment);
    glLinkProgram(shader_id_);
    CheckCompileErrors(shader_id_, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  void Use() const { glUseProgram(shader_id_); }

  void SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(shader_id_, name.c_str()), (int)value);
  }

  void SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(shader_id_, name.c_str()), value);
  }

  void SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(shader_id_, name.c_str()), value);
  }

  void SetVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(shader_id_, name.c_str()), 1, &value[0]);
  }
  void SetVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(shader_id_, name.c_str()), x, y);
  }

  void SetVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(shader_id_, name.c_str()), 1, &value[0]);
  }
  void SetVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(shader_id_, name.c_str()), x, y, z);
  }

  void SetVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(shader_id_, name.c_str()), 1, &value[0]);
  }
  void SetVec4(const std::string &name, float x, float y, float z,
               float w) const {
    glUniform4f(glGetUniformLocation(shader_id_, name.c_str()), x, y, z, w);
  }

  void SetMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(shader_id_, name.c_str()), 1,
                       GL_FALSE, &mat[0][0]);
  }

  void SetMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(shader_id_, name.c_str()), 1,
                       GL_FALSE, &mat[0][0]);
  }

  void SetMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(shader_id_, name.c_str()), 1,
                       GL_FALSE, &mat[0][0]);
  }
};