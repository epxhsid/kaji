#pragma once

#include "kaji/gfx/buffer.h"
#include <GL/glew.h>
#include <cstddef>
#include <memory>
#include <span>

namespace kaji::gfx {

struct VertexAttribute {
    GLuint index = 0;
    GLint components = 3;
    GLenum type = GL_FLOAT;
    GLboolean normalized = GL_FALSE;
    GLsizei stride = 0;
    std::size_t offset = 0;
};

class Mesh {
public:

    Mesh(std::shared_ptr<Buffer> vertex_buffer,
        std::span<const VertexAttribute> layout,
        GLsizei vertex_count,
        GLenum draw_mode = GL_TRIANGLES);

    Mesh(std::shared_ptr<Buffer> vertex_buffer,
        std::span<const VertexAttribute> layout,
        std::shared_ptr<Buffer> index_buffer,
        GLsizei index_count,
        GLenum index_type = GL_UNSIGNED_INT,
        GLenum draw_mode = GL_TRIANGLES);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void set_vertex_count(GLsizei count) noexcept { vertex_count_ = count; }
    GLsizei vertex_count() const noexcept { return vertex_count_; }

    void draw() const;

private:
    void configure_layout(std::span<const VertexAttribute> layout);
    void release() noexcept;

    std::shared_ptr<Buffer> buffer_;
    GLuint vao_ = 0;
    GLsizei vertex_count_ = 0;
    GLenum draw_mode_ = GL_TRIANGLES;
    std::shared_ptr<Buffer> index_buffer_;
    GLsizei index_count_ = 0;
    GLenum index_type_ = GL_UNSIGNED_INT;
};

}
