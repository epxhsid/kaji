#pragma once

#include "kaji/gfx/texture.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <cstdint>
#include <string>
#include <unordered_map>

namespace kaji::gfx {

struct Glyph {
    float u0 = 0.0f;
    float v0 = 0.0f;
    float u1 = 0.0f;
    float v1 = 0.0f;
    int width = 0;
    int height = 0;
    int bearing_x = 0;
    int bearing_y = 0;
    int advance = 0;
};

class Font {
public:
    Font(const std::string& path, int pixel_size = 32);
    ~Font();

    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;
    Font(Font&&) noexcept;
    Font& operator=(Font&&) noexcept;

    const Glyph* glyph(uint32_t codepoint) const noexcept;
    float measure(const std::string& text, float size) const;
    int pixel_size() const noexcept { return pixel_size_; }
    int line_height() const noexcept { return line_height_; }
    Texture& atlas() noexcept { return atlas_; }
    const Texture& atlas() const noexcept { return atlas_; }

private:
    FT_Library library_ = nullptr;
    FT_Face face_ = nullptr;
    int pixel_size_ = 32;
    int line_height_ = 0;
    std::unordered_map<uint32_t, Glyph> glyphs_;
    Texture atlas_{1, 1, GL_R8, GL_RED};
};

}
