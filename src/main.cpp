#include "kaji/application.h"
#include "kaji/math/ortographic_camera.h"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

int main() {
    try {
        auto projection = std::make_shared<kaji::math::OrthographicCamera2D>(1280, 780);
        kaji::Application app(std::move(projection));

        if (!app.initialize()) {
            std::cerr << "failed to initialize application\n";
            return EXIT_FAILURE;
        }

        const std::string vert_path = std::string(KAJI_SHADER_DIR) + "/triangle.vert";
        const std::string frag_path = std::string(KAJI_SHADER_DIR) + "/triangle.frag";

        auto shader = std::make_shared<kaji::gfx::Shader>(vert_path, frag_path);

        std::vector<float> vertices = {
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.0f,  -0.5f, 0.0f
        };

        auto vertex_buffer = std::make_shared<kaji::gfx::Buffer>(
            std::as_bytes(std::span<const float>(vertices)),
            kaji::gfx::BufferUsage::StaticDraw
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

        auto mesh = std::make_unique<kaji::gfx::Mesh>(
            vertex_buffer,
            layout,
            static_cast<GLsizei>(vertices.size() / 3)
        );

        const kaji::math::Mat4 scale = kaji::math::Mat4::make_scale(400.0f, 300.0f);
        const kaji::math::Mat4 offset = kaji::math::Mat4::make_translation(640.0f, 390.0f);

        const kaji::math::Mat4 model = offset * scale;

        app.add_render_item(shader, std::move(mesh), model);
        app.run();

        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "fatal error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
