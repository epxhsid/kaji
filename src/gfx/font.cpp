#include "kaji/gfx/font.h"

#include <algorithm>
#include <span>
#include <stdexcept>
#include <vector>

namespace kaji::gfx {

Font::Font(const std::string& path, int pixel_size)
    : pixel_size_(pixel_size), atlas_(512, 512, GL_R8, GL_RED) {
    if (pixel_size <= 0) {
        throw std::invalid_argument("font pixel size must be positive");
    }

    if (FT_Init_FreeType(&library_) != 0) {
        throw std::runtime_error("failed to initialize FreeType");
    }

    if (FT_New_Face(library_, path.c_str(), 0, &face_) != 0) {
        FT_Done_FreeType(library_);
        library_ = nullptr;
        throw std::runtime_error("failed to load font: " + path);
    }

    if (FT_Set_Pixel_Sizes(face_, 0, static_cast<FT_UInt>(pixel_size_)) != 0) {
        FT_Done_Face(face_);
        face_ = nullptr;
        FT_Done_FreeType(library_);
        library_ = nullptr;
        throw std::runtime_error("failed to set font size");
    }

    line_height_ = static_cast<int>(face_->size->metrics.height >> 6);

    constexpr int atlas_width = 512;
    constexpr int atlas_height = 512;
    std::vector<unsigned char> pixels(atlas_width * atlas_height, 0);

    int cursor_x = 1;
    int cursor_y = 1;
    int row_height = 0;

    for (uint32_t codepoint = 32; codepoint <= 126; ++codepoint) {
        if (FT_Load_Char(face_, codepoint, FT_LOAD_RENDER) != 0) {
            continue;
        }

        FT_GlyphSlot slot = face_->glyph;
        const int glyph_width = static_cast<int>(slot->bitmap.width);
        const int glyph_height = static_cast<int>(slot->bitmap.rows);

        if (cursor_x + glyph_width + 1 >= atlas_width) {
            cursor_x = 1;
            cursor_y += row_height + 1;
            row_height = 0;
        }

        if (cursor_y + glyph_height + 1 >= atlas_height) {
            throw std::runtime_error("glyph atlas is too small");
        }

        for (int y = 0; y < glyph_height; ++y) {
            for (int x = 0; x < glyph_width; ++x) {
                pixels[(cursor_y + y) * atlas_width + cursor_x + x] =
                    slot->bitmap.buffer[y * slot->bitmap.pitch + x];
            }
        }

        Glyph glyph;
        glyph.u0 = static_cast<float>(cursor_x) / atlas_width;
        glyph.v0 = static_cast<float>(cursor_y) / atlas_height;
        glyph.u1 = static_cast<float>(cursor_x + glyph_width) / atlas_width;
        glyph.v1 = static_cast<float>(cursor_y + glyph_height) / atlas_height;
        glyph.width = glyph_width;
        glyph.height = glyph_height;
        glyph.bearing_x = slot->bitmap_left;
        glyph.bearing_y = slot->bitmap_top;
        glyph.advance = static_cast<int>(slot->advance.x >> 6);
        glyphs_[codepoint] = glyph;

        cursor_x += glyph_width + 1;
        row_height = std::max(row_height, glyph_height);
    }

    atlas_.upload(std::as_bytes(std::span<const unsigned char>(pixels)));
}

Font::~Font() {
    if (face_ != nullptr) {
        FT_Done_Face(face_);
    }
    if (library_ != nullptr) {
        FT_Done_FreeType(library_);
    }
}

Font::Font(Font&& other) noexcept
    : library_(other.library_),
      face_(other.face_),
      pixel_size_(other.pixel_size_),
      line_height_(other.line_height_),
      glyphs_(std::move(other.glyphs_)),
      atlas_(std::move(other.atlas_)) {
    other.library_ = nullptr;
    other.face_ = nullptr;
    other.pixel_size_ = 0;
    other.line_height_ = 0;
}

Font& Font::operator=(Font&& other) noexcept {
    if (this != &other) {
        if (face_ != nullptr) FT_Done_Face(face_);
        if (library_ != nullptr) FT_Done_FreeType(library_);

        library_ = other.library_;
        face_ = other.face_;
        pixel_size_ = other.pixel_size_;
        line_height_ = other.line_height_;
        glyphs_ = std::move(other.glyphs_);
        atlas_ = std::move(other.atlas_);

        other.library_ = nullptr;
        other.face_ = nullptr;
        other.pixel_size_ = 0;
        other.line_height_ = 0;
    }
    return *this;
}

const Glyph* Font::glyph(uint32_t codepoint) const noexcept {
    const auto it = glyphs_.find(codepoint);
    return it == glyphs_.end() ? nullptr : &it->second;
}

float Font::measure(const std::string& text, float size) const {
    const float scale = size / static_cast<float>(pixel_size_);
    float width = 0.0f;

    for (char character : text) {
        const Glyph* glyph = this->glyph(static_cast<uint32_t>(
            static_cast<unsigned char>(character)
        ));
        if (glyph != nullptr) {
            width += static_cast<float>(glyph->advance) * scale;
        }
    }

    return width;
}

}
