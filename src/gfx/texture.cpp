#include "kaji/gfx/texture.h"
#include <stdexcept>

namespace kaji::gfx {

Texture::Texture(int width, int height, GLenum internal_format, GLenum format)
  : width_(width), height_(height), internal_format_(internal_format), format_(format) {
    if (width_ <= 0 || height_ <= 0) {
        throw std::invalid_argument("texture dimensions must be positive");
    }

    if (id_ == 0) {
        throw std::runtime_error("failed to create texture");
    }

    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        static_cast<GLint>(internal_format_),
        width_,
        height_,
        0,
        format_,
        GL_UNSIGNED_BYTE,
        nullptr
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    release();
}

Texture::Texture(Texture&& other) noexcept
    : id_(other.id_),
      width_(other.width_),
      height_(other.height_),
      internal_format_(other.internal_format_),
      format_(other.format_) {
    other.id_ = 0;
    other.width_ = 0;
    other.height_ = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        release();

        id_ = other.id_;
        width_ = other.width_;
        height_ = other.height_;
        internal_format_ = other.internal_format_;
        format_ = other.format_;

        other.id_ = 0;
        other.width_ = 0;
        other.height_ = 0;
    }
    return *this;
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::unbind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::resize(int width, int height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Texture dimensions must be positive");
    }
    if (width == width_ && height == height_) {
        return;
    }

    width_ = width;
    height_ = height;

    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        static_cast<GLint>(internal_format_),
        width_,
        height_,
        0,
        format_,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::release() noexcept {
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }
    width_ = 0;
    height_ = 0;
}


}
