#include "kaji/gfx/texture.h"
#include <stdexcept>


Texture::Texture(int width, int height, GLenum internal_format, GLenum format)
    : width_(width), height_(height), internal_format_(internal_format), format_(format) {
        if (width_ <= 0 || height_ <= 0) {
            throw std::invalid_argument("texture dimensions must be positive");
        }

        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);
    }
