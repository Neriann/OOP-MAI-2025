#include <cmath>

#include "polygon.hpp"

int Polygon::VertexCount() const {
    return vertex_count;
}

Polygon::Polygon(int n): vertex_count(n) {}

Polygon::Polygon(const std::initializer_list<Point>& p, int n) {
    if (p.size() != n) {
        throw std::invalid_argument("The size of the std::initializer_list must be equal vertex_count");
    }
    vertex_count = n;
    for (const auto& elem : p) {
        points.emplace_back(elem);
    }
    SortPoints();
    if (!AllSidesEqual()) {
        vertex_count = 0;
        points.clear();
        throw std::invalid_argument("The sides must be equal");
    }
    side = Point::Distance(points[0], points[1]);
}

Polygon::Polygon(const Polygon& other) {
    if (this != &other) {
        side = other.side;
        points = other.points;
        vertex_count = other.vertex_count;
    }
}

Polygon::Polygon(Polygon&& other) noexcept {
    side = other.side;
    points = std::move(other.points);
    vertex_count = other.vertex_count;
    other.side = 0;
    other.points.clear();
    other.vertex_count = 0;
}

Polygon& Polygon::operator=(const Polygon& other) {
    Polygon copy = other;
    // идиома copy and swap
    std::swap(side, copy.side);
    std::swap(points, copy.points);
    std::swap(vertex_count, copy.vertex_count);
    return *this;
}

Polygon& Polygon::operator=(Polygon&& other) noexcept {
    if (this != &other) {
        side = other.side;
        points = std::move(other.points);
        vertex_count = other.vertex_count;
        other.side = 0;
        other.vertex_count = 0;
        other.points.clear();
    }
    return *this;
}

Polygon::operator double() const {
    if (vertex_count == TRIANGLE_VERTICES) {
        double p = 3 * side / 2;
        return std::sqrt(p * (p - side) * (p - side) * (p - side));
    }
    if (vertex_count == SQUARE_VERTICES) return side * side;
    if (vertex_count == OCTAGON_VERTICES) return 2 * side * side * (1 + std::sqrt(2));

    return 0;
}
