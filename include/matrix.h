#pragma once

#include "coord.h"
#include <cstdint>
#include <functional>
#include <tuple>
#include <vector>

template<typename T>
class Matrix {
public:
    Matrix() = default;

    explicit Matrix(const Coord& c) {
        for (uint32_t i = 0; i < c.X; i++) {
            m_matrix.emplace_back(std::vector<T>(c.Y));
        }
    }

    explicit Matrix(const Coord& c, const T& value) {
        for (uint32_t i = 0; i < c.X; i++) {
            m_matrix.emplace_back(std::vector<T>(c.Y, value));
        }
    }

    [[nodiscard]] Coord Size() const {
        return {static_cast<uint32_t>(m_matrix.size()), static_cast<uint32_t>(m_matrix.front().size())};
    }

    [[nodiscard]] T& Get(const Coord& c) {
        return m_matrix[c.X][c.Y];
    }

    [[nodiscard]] const T& Get(const Coord& c) const {
        return m_matrix[c.X][c.Y];
    }

    void ForEach(const std::function<void(const Coord& c, T& item)>& fn) {
        auto dimension = Size();
        for (uint32_t j = 0; j < dimension.Y; j++) {
            for (uint32_t i = 0; i < dimension.X; i++) {
                fn({i, j}, m_matrix[i][j]);
            }
        }
    }

    void ForEach(const std::function<void(const Coord& c, const T& item)>& fn) const {
        auto dimension = Size();
        for (uint32_t j = 0; j < dimension.Y; j++) {
            for (uint32_t i = 0; i < dimension.X; i++) {
                fn({i, j}, m_matrix[i][j]);
            }
        }
    }

private:
    std::vector<std::vector<T>> m_matrix;
};
