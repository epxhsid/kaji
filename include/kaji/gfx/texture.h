#pragma once

#include <GL/glew.h>


namespace kaji::gfx {

class Texture {
public:
    Texture(int width, int height, GLenum internal_format = GL_RGBA8, GLenum format = GL_RGBA);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void bind(unsigned int slot = 0) const;
    void unbind(unsigned int slot = 0) const;
    void resize(int width, int height);

    GLuint id() const noexcept { return id_; }
    int width() const noexcept { return width_; }
    int height() const noexcept { return height_; }

private:
    void release() noexcept;

    GLuint id_ = 0;
    int width_ = 0;
    int height_ = 0;
    GLenum internal_format_ = GL_RGBA8;
    GLenum format_ = GL_RGBA;
};

}
