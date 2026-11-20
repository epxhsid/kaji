#pragma once

#include "kaji/math/projection.h"
#include <stdexcept>

namespace kaji::math {

class OrthographicCamera2D final : public IProjection {
public:
    OrthographicCamera2D(int width, int height, float near_z = -1.0f, float far_z = 1.0f)
        : width_(width), height_(height), near_z_(near_z), far_z_(far_z) {
        if (width <= 0 || height <= 0) {
            throw std::invalid_argument("camera dimensions must be positive");
        }
    }

    void resize(int width, int height) override {
        if (width <= 0 || height <= 0) {
            return;
        }
        width_ = width;
        height_ = height;
    }

    Mat4 get_projection_matrix() const override {
        return Mat4::make_orthographic(
            0.0f,
            static_cast<float>(width_),
            static_cast<float>(height_),
            0.0f,
            near_z_,
            far_z_
        );
    }

private:
    int width_;
    int height_;
    float near_z_;
    float far_z_;
};

}
