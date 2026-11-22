#include "kaji/ui/batcher.h"

#include <algorithm>
#include <cstddef>
#include <span>
#include <stdexcept>

namespace kaji::ui {

UIBatcher::UIBatcher(
    std::shared_ptr<gfx::Shader> shader,
    std::shared_ptr<gfx::Shader> text_shader,
    std::shared_ptr<gfx::Font> font,
    std::size_t initial_capacity
) : shader_(std::move(shader)),
    text_shader_(std::move(text_shader)),
    font_(std::move(font)),
    layout_{{
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
    }},
    text_layout_{{
        gfx::VertexAttribute{
            .index = 0,
            .components = 3,
            .type = GL_FLOAT,
            .normalized = GL_FALSE,
            .stride = static_cast<GLsizei>(sizeof(TextVertex)),
            .offset = 0
        },
        gfx::VertexAttribute{
            .index = 1,
            .components = 2,
            .type = GL_FLOAT,
            .normalized = GL_FALSE,
            .stride = static_cast<GLsizei>(sizeof(TextVertex)),
            .offset = offsetof(TextVertex, u)
        },
        gfx::VertexAttribute{
            .index = 2,
            .components = 4,
            .type = GL_FLOAT,
            .normalized = GL_FALSE,
            .stride = static_cast<GLsizei>(sizeof(TextVertex)),
            .offset = offsetof(TextVertex, r)
        }
    }} {
    if (!shader_) {
        throw std::invalid_argument("UI batcher requires a shader");
    }
    if (!text_shader_) {
        throw std::invalid_argument("UI batcher requires a text shader");
    }
    if (!font_) {
        throw std::invalid_argument("UI batcher requires a font");
    }

    const std::size_t vertex_capacity = std::max<std::size_t>(6, initial_capacity);
    vertices_.reserve(vertex_capacity);
    text_vertices_.reserve(vertex_capacity);

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

    text_vertex_buffer_ = std::make_shared<gfx::Buffer>(
        vertex_capacity * sizeof(TextVertex),
        gfx::BufferUsage::DynamicDraw,
        gfx::BufferTarget::ArrayBuffer
    );
    text_mesh_ = std::make_unique<gfx::Mesh>(
        text_vertex_buffer_,
        std::span<const gfx::VertexAttribute>(text_layout_),
        6,
        GL_TRIANGLES
    );
}

void UIBatcher::begin() noexcept {
    vertices_.clear();
    text_vertices_.clear();
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

void UIBatcher::text(
    const gfx::Rect& bounds,
    const std::string& text,
    float font_size,
    const Color& color
) {
    if (bounds.empty() || text.empty() || font_size <= 0.0f) {
        return;
    }

    const float scale = font_size / static_cast<float>(font_->pixel_size());
    const float text_width = font_->measure(text, font_size);
    float cursor_x = static_cast<float>(bounds.x)
        + (static_cast<float>(bounds.width) - text_width) * 0.5f;

    const float baseline_y = static_cast<float>(bounds.y)
        + (static_cast<float>(bounds.height) - font_size) * 0.5f
        + font_size;

    for (unsigned char character : text) {
        const gfx::Glyph* glyph = font_->glyph(character);
        if (glyph == nullptr) {
            continue;
        }

        const float glyph_width = glyph->width * scale;
        const float glyph_height = glyph->height * scale;
        const float x0 = cursor_x + glyph->bearing_x * scale;
        const float y0 = baseline_y - glyph->bearing_y * scale;
        const float x1 = x0 + glyph_width;
        const float y1 = y0 + glyph_height;

        text_vertices_.insert(text_vertices_.end(), {
            TextVertex{x0, y0, 0.0f, glyph->u0, glyph->v0, color.r, color.g, color.b, color.a},
            TextVertex{x1, y0, 0.0f, glyph->u1, glyph->v0, color.r, color.g, color.b, color.a},
            TextVertex{x1, y1, 0.0f, glyph->u1, glyph->v1, color.r, color.g, color.b, color.a},
            TextVertex{x0, y0, 0.0f, glyph->u0, glyph->v0, color.r, color.g, color.b, color.a},
            TextVertex{x1, y1, 0.0f, glyph->u1, glyph->v1, color.r, color.g, color.b, color.a},
            TextVertex{x0, y1, 0.0f, glyph->u0, glyph->v1, color.r, color.g, color.b, color.a}
        });

        cursor_x += glyph->advance * scale;
    }
}

void UIBatcher::flush(const math::Mat4& projection) {
    if (!vertices_.empty()) {
        vertex_buffer_->update_orphan(
            std::as_bytes(std::span<const Vertex>(vertices_.data(), vertices_.size()))
        );
        mesh_->set_vertex_count(static_cast<GLsizei>(vertices_.size()));

        shader_->bind();
        shader_->set_mat4_col_major("uProjection", projection.data);
        shader_->set_mat4_col_major("uModel", math::Mat4::identity().data);
        mesh_->draw();
        shader_->unbind();
    }

    if (!text_vertices_.empty()) {
        text_vertex_buffer_->update_orphan(
            std::as_bytes(std::span<const TextVertex>(text_vertices_.data(), text_vertices_.size()))
        );
        text_mesh_->set_vertex_count(static_cast<GLsizei>(text_vertices_.size()));

        text_shader_->bind();
        text_shader_->set_mat4_col_major("uProjection", projection.data);
        text_shader_->set_mat4_col_major("uModel", math::Mat4::identity().data);
        text_shader_->set_int("uGlyphAtlas", 0);
        font_->atlas().bind(0);
        text_mesh_->draw();
        font_->atlas().unbind(0);
        text_shader_->unbind();
    }
}

}
