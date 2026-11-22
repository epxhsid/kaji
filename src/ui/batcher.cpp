#include "kaji/ui/batcher.h"

#include <algorithm>
#include <cstddef>
#include <span>
#include <stdexcept>

namespace kaji::ui {

UIBatcher::UIBatcher(std::shared_ptr<gfx::Shader> shader, std::size_t initial_capacity) : shader_(std::move(shader)), layout_{ {
    gfx::VertexAttribute{
        .index = 0,
        .components = 3,
        .type = GL_FLOAT,
        .normalized = GL_FALSE,
        .stride = static_cast<GLsizei>(sizeof(Vertex)),
        .offset = 0
    },
    gfx::VertexAttribute{
        .index = 1,
        .components = 4,
        .type = GL_FLOAT,
        .normalized = GL_FALSE,
        .stride = static_cast<GLsizei>(sizeof(Vertex)),
        .offset = offsetof(Vertex, r)
    }
    } } {
    if (!shader_) {
        throw std::invalid_argument("UI batcher requires a shader");
    }

    const std::size_t vertex_capacity = std::max<std::size_t>(6, initial_capacity);
    vertices_.reserve(vertex_capacity);
    vertex_buffer_ = std::make_shared<gfx::Buffer>(
        vertex_capacity * sizeof(Vertex),
        gfx::BufferUsage::DynamicDraw,
        gfx::BufferTarget::ArrayBuffer
    );

    mesh_ = std::make_unique<gfx::Mesh>(
        vertex_buffer_,
        std::span<const gfx::VertexAttribute>(layout_),
        6,
        GL_TRIANGLES
    );
}

void UIBatcher::begin() noexcept {
    vertices_.clear();
}

void UIBatcher::quad(const gfx::Rect& rect, const Color& color) {
    if (rect.empty()) {
        return;
    }

    const float x0 = static_cast<float>(rect.x);
    const float y0 = static_cast<float>(rect.y);
    const float x1 = static_cast<float>(rect.right());
    const float y1 = static_cast<float>(rect.bottom());

    vertices_.insert(vertices_.end(), {
        Vertex{x0, y0, 0.0f, color.r, color.g, color.b, color.a},
        Vertex{x1, y0, 0.0f, color.r, color.g, color.b, color.a},
        Vertex{x1, y1, 0.0f, color.r, color.g, color.b, color.a},
        Vertex{x0, y0, 0.0f, color.r, color.g, color.b, color.a},
        Vertex{x1, y1, 0.0f, color.r, color.g, color.b, color.a},
        Vertex{x0, y1, 0.0f, color.r, color.g, color.b, color.a}
    });
}

void UIBatcher::flush(const math::Mat4& projection) {
    if (vertices_.empty()) {
        mesh_->set_vertex_count(0);
        return;
    }

    vertex_buffer_->update_orphan(std::as_bytes(std::span<const Vertex>(vertices_.data(), vertices_.size())));
    mesh_->set_vertex_count(static_cast<GLsizei>(vertices_.size()));

    shader_->bind();
    shader_->set_mat4_col_major("uProjection", projection.data);
    shader_->set_mat4_col_major("uModel", math::Mat4::identity().data);
    mesh_->draw();
    shader_->unbind();
}

}
