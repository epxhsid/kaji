#pragma once

#include "kaji/ui/component.h"
#include <functional>
#include <string>

namespace kaji::ui {

class Button : public UIElement {
public:
    using Callback = std::function<void()>;

    explicit Button(LayoutStyle style, Callback callback);

    void on_mouse_press(kaji::MouseButton button) override;
    void on_mouse_release(kaji::MouseButton button) override;

    void set_toggled(bool toggled) noexcept;
    bool toggled() const noexcept;

    void set_label(std::string label);
    const std::string& label() const noexcept;

    void paint(UIBatcher& batcher) const override;

private:
    Callback callback_;
    bool toggled_ = false;
    std::string label_;
};

}
