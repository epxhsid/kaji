#pragma once

#include <array>
namespace kaji::math {

struct Mat4 {
    std::array<float, 16> data {};

    float& operator()(int row, int col);
    const float& operator()(int row, int col) const;

    float* ptr() noexcept;
    const float* ptr() const noexcept;

    static Mat4 identity() noexcept;

    Mat4 operator*(const Mat4& other) const noexcept;

    static Mat4 make_orthographic(float left, float right, float bottom, float top, float near_z, float far_z) noexcept;

    static Mat4 make_translation(float x, float y) noexcept;

    static Mat4 make_scale(float x, float y) noexcept;
};

class IProjection {
public:
    virtual ~IProjection() = default;
    virtual void resize(int width, int height) = 0;
    virtual Mat4 get_projection_matrix() const = 0;
};

}
