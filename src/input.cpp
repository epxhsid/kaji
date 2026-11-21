#include <GL/glew.h>
#include "kaji/input.h"
#include "kaji/application.h"

namespace kaji {

void InputState::begin_frame() noexcept {
    previous_keys_ = keys_;
    previous_mouse_buttons_ = mouse_buttons_;

    previous_mouse_x_ = mouse_x_;
    previous_mouse_y_ = mouse_y_;
    mouse_delta_x_ = 0.0;
    mouse_delta_y_ = 0.0;
    scroll_x_ = 0.0;
    scroll_y_ = 0.0;
}

bool InputState::key_down(int key) const noexcept {
    if (key < 0 || key > GLFW_KEY_LAST) {
        return false;
    }
    return keys_[key];
}

bool InputState::key_pressed(int key) const noexcept {
    if (key < 0 || key > GLFW_KEY_LAST) {
        return false;
    }
    return keys_[key] && !previous_keys_[key];
}

bool InputState::key_released(int key) const noexcept {
    if (key < 0 || key > GLFW_KEY_LAST) {
        return false;
    }
    return !keys_[key] && previous_keys_[key];
}

bool InputState::mouse_down(MouseButton button) const noexcept {
    const int index = static_cast<int>(button);
    if (index < 0 || index > GLFW_MOUSE_BUTTON_LAST) {
        return false;
    }
    return mouse_buttons_[index];
}

bool InputState::mouse_pressed(MouseButton button) const noexcept {
    const int index = static_cast<int>(button);
    if (index < 0 || index > GLFW_MOUSE_BUTTON_LAST) {
        return false;
    }
    return mouse_buttons_[index] && !previous_mouse_buttons_[index];
}

bool InputState::mouse_released(MouseButton button) const noexcept {
    const int index = static_cast<int>(button);
    if (index < 0 || index > GLFW_MOUSE_BUTTON_LAST) {
        return false;
    }
    return !mouse_buttons_[index] && previous_mouse_buttons_[index];
}

void InputState::cursor_position_callback(GLFWwindow* window, double x, double y) {
    auto* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (application == nullptr) {
        return;
    }

    InputState& input = application->input();
    input.mouse_delta_x_ += x - input.mouse_x_;
    input.mouse_delta_y_ += y - input.mouse_y_;
    input.mouse_x_ = x;
    input.mouse_y_ = y;
}

void InputState::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (application == nullptr) {
        return;
    }

    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) {
        return;
    }

    InputState& input = application->input();
    input.set_modifiers(mods);
    input.mouse_buttons_[button] = action != GLFW_RELEASE;
}

void InputState::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (application == nullptr) {
        return;
    }

    if (key < 0 || key > GLFW_KEY_LAST) {
        return;
    }

    InputState& input = application->input();
    input.set_modifiers(mods);

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        input.keys_[key] = true;
    } else if (action == GLFW_RELEASE) {
        input.keys_[key] = false;
    }
}

void InputState::scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    auto* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (application == nullptr) {
        return;
    }

    application->input().scroll_x_ += x_offset;
    application->input().scroll_y_ += y_offset;
}

}
