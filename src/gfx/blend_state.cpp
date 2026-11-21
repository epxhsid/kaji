#include "kaji/gfx/blend_state.h"

namespace kaji::gfx {

void BlendState::enable_alpha() noexcept {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void BlendState::disable() noexcept {
    glDisable(GL_BLEND);
}

}
