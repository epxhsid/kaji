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
};



}
