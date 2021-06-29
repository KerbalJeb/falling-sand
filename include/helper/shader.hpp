#ifndef CPP_FALLING_SAND_SHADER_HPP
#define CPP_FALLING_SAND_SHADER_HPP
#include <string>
#include <fstream>
#include <iostream>

#include <GL/glew.h>

namespace ra::graphics{
    class shader{
    public:
        class path{};
        shader(const path& , const std::string &path, GLenum shaderType){
            shader_id = glCreateShader(shaderType);
            if (!shader_id) {
                throw std::runtime_error{"Unable to create shader"};
            }

            std::basic_ifstream<GLchar> f{path};
            std::basic_string<GLchar> shaderSource{
                    (std::istreambuf_iterator<GLchar>{f}),
                    (std::istreambuf_iterator<GLchar>{})
            };
            const GLchar *sourceStart = &shaderSource[0];
            auto len = static_cast<GLint>(shaderSource.size());

            glShaderSource(shader_id, 1, &sourceStart, &len);
            glCompileShader(shader_id);

            GLint status = GL_FALSE;
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
            if (status != GL_TRUE) {
                char msg_buffer[512];
                glGetShaderInfoLog(shader_id, 512, nullptr, msg_buffer);
                glDeleteShader(shader_id);
                std::cout << "Error compiling shader:" << std::endl << msg_buffer << std::endl;
                throw std::runtime_error{"Unable to compile shader"};
            }
        }

        ~shader(){
            glDeleteShader(shader_id);
        }

        void attach(GLuint program){
            glAttachShader(program, shader_id);
        };

    private:
        GLuint shader_id;
    };

    class shader_program{
    public:
        shader_program(const std::string& vertexPath, const std::string& fragmentPath){
            // Create Shaders
            shader vertexShader{shader::path{}, vertexPath, GL_VERTEX_SHADER};
            shader fragmentShader{shader::path{}, fragmentPath, GL_FRAGMENT_SHADER};

            program_id = glCreateProgram();
            if (!program_id){
                throw std::runtime_error{"Unable to create shader program"};
            }

            vertexShader.attach(program_id);
            fragmentShader.attach(program_id);

            glLinkProgram(program_id);
            GLint status = GL_FALSE;
            glGetProgramiv(program_id, GL_LINK_STATUS, &status);

            if (status != GL_TRUE) {
                char infoLog[512];
                glGetProgramInfoLog(program_id, 512, nullptr, infoLog);
                glDeleteProgram(program_id);
                std::cout << "Link failed" << std::endl << infoLog << std::endl;
                throw std::runtime_error{"Unable to link shader program"};
            }
        }

        ~shader_program(){
            glDeleteProgram(program_id);
        }

        void use()
        {
            glUseProgram(program_id);
        }

        [[nodiscard]] GLint get_attrib_location(const std::string& name) const{
            return glGetAttribLocation(program_id, name.c_str());
        }

        [[nodiscard]] GLint get_uniform_location(const std::string& name) const{
            return glGetUniformLocation(program_id, name.c_str());
        }

        void set_uniform4f(const std::string& name, float r, float g, float b, float a) {
            auto pos = get_uniform_location(name);
            glUniform4f(pos, r, g, b, a);
        }

    private:
        GLuint program_id;
    };
}

#endif //CPP_FALLING_SAND_SHADER_HPP
