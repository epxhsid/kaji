#pragma once
#include "kaji/shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

namespace kaji {

class Application {
public:
    Application(std::string vertex_shader_path, std::string fragment_shader_path);
    ~Application();

    bool initialize();
    void run();

private:
    void setup_buffers();
    void setup_viewport();

    GLFWwindow* window_ = nullptr;
    std::string vertex_shader_path_;
    std::string fragment_shader_path_;
    std::unique_ptr<Shader> shader_;
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    int width_ = 0;
    int height_ = 0;
};

}
