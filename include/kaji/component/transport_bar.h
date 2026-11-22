#pragma once

#include "kaji/transport.h"
#include "kaji/ui/button.h"
#include "kaji/ui/component.h"
#include <memory>

namespace kaji::component {

struct TransportBarConfig {
    int width = 1280;
    int height = 80;
    float sample_rate = 44100.0f;
};

class TransportBar : public kaji::ui::Panel {
public:
    explicit TransportBar(
        std::shared_ptr<kaji::TransportState> state,
        const TransportBarConfig& config = TransportBarConfig{}
    );

    bool is_playing() const noexcept;
    bool is_recording() const noexcept;
    bool is_looping() const noexcept;

private:
    void sync_button_states() noexcept;

    std::shared_ptr<kaji::TransportState> state_;
    TransportBarConfig config_;

    kaji::ui::Button* play_button_ = nullptr;
    kaji::ui::Button* stop_button_ = nullptr;
    kaji::ui::Button* record_button_ = nullptr;
    kaji::ui::Button* loop_button_ = nullptr;
};

}
