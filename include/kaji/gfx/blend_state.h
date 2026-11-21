#pragma once

#include <GL/glew.h>

namespace kaji::gfx {

class BlendState {
public:
    static void enable_alpha() noexcept;
    static void disable() noexcept;
};

}
