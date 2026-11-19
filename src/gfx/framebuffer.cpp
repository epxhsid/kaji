#include "kaji/gfx/framebuffer.h"
#include <stdexcept>
#include <utility>

namespace kaji::gfx {

Framebuffer::Framebuffer(std::shared_ptr<Texture> color_attachment) : color_(std::move(color_attachment)) {
    if (!color_) {
        throw std::invalid_argument("Framebuffer requires a color Texture");
    }

    glGenFramebuffers(1, &fbo_id_);
    attach_color();
}

Framebuffer::~Framebuffer() {
    release();
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
    : fbo_id_(other.fbo_id_),
      color_(std::move(other.color_)) {
    other.fbo_id_ = 0;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
    if (this != &other) {
        release();

        fbo_id_ = other.fbo_id_;
        color_ = std::move(other.color_);
        other.fbo_id_ = 0;
    }
    return *this;
}

void Framebuffer::attach_color() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        color_->id(),
        0
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        release();
        throw std::runtime_error("Framebuffer is not complete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::release() noexcept {
    if (fbo_id_ != 0) {
        glDeleteFramebuffers(1, &fbo_id_);
        fbo_id_ = 0;
    }
    color_.reset();
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
    if (color_) {
        glViewport(0, 0, color_->width(), color_->height());
    }
}

void Framebuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::resize(int width, int height) {
    if (!color_) {
        return;
    }
    color_->resize(width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        color_->id(),
        0
    );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::blit_to_default(int dst_x0, int dst_y0, int dst_x1, int dst_y1, GLenum filter) const {
    if (!color_) {
        return;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_id_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(
        0, 0, color_->width(), color_->height(),
        dst_x0, dst_y0, dst_x1, dst_y1,
        GL_COLOR_BUFFER_BIT,
        filter
    );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}
