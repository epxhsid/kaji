#pragma once

#include <GLFW/glfw3.h>
#include <array>

namespace kaji {

enum class MouseButton {
    Left = GLFW_MOUSE_BUTTON_LEFT,
    Right = GLFW_MOUSE_BUTTON_RIGHT,
    Middle = GLFW_MOUSE_BUTTON_MIDDLE
};

class InputState {
public:
    void begin_frame() noexcept;

    bool key_down(int key) const noexcept;
    bool key_pressed(int key) const noexcept;
    bool key_released(int key) const noexcept;

    bool mouse_down(MouseButton button) const noexcept;
    bool mouse_pressed(MouseButton button) const noexcept;
    bool mouse_released(MouseButton button) const noexcept;

    double mouse_x() const noexcept { return mouse_x_; }
    double mouse_y() const noexcept { return mouse_y_; }
    double mouse_delta_x() const noexcept { return mouse_delta_x_; }
    double mouse_delta_y() const noexcept { return mouse_delta_y_; }
    double scroll_x() const noexcept { return scroll_x_; }
    double scroll_y() const noexcept { return scroll_y_; }

    bool shift_down() const noexcept { return shift_down_; }
    bool control_down() const noexcept { return control_down_; }
    bool alt_down() const noexcept { return alt_down_; }
    bool super_down() const noexcept { return super_down_; }

    static void cursor_position_callback(GLFWwindow* window, double x, double y);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

private:
    void set_modifiers(int mods) noexcept {
        shift_down_ = (mods & GLFW_MOD_SHIFT) != 0;
        control_down_ = (mods & GLFW_MOD_CONTROL) != 0;
        alt_down_ = (mods & GLFW_MOD_ALT) != 0;
        super_down_ = (mods & GLFW_MOD_SUPER) != 0;
    }

    std::array<bool, GLFW_KEY_LAST + 1> keys_{};
    std::array<bool, GLFW_KEY_LAST + 1> previous_keys_{};
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> mouse_buttons_{};
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> previous_mouse_buttons_{};

    double mouse_x_ = 0.0;
    double mouse_y_ = 0.0;
    double previous_mouse_x_ = 0.0;
    double previous_mouse_y_ = 0.0;
    double mouse_delta_x_ = 0.0;
    double mouse_delta_y_ = 0.0;
    double scroll_x_ = 0.0;
    double scroll_y_ = 0.0;

    bool shift_down_ = false;
    bool control_down_ = false;
    bool alt_down_ = false;
    bool super_down_ = false;
};

}
