#include "kaji/gfx/mesh.h"
#include <cstring>
#include <stdexcept>


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

Mesh::Mesh(std::size_t max_vertices, BufferUsage usage) {
    gl_usage_ = to_gl_usage(usage);
    const auto byte_size = static_cast<GLsizeiptr>(max_vertices * 3 * sizeof(float));
    init_buffers(byte_size, nullptr, gl_usage_);
    vertex_count_ = 0;
}

Mesh::~Mesh() {
    release();
}

Mesh::Mesh(Mesh&& other) noexcept
    : vao_(other.vao_), vbo_(other.vbo_),
      vertex_count_(other.vertex_count_),
      capacity_bytes_(other.capacity_bytes_),
      gl_usage_(other.gl_usage_) {

    other.vao_ = 0;
    other.vbo_ = 0;
    other.vertex_count_ = 0;
    other.capacity_bytes_ = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        release();

        vao_ = other.vao_;
        vbo_ = other.vbo_;
        vertex_count_ = other.vertex_count_;
        capacity_bytes_ = other.capacity_bytes_;
        gl_usage_ = other.gl_usage_;

        other.vao_ = 0;
        other.vbo_ = 0;
        other.vertex_count_ = 0;
        other.capacity_bytes_ = 0;
    }
    return *this;
}

void Mesh::update_orphan(std::span<const float> vertices) {
    const auto byte_size = static_cast<GLsizeiptr>(vertices.size_bytes());

    // resizes if needed
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, byte_size, nullptr, gl_usage_);
    capacity_bytes_ = byte_size;

    if (byte_size > 0 && vertices.data() != nullptr) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, byte_size, vertices.data());
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    vertex_count_ = static_cast<GLsizei>(vertices.size() / 3);
}

void Mesh::update_mapped(std::span<const float> vertices, bool orphan_whole_buffer) {
    const auto byte_size = static_cast<GLsizeiptr>(vertices.size_bytes());

    if (byte_size > capacity_bytes_) {
        update_orphan(vertices);
        return;
    }

    if (byte_size == 0) {
        vertex_count_ = 0;
        return;
    }

    GLbitfield access = GL_MAP_WRITE_BIT;
    if (orphan_whole_buffer) {
        access |= GL_MAP_INVALIDATE_BUFFER_BIT;
    } else {
        access |= GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, byte_size, access);
    if (ptr == nullptr) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        throw std::runtime_error("glMapBufferRange failed");
    }

    std::memcpy(ptr, vertices.data(), static_cast<std::size_t>(byte_size));

    if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        throw std::runtime_error("buffer data corrupted during unmap");
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    vertex_count_ = static_cast<GLsizei>(vertices.size() / 3);
}

void Mesh::init_buffers(GLsizeiptr byte_size, const void* initial_data, GLenum gl_usage) {
    capacity_bytes_ = byte_size;

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, byte_size, initial_data, gl_usage);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::release() noexcept {
    if (vbo_ != 0) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }
    vertex_count_ = 0;
    capacity_bytes_ = 0;
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
