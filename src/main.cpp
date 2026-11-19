#include "kaji/application.h"
#include "kaji/gfx/buffer.h"
#include "kaji/gfx/shader.h"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <span>
#include <string>
#include <vector>

int main() {
    try {
        kaji::Application app;

        if (!app.initialize()) {
            return EXIT_FAILURE;
        }

        const std::string vert_path = std::string(KAJI_SHADER_DIR) + "/triangle.vert";
        const std::string frag_path = std::string(KAJI_SHADER_DIR) + "/triangle.frag";

        auto shader = std::make_shared<kaji::gfx::Shader>(vert_path, frag_path);

        std::vector<float> initial_verts = {
            -0.6f, -0.4f, 0.0f,
            0.0f, -0.4f, 0.0f,
            -0.3f,  0.4f, 0.0f
        };

        auto vertex_buffer = std::make_shared<kaji::gfx::Buffer>(
            std::as_bytes(std::span<const float>(initial_verts)),
            kaji::gfx::BufferUsage::DynamicDraw
        );

        std::array<kaji::gfx::VertexAttribute, 1> layout {{
            {
                .index = 0,
                .components = 3,
                .type = GL_FLOAT,
                .normalized = GL_FALSE,
                .stride = static_cast<GLsizei>(3 * sizeof(float)),
                .offset = 0
            }
        }};

        auto waveform_mesh = std::make_unique<kaji::gfx::Mesh>(
            vertex_buffer,
            std::span<const kaji::gfx::VertexAttribute>(layout),
            static_cast<GLsizei>(initial_verts.size() / 3)
        );

        std::vector<float> updated_verts = {
             0.0f, -0.4f, 0.0f,
             0.6f, -0.4f, 0.0f,
             0.3f,  0.4f, 0.0f
        };

        vertex_buffer->update_orphan(std::as_bytes(std::span<const float>(updated_verts)));
        waveform_mesh->set_vertex_count(static_cast<GLsizei>(updated_verts.size() / 3));

        app.add_render_item(shader, std::move(waveform_mesh));
        app.run();
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "fatal error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
