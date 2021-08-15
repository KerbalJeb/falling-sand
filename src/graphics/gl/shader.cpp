#include <graphics/gl/shader.hpp>

shader::shader(const std::filesystem::path &src_path, GLenum shaderType) {
  if (!std::filesystem::exists(src_path)) {
    std::cout << "File not found: " << src_path << std::endl;
    throw std::runtime_error{"Shader source not found"};
  }
  obj_ = glCreateShader(shaderType);
  if (!obj_) {
    throw std::runtime_error{"Unable to create shader"};
  }
  std::basic_ifstream<GLchar> f{src_path};
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

shader &shader::operator=(shader &&other) noexcept {
  if (this != &other) {
    release();
    std::swap(obj_, other.obj_);
  }
  return *this;
}

shader_program::shader_program(const std::string &vertexPath, const std::string &fragmentPath) {
  // Create Shaders
  shader vertexShader{vertexPath, GL_VERTEX_SHADER};
  shader fragmentShader{fragmentPath, GL_FRAGMENT_SHADER};

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

shader_program &shader_program::operator=(shader_program &&other) noexcept {
  if (this != &other) {
    release();
    std::swap(obj_, other.obj_);
  }
  return *this;
}
