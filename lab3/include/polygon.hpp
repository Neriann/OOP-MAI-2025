#pragma once

#include "figure.hpp"

constexpr int TRIANGLE_VERTICES = 3;
constexpr int SQUARE_VERTICES = 4;
constexpr int OCTAGON_VERTICES = 8;


class Polygon : public Figure
{
public:
    explicit Polygon(int n = 0);

    explicit operator double() const override;

    int VertexCount() const override;

    Polygon(const std::initializer_list<Point>& points, int n = 0);

    Polygon(const Polygon& other);

    Polygon(Polygon&& other) noexcept;

    Polygon& operator=(const Polygon& other);

    Polygon& operator=(Polygon&& other) noexcept;

protected:
    int vertex_count{};
};
