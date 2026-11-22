#pragma once

#include <GL/glew.h>
#include "kaji/gfx/buffer.h"
#include "kaji/gfx/mesh.h"
#include "kaji/gfx/rect.h"
#include "kaji/gfx/shader.h"
#include "kaji/math/projection.h"

#include <array>
#include <memory>
#include <vector>

namespace kaji::ui {

struct Color {
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};

class UIBatcher {
public:
    UIBatcher(std::shared_ptr<gfx::Shader> shader, std::size_t initial_capacity = 4096);

    void begin() noexcept;
    void quad(const gfx::Rect& rect, const Color& color);
    void flush(const math::Mat4& projection);

private:
    struct Vertex {
        float x;
        float y;
        float z;
        float r;
        float g;
        float b;
        float a;
    };

    std::shared_ptr<gfx::Shader> shader_;
    std::shared_ptr<gfx::Buffer> vertex_buffer_;
    std::unique_ptr<gfx::Mesh> mesh_;
    std::array<gfx::VertexAttribute, 2> layout_;
    std::vector<Vertex> vertices_;
};

}
