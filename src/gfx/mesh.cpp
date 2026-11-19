#include "kaji/gfx/mesh.h"

namespace kaji::gfx {

namespace {
GLenum to_gl_usage(BufferUsage usage) noexcept {
    switch (usage) {
        case BufferUsage::Dynamic:
            return GL_DYNAMIC_DRAW;
        case BufferUsage::Static:
        default:
            return GL_STATIC_DRAW;
    }
}

}

Mesh::Mesh(std::span<const float> vertices, BufferUsage usage) {
    gl_usage_ = to_gl_usage(usage);
    const auto byte_size = static_cast<GLsizeiptr>(vertices.size_bytes());
    init_buffers(byte_size, vertices.data(), gl_usage_);
    vertex_count_ = static_cast<GLsizei>(vertices.size() / 3);
}

Mesh::~Mesh() {
    if (vao_ != 0) glDeleteVertexArrays(1, &vao_);
    if (vbo_ != 0) glDeleteBuffers(1, &vbo_);
}

Mesh::Mesh(Mesh&& other) noexcept : vao_(other.vao_), vbo_(other.vbo_), vertex_count_(other.vertex_count_) {
    other.vao_ = 0;
    other.vbo_ = 0;
    other.vertex_count_ = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        if (vao_ != 0) glDeleteVertexArrays(1, &vao_);
        if (vbo_ != 0) glDeleteBuffers(1, &vbo_);

        vao_ = other.vao_;
        vbo_ = other.vbo_;
        vertex_count_ = other.vertex_count_;

        other.vao_ = 0;
        other.vbo_ = 0;
        other.vertex_count_ = 0;
    }
    return *this;
}

void Mesh::draw() const {
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
    glBindVertexArray(0);
}

}
