#include "kaji/application.h"
#include "kaji/shader.h"
#include <iostream>
#include <memory>

namespace kaji {

Application::Application(std::string vertex_shader_path, std::string fragment_shader_path)
    : vertex_shader_path_(std::move(vertex_shader_path)),
    fragment_shader_path_(std::move(fragment_shader_path)) {}

Application::~Application() {
    shader_.reset();
    if (vao_ != 0) glDeleteVertexArrays(1, &vao_);
    if (vbo_ != 0) glDeleteBuffers(1, &vbo_);
    if (window_ != nullptr) glfwDestroyWindow(window_);
    glfwTerminate();
}

bool Application::initialize() {
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    if (glfwInit() != GLFW_TRUE) {
        std::cerr << "failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(1280, 780, "kaji", nullptr, nullptr);
    if (window_ == nullptr) {
        std::cerr << "failed to create window\n";
        return false;
    }

    glfwMakeContextCurrent(window_);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "failed to initialize opengl extension\n";
        return false;
    }

    setup_viewport();

    shader_ = std::make_unique<Shader>(vertex_shader_path_, fragment_shader_path_);

    setup_buffers();
    return true;
}

void Application::setup_viewport() {
    glfwSwapInterval(1);
    glfwGetFramebufferSize(window_, &width_, &height_);
    glViewport(0, 0, width_, height_);
    glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
}

void Application::setup_buffers() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Application::run() {
    while (!glfwWindowShouldClose(window_)) {
        glClear(GL_COLOR_BUFFER_BIT);
        shader_->bind();
        glBindVertexArray(vao_);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

}
