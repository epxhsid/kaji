#include "kaji/gfx/renderer.h"

namespace kaji::gfx {

void Renderer::submit(DrawCommand cmd) {
    commands_.push_back(std::move(cmd));
}

void Renderer::clear() {
    commands_.clear();
}

void Renderer::render_all() const {
    for (const auto& cmd : commands_) {
        if (!cmd.shader || !cmd.mesh) {
            continue;
        }
        cmd.shader->bind();
        cmd.mesh->draw();
        cmd.shader->unbind();
    }
}

}
