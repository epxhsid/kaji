#pragma once

#include <GL/glew.h>
#include <cstddef>
#include <span>

namespace kaji::gfx {

enum class BufferUsage {
    StaticDraw,
    DynamicDraw,
    StreamDraw
};

enum class BufferTarget {
    ArrayBuffer,
    ElementArrayBuffer
};

class Buffer {
public:

    Buffer(std::span<const std::byte> data, BufferUsage usage, BufferTarget target = BufferTarget::ArrayBuffer);
    Buffer(std::size_t capacity_bytes, BufferUsage usage, BufferTarget target = BufferTarget::ArrayBuffer);
    BufferTarget target() const noexcept;

    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;

    void bind() const;
    void unbind() const;

    void update_orphan(std::span<const std::byte> data);
    void update_mapped(std::span<const std::byte> data);

    GLuint id() const noexcept { return id_; }
    std::size_t capacity_bytes() const noexcept { return capacity_bytes_; }

private:
    void allocate(std::size_t bytes, const void* data);
    void release() noexcept;

    GLuint id_ = 0;
    std::size_t capacity_bytes_ = 0;
    GLenum gl_usage_ = GL_STATIC_DRAW;
    BufferTarget target_ = BufferTarget::ArrayBuffer;
    GLenum gl_target_ = GL_ARRAY_BUFFER;
};

}
