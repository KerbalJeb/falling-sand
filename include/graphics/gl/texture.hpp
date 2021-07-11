//
// Created by ben on 2021-07-10.
//

#ifndef CPP_FALLING_SAND_TEXTURE_HPP
#define CPP_FALLING_SAND_TEXTURE_HPP
#include <cstddef>
#include <GL/glew.h>

class texture{
public:
    texture(std::size_t width, std::size_t height);
    texture(std::size_t width, std::size_t height, char* data);

    texture(const texture&) = delete;
    texture& operator=(const texture&) = delete;

    texture(texture&& other);
    texture& operator=(texture&& other);

    ~texture();

    void update(char* new_data);
    void bind();

private:
    std::size_t width_;
    std::size_t height_;
    GLuint obj_{0};
};

#endif //CPP_FALLING_SAND_TEXTURE_HPP
