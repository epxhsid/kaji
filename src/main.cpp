#include "kaji/application.h"
#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
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
        auto waveform_mesh = std::make_unique<kaji::gfx::Mesh>(1024, kaji::gfx::BufferUsage::Dynamic);

        std::vector<float> initial_verts = {
            -0.6f, -0.4f, 0.0f,
             0.0f, -0.4f, 0.0f,
            -0.3f,  0.4f, 0.0f
        };

        waveform_mesh->update_mapped(initial_verts, true);

        std::vector<float> updated_verts = {
             0.0f, -0.4f, 0.0f,
             0.6f, -0.4f, 0.0f,
             0.3f,  0.4f, 0.0f
        };

        std::this_thread::sleep_for(std::chrono::seconds(4));
        waveform_mesh->update_orphan(updated_verts);

        app.add_render_item(shader, std::move(waveform_mesh));

        app.run();
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "fatal error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
