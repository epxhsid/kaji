#pragma once

#include "kaji/gfx/mesh.h"
#include "kaji/gfx/shader.h"
#include <memory>
#include <vector>

namespace kaji::gfx {

struct DrawCommand {
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Mesh> mesh;
};

class Renderer {
public:
    void submit(DrawCommand cmd);
    void clear();
    void render_all() const;

private:
    std::vector<DrawCommand> commands_;
};

}
