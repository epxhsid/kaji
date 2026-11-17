#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>

class Application {
public:
    bool initialize_glfw() {
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
        if (glfwInit() != GLFW_TRUE) {
            std::cerr << "failed to initialize GLFW\n";
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


        window_ = glfwCreateWindow(1280, 780, "kaji", NULL, NULL);
        if (window_ == nullptr) {
            std::cerr << "failed to create window\n";
            return false;
        }

        glfwMakeContextCurrent(window_);

        return true;
    }

    bool initialize_glew() {
        glewExperimental = GL_TRUE;

        GLenum err = glewInit();

        if (err != GLEW_OK) {
            std::cerr << "failed to initialize opengl extension: " << glewGetErrorString(err) << std::endl;
            return false;
        }

        setup_viewport();

        return true;
    }

    void setup_viewport() {
        glfwSwapInterval(1);
        glfwGetFramebufferSize(window_, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
    }

    void run() {
        while (!glfwWindowShouldClose(window_)) {
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window_);
            glfwPollEvents();
        }
    }

    ~Application() {
        if (window_ != nullptr) {
            glfwDestroyWindow(window_);
        }

        glfwTerminate();
    }

private:
    GLFWwindow* window_ = nullptr;
    int width, height;
};

int main() {
    Application app;

    if (!app.initialize_glfw()) {
        return EXIT_FAILURE;
    }

    if (!app.initialize_glew()) {
        return EXIT_FAILURE;
    }

    app.run();

    return EXIT_SUCCESS;
}
