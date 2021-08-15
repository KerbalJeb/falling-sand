#ifndef CPP_FALLING_SAND_SHADER_HPP
#define CPP_FALLING_SAND_SHADER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// A wrapper class for opengl shader objects
class shader {
public:
  // Constructs a shader type shaderType from the source file
  // src_path must be the path of a valid shader file
  // A runtime exception will be thrown if the src_path does not exists
  // or the shader fails to complies
  // In the event of a compilation error,
  // the full opengl error message will be printed to the console
  shader(const std::filesystem::path &src_path, GLenum shaderType);

  // Shaders live on the GPU so the cannot (easily) be copied
  shader(const shader &) = delete;

  shader &operator=(const shader &) = delete;

  // Shaders can be moved by swapping the stored object ids
  shader(shader &&other) noexcept: obj_(other.obj_) {
    other.obj_ = 0;
  }

  shader &operator=(shader &&other) noexcept;

  // Delete the shader on destruction
  ~shader() { release(); }

  // Calls glAttachShader for the given program
  void attach(GLuint program) const { glAttachShader(program, obj_); };

private:
  GLuint obj_{0};

  void release() {
    glDeleteShader(obj_);
    obj_ = 0;
  }
};

// A wrapper class for opengl shader programs
class shader_program {
public:
  // Generates a shader program from a vertex and fragment shader
  // both paths must be the paths of valid shader files
  // A runtime exception will be thrown if either of the files does not exists
  // or the shaders fail to compile
  // In the event of a compilation error,
  // the full opengl error message will be printed to the console
  shader_program(const std::string &vertexPath,
                 const std::string &fragmentPath);

  // Shader objects cannot be copied
  shader_program(const shader_program &) = delete;

  shader_program &operator=(const shader_program &) = delete;

  // Shader objects can be moved by swapping object ids
  shader_program(shader_program &&other) noexcept: obj_(other.obj_) { other.obj_ = 0; }

  shader_program &operator=(shader_program &&other) noexcept;

  // Destroy the opengl shader object on destruction
  ~shader_program() { release(); }

  // Call glUseProgram for the current program
  void bind() const {
    glUseProgram(obj_);
  }

  // Return the value of get_attrib_location for the program
  [[nodiscard]] GLint get_attrib_location(const std::string &name) const {
    return glGetAttribLocation(obj_, name.c_str());
  }

  // Return the value of get_uniform_location for the program
  [[nodiscard]] GLint get_uniform_location(const std::string &name) const {
    return glGetUniformLocation(obj_, name.c_str());
  }

  // Calls set_uniform3f with the program
  void set_uniform3f(const std::string &uniform_name, const glm::vec3 &vec3) {
    auto pos = get_uniform_location(uniform_name);
    glUniform3f(pos, vec3.x, vec3.y, vec3.z);
  }

  // Calls set_uniform_mat4 with the program
  void set_uniform_mat4(const std::string &uniform_name, const glm::mat4 &mat) {
    auto pos = get_uniform_location(uniform_name);
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
