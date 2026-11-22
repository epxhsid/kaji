#include "kaji/ui/component.h"
#include <algorithm>
#include <utility>

namespace kaji::ui {

Length Length::fixed(int pixels) noexcept {
    return Length{SizeMode::Fixed, pixels};
}

Length Length::fill() noexcept {
    return Length{SizeMode::Fill, 0};
}

Insets Insets::all(int value) noexcept {
    return Insets{value, value, value, value};
}

Insets Insets::symmetric(int horizontal, int vertical) noexcept {
    return Insets{horizontal, vertical, horizontal, vertical};
}

UIElement::UIElement(LayoutStyle style)
    : style_(style) {
}

const gfx::Rect& UIElement::bounds() const noexcept {
    return bounds_;
}

ComponentState UIElement::state() const noexcept {
    return state_;
}

bool UIElement::visible() const noexcept {
    return visible_;
}

bool UIElement::enabled() const noexcept {
    return enabled_;
}

void UIElement::set_bounds(const gfx::Rect& bounds) noexcept {
    bounds_ = bounds;
}

void UIElement::set_state(ComponentState state) noexcept {
    state_ = state;
}

void UIElement::set_visible(bool visible) noexcept {
    visible_ = visible;
}

void UIElement::set_enabled(bool enabled) noexcept {
    enabled_ = enabled;
    if (!enabled) {
        state_ = ComponentState::Disabled;
    } else if (state_ == ComponentState::Disabled) {
        state_ = ComponentState::Normal;
    }
}

void UIElement::set_style(const LayoutStyle& style) noexcept {
    style_ = style;
}

const LayoutStyle& UIElement::style() const noexcept {
    return style_;
}

void UIElement::layout(const gfx::Rect& available) {
    const Insets& margin = style_.margin;

    const int available_width = std::max(0, available.width - margin.left - margin.right);
    const int available_height = std::max(0, available.height - margin.top - margin.bottom);

    int width = 0;
    if (style_.width.mode == SizeMode::Fill) {
        width = available_width;
    } else {
        width = std::clamp(style_.width.value, 0, available_width);
    }

    int height = 0;
    if (style_.height.mode == SizeMode::Fill) {
        height = available_height;
    } else {
        height = std::clamp(style_.height.value, 0, available_height);
    }

    bounds_ = gfx::Rect{
        available.x + margin.left,
        available.y + margin.top,
        width,
        height
    };
}

Panel::Panel(LayoutStyle style)
    : UIElement(style) {
}

UIElement* Panel::add_child(std::unique_ptr<UIElement> child) {
    if (!child) {
        return nullptr;
    }

    UIElement* result = child.get();
    children_.push_back(std::move(child));
    return result;
}

std::unique_ptr<UIElement> Panel::remove_child(UIElement* child) {
    for (auto it = children_.begin(); it != children_.end(); ++it) {
        if (it->get() == child) {
            std::unique_ptr<UIElement> removed = std::move(*it);
            children_.erase(it);
            return removed;
        }
    }
    return nullptr;
}

void Panel::clear_children() noexcept {
    children_.clear();
}

std::size_t Panel::child_count() const noexcept {
    return children_.size();
}

UIElement* Panel::child(std::size_t index) noexcept {
    if (index >= children_.size()) {
        return nullptr;
    }
    return children_[index].get();
}

const UIElement* Panel::child(std::size_t index) const noexcept {
    if (index >= children_.size()) {
        return nullptr;
    }
    return children_[index].get();
}

void Panel::layout(const gfx::Rect& available) {
    UIElement::layout(available);

    gfx::Rect inner = bounds_.inset(
        style_.padding.left,
        style_.padding.top,
        style_.padding.right,
        style_.padding.bottom
    );

    if (children_.empty() || inner.empty()) {
        return;
    }

    std::size_t fill_count = 0;
    int fixed_space = 0;

    for (const auto& child : children_) {
        if (!child->visible()) {
            continue;
        }

        const LayoutStyle& child_style = child->style();

        if (style_.direction == LayoutDirection::Row
            && child_style.width.mode == SizeMode::Fill) {
            ++fill_count;
        } else if (style_.direction == LayoutDirection::Column
            && child_style.height.mode == SizeMode::Fill) {
            ++fill_count;
        }

        if (style_.direction == LayoutDirection::Row
            && child_style.width.mode == SizeMode::Fixed) {
            fixed_space += child_style.width.value;
        } else if (style_.direction == LayoutDirection::Column
            && child_style.height.mode == SizeMode::Fixed) {
            fixed_space += child_style.height.value;
        }

        fixed_space += child_style.margin.left + child_style.margin.right;
        if (style_.direction == LayoutDirection::Column) {
            fixed_space += child_style.margin.top + child_style.margin.bottom;
        }
    }

    if (style_.direction == LayoutDirection::Row) {
        fixed_space -= 0;
    }

    int visible_count = 0;
    for (const auto& child : children_) {
        if (child->visible()) {
            ++visible_count;
        }
    }

    const int gaps = std::max(0, visible_count - 1) * style_.gap;

    int cursor = inner.x;
    int remaining = inner.width - fixed_space - gaps;

    if (style_.direction == LayoutDirection::Column) {
        remaining = inner.height;
        fixed_space = 0;

        for (const auto& child : children_) {
            if (!child->visible()) {
                continue;
            }

            const LayoutStyle& child_style = child->style();
            if (child_style.height.mode == SizeMode::Fixed) {
                fixed_space += child_style.height.value;
            }
            fixed_space += child_style.margin.top + child_style.margin.bottom;
        }

        remaining = std::max(0, inner.height - fixed_space - gaps);
    }

    cursor = (style_.direction == LayoutDirection::Row) ? inner.x : inner.y;

    for (const auto& child : children_) {
        if (!child->visible()) {
            continue;
        }

        const LayoutStyle& child_style = child->style();

        if (style_.direction == LayoutDirection::Row) {
            int slot_width = 0;

            if (child_style.width.mode == SizeMode::Fill) {
                slot_width = fill_count > 0
                    ? std::max(0, remaining / static_cast<int>(fill_count))
                    : 0;
            } else {
                slot_width = child_style.width.value
                    + child_style.margin.left
                    + child_style.margin.right;
            }

            const int slot_x = cursor;
            child->layout(gfx::Rect{
                slot_x,
                inner.y,
                slot_width,
                inner.height
            });

            cursor += slot_width + style_.gap;
        } else {
            int slot_height = 0;

            if (child_style.height.mode == SizeMode::Fill) {
                slot_height = fill_count > 0
                    ? std::max(0, remaining / static_cast<int>(fill_count))
                    : 0;
            } else {
                slot_height = child_style.height.value
                    + child_style.margin.top
                    + child_style.margin.bottom;
            }

            const int slot_y = cursor;
            child->layout(gfx::Rect{
                inner.x,
                slot_y,
                inner.width,
                slot_height
            });

            cursor += slot_height + style_.gap;
        }
    }
}

}
