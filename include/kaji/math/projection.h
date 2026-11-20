#pragma once

#include <array>
namespace kaji::math {

struct Mat4 {
    std::array<float, 16> data {};

    float& operator()(int row, int col) { return data[col * 4 + row]; }
    const float& operator()(int row, int col) const { return data[col * 4 + row]; }

    float* ptr() noexcept { return data.data(); }
    const float* ptr() const noexcept { return data.data(); }

    static Mat4 identity() noexcept {
        Mat4 mat{};
        mat(0, 0) = 1.0f;
        mat(1, 1) = 1.0f;
        mat(2, 2) = 1.0f;
        mat(3, 3) = 1.0f;
        return mat;
    }

    Mat4 operator*(const Mat4& other) const noexcept {
        Mat4 result {};
        for (int col = 0; col < 4; ++col) {
            for (int row = 0; row < 4; ++row) {
                float sum = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    sum += (*this)(row, k) * other(k, col);
                }
                result(row, col) = sum;
            }
        }
        return result;
    }

    static Mat4 make_orthographic(float left, float right, float bottom, float top, float near_z, float far_z) noexcept {
        Mat4 mat = Mat4::identity();
        const float rl = right - left;
        const float tb = top - bottom;
        const float fn = far_z - near_z;

        mat(0, 0) = 2.0f / rl;
        mat(1, 1) = 2.0f / tb;
        mat(2, 2) = -2.0f / fn;

        mat(0, 3) = -(right + left) / rl;
        mat(1, 3) = -(top + bottom) / tb;
        mat(2, 3) = -(far_z + near_z) / fn;
        mat(3, 3) = 1.0f;
        return mat;
    }

    inline Mat4 make_translation(float x, float y) noexcept {
        Mat4 mat = Mat4::identity();
        mat(0, 3) = x;
        mat(1, 3) = y;
        return mat;
    }

    inline Mat4 make_scale(float x, float y) noexcept {
        Mat4 mat = Mat4::identity();
        mat(0, 0) = x;
        mat(1, 1) = y;
        return mat;
    }
};

class IProjection {
public:
    virtual ~IProjection() = default;
    virtual void resize(int width, int height) = 0;
    virtual Mat4 get_projection_matrix() const = 0;
};

}
