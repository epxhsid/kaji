#include "kaji/gfx/rect.h"
#include <algorithm>
#include <GL/glew.h>

namespace kaji::gfx {

int Rect::right() const noexcept {
    return x + width;
}

int Rect::bottom() const noexcept {
    return y + height;
}

bool Rect::empty() const noexcept {
    return width <= 0 || height <= 0;
}

Rect Rect::intersect(const Rect &other) const noexcept {
    const int left = std::max(x, other.x);
    const int top = std::max(y, other.y);
    const int right = std::min(Rect::right(), other.right());
    const int bottom = std::min(Rect::bottom(), other.bottom());

    if (left >= right || top >= bottom) {
        return Rect();
    }

    return Rect(left, top, right - left, bottom - top);
}

ClipRect::ClipRect(int framebuffer_width, int framebuffer_height)
    : framebuffer_width_(framebuffer_width),
      framebuffer_height_(framebuffer_height) {
}

void ClipRect::resize(int framebuffer_width, int framebuffer_height) {
    if (framebuffer_width <= 0 || framebuffer_height <= 0) {
        return;
    }

    framebuffer_width_ = framebuffer_width;
    framebuffer_height_ = framebuffer_height;
    apply();
}

void ClipRect::push(const Rect& rect) {
    stack_.push_back(rect);
    glEnable(GL_SCISSOR_TEST);
    apply();
}

void ClipRect::pop() {
    if (stack_.empty()) {
        return;
    }

    stack_.pop_back();
    if (stack_.empty()) {
        glScissor(0, 0, framebuffer_width_, framebuffer_height_);
        glDisable(GL_SCISSOR_TEST);
        return;
    }

    apply();
}

Rect ClipRect::current() const noexcept {
    if (stack_.empty()) {
        return Rect(0, 0, framebuffer_width_, framebuffer_height_);
    }

    Rect result{
        0, 0, framebuffer_width_, framebuffer_height_
    };

    for (const Rect& rect : stack_) {
        result = result.intersect(rect);
        if (result.empty()) {
            break;
        }
    }

    return result;
}

void ClipRect::apply() const {
    const Rect rect = current();

    if (rect.empty()) {
        glScissor(0, 0, 0, 0);
        return;
    }

    const int gl_y = framebuffer_height_ - rect.y - rect.height;

    glScissor(rect.x, gl_y, rect.width, rect.height);
}

ScopedClip::ScopedClip(ClipRect& clip, const Rect& rect)
    : clip_(&clip) {
    clip_->push(rect);
}

ScopedClip::~ScopedClip() {
    if (clip_) {
        clip_->pop();
    }
}

}
