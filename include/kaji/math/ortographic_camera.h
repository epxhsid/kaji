#pragma once

#include "kaji/math/projection.h"

namespace kaji::math {

class OrthographicCamera2D final : public IProjection {
public:
    OrthographicCamera2D(int width, int height, float near_z = -1.0f, float far_z = 1.0f);

    void resize(int width, int height) override;

    Mat4 get_projection_matrix() const override;

private:
    int width_;
    int height_;
    float near_z_;
    float far_z_;
};

}
