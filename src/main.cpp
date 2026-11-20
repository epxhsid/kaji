#include "kaji/application.h"
#include "kaji/math/ortographic_camera.h"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <utility>

int main() {
    try {
        auto projection = std::make_shared<kaji::math::OrthographicCamera2D>(1280, 780);
        kaji::Application app(std::move(projection));

        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "fatal error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
