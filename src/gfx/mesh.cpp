#include "kaji/gfx/mesh.h"
#include <memory>
#include <span>

namespace kaji::gfx {
Mesh::Mesh(std::shared_ptr<Buffer> vertex_buffer, std::span<const VertexAttribute> layout, GLsizei vertex_count, GLenum draw_mode)
    : buffer_(std::move(vertex_buffer)), vertex_count_(vertex_count), draw_mode_(draw_mode) {
    if (!buffer_) {
        throw std::invalid_argument("mesh requires a valid vertex/non-null buffer");
    }

    if (vertex_count_ <= 0) {
        throw std::invalid_argument("mesh requires a valid vertex count");
    }

    if (layout.empty()) {
        throw std::invalid_argument("mesh requires a valid vertex layout");
    }

    glGenVertexArrays(1, &vao_);

    configure_layout(layout);
}

Mesh::Mesh(std::shared_ptr<Buffer> vertex_buffer, std::span<const VertexAttribute> layout, std::shared_ptr<Buffer> index_buffer, GLsizei index_count, GLenum index_type, GLenum draw_mode)
    : buffer_(std::move(vertex_buffer)), index_buffer_(std::move(index_buffer)), vertex_count_(index_count), index_type_(index_type), draw_mode_(draw_mode) {
    if (!buffer_) {
        throw std::invalid_argument("mesh requires a valid vertex buffer");
    }

    if (buffer_->target() != BufferTarget::ArrayBuffer) {
        throw std::invalid_argument(
            "mesh vertex buffer must use BufferTarget::ArrayBuffer"
        );
    }

    if (!index_buffer_) {
        throw std::invalid_argument("mesh requires a valid index buffer");
    }

    if (index_buffer_->target() != BufferTarget::ElementArrayBuffer) {
        throw std::invalid_argument(
            "mesh index buffer must use BufferTarget::ElementArrayBuffer"
        );
    }

    if (index_count_ <= 0) {
        throw std::invalid_argument("indexed mesh requires a valid index count");
    }

    if (layout.empty()) {
        throw std::invalid_argument("mesh requires a valid vertex layout");
    }

    glGenVertexArrays(1, &vao_);
    configure_layout(layout);
}

Mesh::~Mesh() {
    release();
}

Mesh::Mesh(Mesh&& other) noexcept
    : buffer_(std::move(other.buffer_)),
      index_buffer_(std::move(other.index_buffer_)),
      vao_(other.vao_),
      vertex_count_(other.vertex_count_),
      index_count_(other.index_count_),
      index_type_(other.index_type_),
      draw_mode_(other.draw_mode_) {

    other.vao_ = 0;
    other.vertex_count_ = 0;
    other.index_count_ = 0;
    other.index_type_ = GL_UNSIGNED_INT;
    other.draw_mode_ = GL_TRIANGLES;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        release();

        buffer_ = std::move(other.buffer_);
        vao_ = other.vao_;
        vertex_count_ = other.vertex_count_;
        draw_mode_ = other.draw_mode_;

        other.vao_ = 0;
        other.vertex_count_ = 0;
        other.draw_mode_ = GL_TRIANGLES;
    }
    return *this;
}

void Mesh::draw() const {
    if (vertex_count_ <= 0 || vao_ == 0) {
        return;
    }

    glBindVertexArray(vao_);
    glDrawArrays(draw_mode_, 0, vertex_count_);
    glBindVertexArray(0);
}

void Mesh::configure_layout(std::span<const VertexAttribute> layout) {
    glBindVertexArray(vao_);
    buffer_->bind();

    for (const auto& attr : layout) {
        const auto* offset_ptr = reinterpret_cast<const void*>(attr.offset);
        glVertexAttribPointer(
            attr.index,
            attr.components,
            attr.type,
            attr.normalized,
            attr.stride,
            offset_ptr
        );
        glEnableVertexAttribArray(attr.index);
    }

    buffer_->unbind();
    glBindVertexArray(0);
}

void Mesh::release() noexcept {
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }
    vertex_count_ = 0;
    buffer_.reset();
}

}
