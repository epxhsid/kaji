#pragma once

#include "kaji/gfx/texture.h"
#include <GL/glew.h>
#include <memory>

namespace kaji::gfx {

class Framebuffer {
public:
    explicit Framebuffer(std::shared_ptr<Texture> color_attachment);
    ~Framebuffer();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    Framebuffer(Framebuffer&& other) noexcept;
    Framebuffer& operator=(Framebuffer&& other) noexcept;

    void bind() const;
    void unbind() const;
    void resize(int width, int height);

    void blit_to_default(int dst_x0, int dst_y0, int dst_x1, int dst_y1,
                         GLenum filter = GL_LINEAR) const;

    const std::shared_ptr<Texture>& color_texture() const noexcept { return color_; }
    GLuint id() const noexcept { return fbo_id_; }

private:
    void attach_color();
    void release() noexcept;

    GLuint fbo_id_ = 0;
    std::shared_ptr<Texture> color_;
};

}
