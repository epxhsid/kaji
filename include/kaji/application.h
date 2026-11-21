#pragma once
#include "kaji/gfx/shader.h"
#include "kaji/gfx/mesh.h"
#include "kaji/math/projection.h"
#include "kaji/input.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

namespace kaji {

struct RenderItem {
    std::shared_ptr<gfx::Shader> shader;
    std::unique_ptr<gfx::Mesh> mesh;
    math::Mat4 model = math::Mat4::identity();
};

class Application {
public:
    Application(std::shared_ptr<kaji::math::IProjection> projection);
    ~Application();

    bool initialize();

    void add_render_item(
        std::shared_ptr<gfx::Shader> shader,
        std::unique_ptr<gfx::Mesh> mesh,
        math::Mat4 model = math::Mat4::identity()
    );

    InputState& input() noexcept { return input_; }
    const InputState& input() const noexcept { return input_; }

    void run();

private:
    void setup_viewport();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    std::shared_ptr<kaji::math::IProjection> projection_;
    GLFWwindow* window_ = nullptr;
    std::vector<RenderItem> render_items_;
    InputState input_;
    int width_ = 0;
    int height_ = 0;
};

}
