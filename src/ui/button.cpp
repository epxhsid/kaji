#include "kaji/ui/button.h"
#include <algorithm>

namespace kaji::ui {

Button::Button(LayoutStyle style, Callback callback)
    : UIElement(std::move(style)), callback_(std::move(callback)) {
}

void Button::on_mouse_press(kaji::MouseButton button) {
    if (button != kaji::MouseButton::Left) {
        return;
    }

    UIElement::on_mouse_press(button);
}

void Button::on_mouse_release(kaji::MouseButton button) {
    if (button != kaji::MouseButton::Left || !enabled()) {
        return;
    }

    UIElement::on_mouse_release(button);

    if (callback_) {
        callback_();
    }
}

void Button::set_toggled(bool toggled) noexcept {
    toggled_ = toggled;
}

bool Button::toggled() const noexcept {
    return toggled_;
}

void Button::paint(UIBatcher& batcher) const {
    if (!visible() || bounds().empty()) {
        return;
    }

    if (!toggled_) {
        UIElement::paint(batcher);
        return;
    }

    Color color = style().background;
    color.r = std::min(1.0f, color.r + 0.18f);
    color.g = std::min(1.0f, color.g + 0.18f);
    color.b = std::min(1.0f, color.b + 0.18f);

    if (state() == ComponentState::Pressed) {
        color.r = std::max(0.0f, color.r - 0.08f);
        color.g = std::max(0.0f, color.g - 0.08f);
        color.b = std::max(0.0f, color.b - 0.08f);
    }

    batcher.quad(bounds(), color);
}

}
