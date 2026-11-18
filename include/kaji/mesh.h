#include "kaji/shader.h"
#include <vector>
namespace kaji {
class Mesh {
public:
    explicit Mesh(const std::vector<float>& vertices);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator = (const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator = (Mesh&& other) noexcept;

    void draw() const;

private:
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLsizei vertex_count_ = 0;
};

}
