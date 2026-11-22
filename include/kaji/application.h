#pragma once
#include "kaji/gfx/shader.h"
#include "kaji/gfx/mesh.h"
#include "kaji/math/projection.h"
#include "kaji/input.h"
#include "kaji/ui/component.h"
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
    void shutdown() noexcept;

    void add_render_item(
        std::shared_ptr<gfx::Shader> shader,
        std::unique_ptr<gfx::Mesh> mesh,
        math::Mat4 model = math::Mat4::identity()
    );

    void set_ui_root(std::unique_ptr<ui::UIElement> root);

    InputState& input() noexcept { return input_; }
    const InputState& input() const noexcept { return input_; }

    void run();

private:
    void setup_viewport();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void dispatch_ui_input() noexcept;

    ui::UIElement* hovered_ui_element_ = nullptr;
    ui::UIElement* captured_ui_element_ = nullptr;

    std::shared_ptr<kaji::math::IProjection> projection_;
    GLFWwindow* window_ = nullptr;
    std::vector<RenderItem> render_items_;
    std::unique_ptr<ui::UIElement> ui_root_;
    std::shared_ptr<gfx::Shader> ui_shader_;
    std::unique_ptr<ui::UIBatcher> ui_batcher_;
    InputState input_;
    int width_ = 0;
    int height_ = 0;
    bool glfw_initialized_ = false;
    bool opengl_initialized_ = false;
};

}
