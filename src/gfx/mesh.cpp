#include "kaji/gfx/mesh.h"
#include <vector>

namespace kaji::gfx {

Mesh::Mesh(const std::vector<float>& vertices) {
    vertex_count_ = static_cast<GLsizei>(vertices.size() / 3);
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
