#include "kaji/math/ortographic_camera.h"
#include <stdexcept>

namespace kaji::math {

OrthographicCamera2D::OrthographicCamera2D(int width, int height, float near_z, float far_z)
    : width_(width), height_(height), near_z_(near_z), far_z_(far_z) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("camera dimensions must be positive");
    }
}

void OrthographicCamera2D::resize(int width, int height) {
    if (width <= 0 || height <= 0) {
        return;
    }

    width_ = width;
    height_ = height;
}

Mat4 OrthographicCamera2D::get_projection_matrix() const {
    return Mat4::make_orthographic(
        0.0f,
        static_cast<float>(width_),
        static_cast<float>(height_),
        0.0f,
        near_z_,
        far_z_
    );
}

}
