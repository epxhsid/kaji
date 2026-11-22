#include "kaji/component/transport_bar.h"

#include <stdexcept>
#include <utility>

namespace kaji::component {

namespace {
ui::LayoutStyle button_style(int width) {
    ui::LayoutStyle style;
    style.width = ui::Length::fixed(width);
    style.height = ui::Length::fixed(48);
    style.background = ui::Color{0.22f, 0.23f, 0.26f, 1.0f};
    return style;
}
}

TransportBar::TransportBar(std::shared_ptr<kaji::TransportState> state, const TransportBarConfig& config) : Panel(), state_(std::move(state)), config_(config) {
    if (!state_) {
        throw std::invalid_argument("TransportBar requires a TransportState");
    }

    ui::LayoutStyle bar_style;
    bar_style.width = ui::Length::fill();
    bar_style.height = ui::Length::fixed(config_.height);
    bar_style.padding = ui::Insets::symmetric(12, 16);
    bar_style.direction = ui::LayoutDirection::Row;
    bar_style.gap = 8;
    bar_style.background = ui::Color{0.10f, 0.11f, 0.13f, 1.0f};
    set_style(bar_style);

    play_button_ = static_cast<ui::Button*>(
        add_child(std::make_unique<ui::Button>(
            button_style(72),
            [this]() {
                state_->playing = true;
                sync_button_states();
            }
        ))
    );

    stop_button_ = static_cast<ui::Button*>(
        add_child(std::make_unique<ui::Button>(
            button_style(72),
            [this]() {
                state_->stop();
                sync_button_states();
            }
        ))
    );

    record_button_ = static_cast<ui::Button*>(
        add_child(std::make_unique<ui::Button>(
            button_style(72),
            [this]() {
                state_->toggle_recording();
                sync_button_states();
            }
        ))
    );

    loop_button_ = static_cast<ui::Button*>(
        add_child(std::make_unique<ui::Button>(
            button_style(72),
            [this]() {
                state_->toggle_loop();
                sync_button_states();
            }
        ))
    );

    sync_button_states();
}

void TransportBar::sync_button_states() noexcept {
    if (play_button_) {
        play_button_->set_toggled(state_->playing);
    }
    if (record_button_) {
        record_button_->set_toggled(state_->recording);
    }
    if (loop_button_) {
        loop_button_->set_toggled(state_->looping);
    }
}

bool TransportBar::is_playing() const noexcept {
    return state_ && state_->playing;
}

bool TransportBar::is_recording() const noexcept {
    return state_ && state_->recording;
}

bool TransportBar::is_looping() const noexcept {
    return state_ && state_->looping;
}

}
