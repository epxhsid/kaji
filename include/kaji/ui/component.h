#pragma once

#include "kaji/gfx/rect.h"
#include <memory>
#include <vector>

namespace kaji::ui {

enum class ComponentState {
    Normal,
    Hovered,
    Pressed,
    Disabled
};

enum class SizeMode {
    Fixed,
    Fill
};

enum class LayoutDirection {
    Row,
    Column
};

struct Length {
    SizeMode mode = SizeMode::Fixed;
    int value = 0;

    static Length fixed(int pixels) noexcept;
    static Length fill() noexcept;
};

struct Insets {
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;

    static Insets all(int value) noexcept;
    static Insets symmetric(int horizontal, int vertical) noexcept;
};

struct LayoutStyle {
    Length width = Length::fixed(0);
    Length height = Length::fixed(0);
    Insets margin{};
    Insets padding{};
    LayoutDirection direction = LayoutDirection::Column;
    int gap = 0;
};

class UIElement {
public:
    UIElement() = default;
    explicit UIElement(LayoutStyle style);
    virtual ~UIElement() = default;

    UIElement(const UIElement&) = delete;
    UIElement& operator=(const UIElement&) = delete;

    const gfx::Rect& bounds() const noexcept;
    ComponentState state() const noexcept;
    bool visible() const noexcept;
    bool enabled() const noexcept;

    void set_bounds(const gfx::Rect& bounds) noexcept;
    void set_state(ComponentState state) noexcept;
    void set_visible(bool visible) noexcept;
    void set_enabled(bool enabled) noexcept;
    void set_style(const LayoutStyle& style) noexcept;
    const LayoutStyle& style() const noexcept;

    virtual void layout(const gfx::Rect& available);

protected:
    gfx::Rect bounds_{0, 0, 0, 0};
    LayoutStyle style_{};
    ComponentState state_ = ComponentState::Normal;
    bool visible_ = true;
    bool enabled_ = true;
};

class Panel : public UIElement {
public:
    Panel() = default;
    explicit Panel(LayoutStyle style);

    UIElement* add_child(std::unique_ptr<UIElement> child);
    std::unique_ptr<UIElement> remove_child(UIElement* child);
    void clear_children() noexcept;

    std::size_t child_count() const noexcept;
    UIElement* child(std::size_t index) noexcept;
    const UIElement* child(std::size_t index) const noexcept;

    void layout(const gfx::Rect& available) override;

private:
    std::vector<std::unique_ptr<UIElement>> children_;
};

}
