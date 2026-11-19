#include "kaji/gfx/shader.h"
#include <cstddef>
#include <span>
#include <vector>

namespace kaji::gfx {

enum class BufferUsage {
    Static,
    Dynamic,
};

class Mesh {
public:

    Mesh(std::span<const float> vertices, BufferUsage usage = BufferUsage::Static);
    explicit Mesh(const std::size_t max_vertices, BufferUsage usage = BufferUsage::Dynamic);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator = (const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator = (Mesh&& other) noexcept;

    void update_orphan(std::span<const float> vertices);
    void update_mapped(std::span<const float> vertices, bool orphan_whole_buffer = true);

    void draw() const;
    GLsizei vertex_count() const noexcept { return vertex_count_; }
    std::size_t capacity_floats() const noexcept { return capacity_bytes_ / sizeof(float); }

private:
    void init_buffers(GLsizeiptr byte_size, const void* initial_data, GLenum gl_usage);
    void release() noexcept;

    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLsizei vertex_count_ = 0;
    GLsizeiptr capacity_bytes_ = 0;
    GLenum gl_usage_ = GL_STATIC_DRAW;
};

}
