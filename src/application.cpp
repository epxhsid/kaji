#include "kaji/application.h"
#include "kaji/gfx/shader.h"
#include "kaji/math/projection.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

namespace kaji {

Application::Application(std::shared_ptr<kaji::math::IProjection> projection) : projection_(std::move(projection)) {
    if (!projection_) {
        throw std::invalid_argument("projection cannot be null");
    }
}

Application::~Application() {
    render_items_.clear();
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
    glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "failed to initialize opengl extension\n";
        return false;
    }

    setup_viewport();
    return true;
}

void Application::add_render_item(std::shared_ptr<gfx::Shader> shader, std::unique_ptr<gfx::Mesh> mesh, math::Mat4 model) {
    render_items_.push_back({
        std::move(shader),
        std::move(mesh),
        model
    });
}

void Application::run() {
    while (!glfwWindowShouldClose(window_)) {
        glClear(GL_COLOR_BUFFER_BIT);

        for (const auto& item : render_items_) {
            item.shader->bind();
            const math::Mat4 projection_matrix = projection_->get_projection_matrix();
            item.shader->set_mat4_col_major("uProjection", projection_matrix.data);
            item.shader->set_mat4_col_major("uModel", item.model.data);
            item.mesh->draw();
            item.shader->unbind();
        }

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void Application::setup_viewport() {
    glfwSwapInterval(1);
    glfwGetFramebufferSize(window_, &width_, &height_);
    projection_->resize(width_, height_);
    glViewport(0, 0, width_, height_);
    glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
}

void Application::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    auto* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (application == nullptr) {
        return;
    }

    application->width_ = width;
    application->height_ = height;
    glViewport(0, 0, width, height);
}

}
