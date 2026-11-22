#include "kaji/application.h"
#include "kaji/component/transport_bar.h"
#include "kaji/math/ortographic_camera.h"
#include "kaji/transport.h"
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

        auto transport_state =
            std::make_shared<kaji::TransportState>();

        kaji::ui::LayoutStyle root_style;
        root_style.width = kaji::ui::Length::fill();
        root_style.height = kaji::ui::Length::fill();
        root_style.direction = kaji::ui::LayoutDirection::Column;
        root_style.padding = kaji::ui::Insets::all(16);
        root_style.gap = 8;

        auto root = std::make_unique<kaji::ui::Panel>(root_style);

        auto transport_bar =
            std::make_unique<kaji::component::TransportBar>(
                transport_state,
                kaji::component::TransportBarConfig{1280, 80, 44100.0f}
            );

        root->add_child(std::move(transport_bar));

        app.set_ui_root(std::move(root));
        app.run();

        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "fatal error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}
