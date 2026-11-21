#pragma once

#include <vector>
namespace kaji::gfx {

struct Rect {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;

    int right() const noexcept;
    int bottom() const noexcept;
    bool empty() const noexcept;

    Rect intersect(const Rect& other) const noexcept;
};

class ClipRect {
public:
    ClipRect(int framebuffer_width, int framebuffer_height);
    ~ClipRect() = default;

    ClipRect(const ClipRect&) = delete;
    ClipRect& operator=(const ClipRect&) = delete;

    void resize(int framebuffer_width, int framebuffer_height);
    void push(const Rect& rect);
    void pop();
    void clear();

    Rect current() const noexcept;
private:
    void apply() const;

    int framebuffer_width_;
    int framebuffer_height_;
    std::vector<Rect> stack_;
};

class ScopedClip {
public:
    ScopedClip(ClipRect& clip_rect, const Rect& rect);
    ~ScopedClip();

    ScopedClip(const ScopedClip&) = delete;
    ScopedClip& operator=(const ScopedClip&) = delete;
private:
    ClipRect& clip_;
};

}
