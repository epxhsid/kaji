#include "kaji/application.h"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

int main() {
    try {
        kaji::Application app;

        if (!app.initialize()) {
            return EXIT_FAILURE;
        }

        const std::string vert_path = std::string(KAJI_SHADER_DIR) + "/triangle.vert";
        const std::string frag_path = std::string(KAJI_SHADER_DIR) + "/triangle.frag";
        auto shader = std::make_shared<kaji::gfx::Shader>(vert_path, frag_path);

        std::vector<float> triangle_one = {
            -0.5f, -0.5f, 0.0f,
             0.0f, -0.5f, 0.0f,
            -0.25f, 0.5f, 0.0f
        };

        std::vector<float> triangle_two = {
             0.0f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.25f, 0.5f, 0.0f
        };

        std::vector<float> triangle_three = {
            0.5f, -0.5f, 0.0f,
            1.0f, -0.5f, 0.0f,
            0.75f, 0.5f, 0.0f
        };

        app.add_render_item(shader, std::make_unique<kaji::gfx::Mesh>(triangle_one));
        app.add_render_item(shader, std::make_unique<kaji::gfx::Mesh>(triangle_two));
        app.add_render_item(shader, std::make_unique<kaji::gfx::Mesh>(triangle_three));

        app.run();
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "fatal error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
