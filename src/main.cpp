#include "kaji/application.h"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

int main() {
    try {
        const std::string vert_path = std::string(KAJI_SHADER_DIR) + "/triangle.vert";
        const std::string frag_path = std::string(KAJI_SHADER_DIR) + "/triangle.frag";

        kaji::Application app(vert_path, frag_path);

        if (!app.initialize()) {
            return EXIT_FAILURE;
        }

        app.run();
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "fatal error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
