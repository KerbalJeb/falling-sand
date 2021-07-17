#ifndef CPP_FALLING_SAND_SHADER_HPP
#define CPP_FALLING_SAND_SHADER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class shader {
public:
  class path {
  };

  shader(const path &, const std::string &path, GLenum shaderType) {
    obj_ = glCreateShader(shaderType);
    if (!obj_) {
      throw std::runtime_error{"Unable to create shader"};
    }

    std::basic_ifstream<GLchar> f{path};
    std::basic_string<GLchar> shaderSource{
        (std::istreambuf_iterator<GLchar>{f}),
        (std::istreambuf_iterator<GLchar>{})
    };
    const GLchar *sourceStart = &shaderSource[0];
    auto len = static_cast<GLint>(shaderSource.size());

    glShaderSource(obj_, 1, &sourceStart, &len);
    glCompileShader(obj_);

    GLint status = GL_FALSE;
    glGetShaderiv(obj_, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
      char msg_buffer[512];
      glGetShaderInfoLog(obj_, 512, nullptr, msg_buffer);
      glDeleteShader(obj_);
      std::cout << "Error compiling shader:" << std::endl << msg_buffer
                << std::endl;
      throw std::runtime_error{"Unable to compile shader"};
    }
  }

  shader(const shader &) = delete;

  shader &operator=(const shader &) = delete;

  shader(shader &&other) noexcept: obj_(other.obj_) {
    other.obj_ = 0;
  }

  shader &operator=(shader &&other) noexcept {
    if (this != &other) {
      release();
      std::swap(obj_, other.obj_);
    }
    return *this;
  }

  ~shader() {
    release();
  }

  void attach(GLuint program) const {
    glAttachShader(program, obj_);
  };

private:
  GLuint obj_{0};

  void release() {
    glDeleteShader(obj_);
    obj_ = 0;
  }
};

class shader_program {
public:
  shader_program(const std::string &vertexPath,
                 const std::string &fragmentPath) {
    // Create Shaders
    shader vertexShader{shader::path{}, vertexPath, GL_VERTEX_SHADER};
    shader fragmentShader{shader::path{}, fragmentPath, GL_FRAGMENT_SHADER};

    obj_ = glCreateProgram();
    if (!obj_) {
      throw std::runtime_error{"Unable to create shader program"};
    }

    vertexShader.attach(obj_);
    fragmentShader.attach(obj_);

    glLinkProgram(obj_);
    GLint status = GL_FALSE;
    glGetProgramiv(obj_, GL_LINK_STATUS, &status);

    if (status != GL_TRUE) {
      char infoLog[512];
      glGetProgramInfoLog(obj_, 512, nullptr, infoLog);
      glDeleteProgram(obj_);
      std::cout << "Link failed" << std::endl << infoLog << std::endl;
      throw std::runtime_error{"Unable to link shader program"};
    }
  }

  shader_program(const shader_program &) = delete;

  shader_program &operator=(const shader_program &) = delete;

  shader_program(shader_program &&other) noexcept: obj_(other.obj_) {
    other.obj_ = 0;
  }

  shader_program &operator=(shader_program &&other) noexcept {
    if (this != &other) {
      release();
      std::swap(obj_, other.obj_);
    }
    return *this;
  }

  ~shader_program() {
    release();
  }

  void bind() const {
    glUseProgram(obj_);
  }

  [[nodiscard]] GLint get_attrib_location(const std::string &name) const {
    return glGetAttribLocation(obj_, name.c_str());
  }

  [[nodiscard]] GLint get_uniform_location(const std::string &name) const {
    return glGetUniformLocation(obj_, name.c_str());
  }

  void set_uniform3f(const std::string &name, const glm::vec3 &vec3) {
    auto pos = get_uniform_location(name);
    glUniform3f(pos, vec3.x, vec3.y, vec3.z);
  }

  void set_uniform4f(const std::string &name, const glm::vec4 &vec4) {
    auto pos = get_uniform_location(name);
    glUniform4f(pos, vec4.x, vec4.y, vec4.z, vec4.w);
  }

  void set_uniform4f(const std::string &name, float r, float g, float b,
                     float a) const {
    auto pos = get_uniform_location(name);
    glUniform4f(pos, r, g, b, a);
  }

  void set_uniform_mat4(const std::string &name, const glm::mat4 &mat) {
    auto pos = get_uniform_location(name);
    glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(mat));
  }

private:
  GLuint obj_{0};

  void release() {
    glDeleteProgram(obj_);
    obj_ = 0;
  }
};


#endif //CPP_FALLING_SAND_SHADER_HPP
