#include <GL/gl.h>
#include <cstddef>
#include <span>

namespace kaji::gfx {

enum class BufferUsage {
    StaticDraw,
    DynamicDraw,
    StreamDraw
};

class Buffer {
public:
    Buffer(GLenum target, GLsizeiptr size, BufferUsage usage);
    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&&) noexcept;
    Buffer& operator=(Buffer&&) noexcept;

    void upload(GLintptr offset, std::span<const std::byte> data);
    void orphan(GLsizeiptr new_size);
    void* map(GLintptr offset, GLsizeiptr size, GLbitfield flags);
    void unmap();

    GLuint id() const noexcept;
    GLsizeiptr capacity() const noexcept;

private:
    GLuint id_ = 0;
    GLenum target_ = GL_ARRAY_BUFFER;
    GLsizeiptr capacity_ = 0;
};
}
