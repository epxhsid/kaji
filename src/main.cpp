#include "kaji/application.h"
#include "kaji/math/ortographic_camera.h"
#include "kaji/ui/component.h"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <utility>

int main() {
    try {
        auto projection =
            std::make_shared<kaji::math::OrthographicCamera2D>(1280, 780);
        kaji::Application app(std::move(projection));

        if (!app.initialize()) {
            std::cerr << "failed to initialize application\n";
            return EXIT_FAILURE;
        }

        kaji::ui::LayoutStyle root_style;
        root_style.width = kaji::ui::Length::fill();
        root_style.height = kaji::ui::Length::fill();
        root_style.padding = kaji::ui::Insets::all(16);
        root_style.direction = kaji::ui::LayoutDirection::Column;
        root_style.gap = 8;

        auto root = std::make_unique<kaji::ui::Panel>(root_style);

        kaji::ui::LayoutStyle header_style;
        header_style.width = kaji::ui::Length::fill();
        header_style.height = kaji::ui::Length::fixed(80);

        kaji::ui::LayoutStyle lane_style;
        lane_style.width = kaji::ui::Length::fill();
        lane_style.height = kaji::ui::Length::fill();

        auto* header = root->add_child(
            std::make_unique<kaji::ui::Panel>(header_style)
        );
        auto* lanes = root->add_child(
            std::make_unique<kaji::ui::Panel>(lane_style)
        );

        app.set_ui_root(std::move(root));

        if (header == nullptr || lanes == nullptr) {
            throw std::runtime_error("failed to create layout demo");
        }

        app.run();
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "fatal error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}
