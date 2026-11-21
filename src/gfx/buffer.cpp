#include "kaji/gfx/buffer.h"
#include <cstddef>
#include <cstring>
#include <stdexcept>

namespace kaji::gfx {
namespace {
GLenum to_gl_usage(BufferUsage usage) noexcept {
    switch (usage) {
        case BufferUsage::StaticDraw: return GL_STATIC_DRAW;
        case BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
        case BufferUsage::StreamDraw: return GL_STREAM_DRAW;
    }

    return GL_STATIC_DRAW;
}

GLenum to_gl_target(BufferTarget target) noexcept {
    switch (target) {
        case BufferTarget::ArrayBuffer:
            return GL_ARRAY_BUFFER;
        case BufferTarget::ElementArrayBuffer:
            return GL_ELEMENT_ARRAY_BUFFER;
    }

    return GL_ARRAY_BUFFER;
}

}

Buffer::Buffer(std::span<const std::byte> data, BufferUsage usage, BufferTarget target)
    : gl_usage_(to_gl_usage(usage)), target_(target), gl_target_(to_gl_target(target)) {
    allocate(data.size(), data.data());
}

Buffer::Buffer(std::size_t capacity_bytes, BufferUsage usage, BufferTarget target)
    : gl_usage_(to_gl_usage(usage)), target_(target), gl_target_(to_gl_target(target)) {
    allocate(capacity_bytes, nullptr);
}

BufferTarget Buffer::target() const noexcept {
    return target_;
}

Buffer::~Buffer() {
    release();
}


Buffer::Buffer(Buffer&& other) noexcept : id_(other.id_), capacity_bytes_(other.capacity_bytes_),
    gl_usage_(other.gl_usage_), target_(other.target_), gl_target_(other.gl_target_) {

    other.id_ = 0;
    other.capacity_bytes_ = 0;
    other.target_ = BufferTarget::ArrayBuffer;
    other.gl_target_ = GL_ARRAY_BUFFER;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
    if (this != &other) {
        release();

        id_ = other.id_;
        capacity_bytes_ = other.capacity_bytes_;
        gl_usage_ = other.gl_usage_;
        target_ = other.target_;
        gl_target_ = other.gl_target_;

        other.id_ = 0;
        other.capacity_bytes_ = 0;
        other.target_ = BufferTarget::ArrayBuffer;
        other.gl_target_ = GL_ARRAY_BUFFER;
    }
    return *this;
}

void Buffer::bind() const {
    glBindBuffer(gl_target_, id_);
}

void Buffer::unbind() const {
    glBindBuffer(gl_target_, 0);
}

void Buffer::update_orphan(std::span<const std::byte> data) {
    const std::size_t data_bytes = data.size();
    const std::size_t alloc_bytes = (data_bytes > capacity_bytes_) ? data_bytes : capacity_bytes_;

    glBindBuffer(gl_target_, id_);
    if (alloc_bytes > 0) {
        glBufferData(gl_target_, static_cast<GLsizeiptr>(alloc_bytes), nullptr, gl_usage_);
        capacity_bytes_ = alloc_bytes;
    }

    if (data_bytes > 0 && data.data() != nullptr) {
        glBufferSubData(gl_target_, 0, static_cast<GLsizeiptr>(data_bytes), data.data());
    }

    glBindBuffer(gl_target_, 0);
}

void Buffer::update_mapped(std::span<const std::byte> data) {
    const std::size_t data_bytes = data.size();

    if (data_bytes > capacity_bytes_) {
        update_orphan(data);
        return;
    }

    if (data_bytes == 0 || data.data() == nullptr) {
        return;
    }

    const GLbitfield access =
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

    glBindBuffer(gl_target_, id_);

    void* ptr = glMapBufferRange(
        gl_target_,
        0,
        static_cast<GLsizeiptr>(data_bytes),
        access
    );

    if (ptr == nullptr) {
        glBindBuffer(gl_target_, 0);
        throw std::runtime_error("glMapBufferRange failed");
    }

    std::memcpy(ptr, data.data(), data_bytes);

    if (glUnmapBuffer(gl_target_) == GL_FALSE) {
        glBindBuffer(gl_target_, 0);
        throw std::runtime_error(
            "buffer data corrupted during unmap"
        );
    }

    glBindBuffer(gl_target_, 0);
}

void Buffer::allocate(std::size_t bytes, const void* data) {
    glGenBuffers(1, &id_);
    glBindBuffer(gl_target_, id_);

    if (bytes > 0) {
        glBufferData(
            gl_target_,
            static_cast<GLsizeiptr>(bytes),
            data,
            gl_usage_
        );
        capacity_bytes_ = bytes;
    }

    glBindBuffer(gl_target_, 0);
}

void Buffer::release() noexcept {
    if (id_ != 0) {
        glDeleteBuffers(1, &id_);
        id_ = 0;
    }

    capacity_bytes_ = 0;
}

}
