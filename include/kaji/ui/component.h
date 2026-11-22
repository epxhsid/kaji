#pragma once

#include "kaji/gfx/rect.h"
namespace kaji::ui {

enum class ComponentState {
    Normal,
    Hovered,
    Pressed,
    Disabled
};

class UIElement {
public:
    virtual ~UIElement() = default;
private:
    gfx::Rect bounds_{0, 0, 0, 0};

};

}
