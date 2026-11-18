#pragma once
#include "kaji/shader.h"
#include "kaji/mesh.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

namespace kaji {

struct RenderItem {
    std::shared_ptr<Shader> shader;
    std::unique_ptr<Mesh> mesh;
};

class Application {
public:
    Application();
    ~Application();

    bool initialize();
    void add_render_item(std::shared_ptr<Shader> shader, std::unique_ptr<Mesh> mesh);
    void run();

private:
    void setup_viewport();
    GLFWwindow* window_ = nullptr;
    std::vector<RenderItem> render_items_;
    int width_ = 0;
    int height_ = 0;
};

}
