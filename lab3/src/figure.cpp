#include <cmath>

#include "figure.hpp"


void Figure::SortPoints(double eps) {
    auto [cx, cy] = Center();
    // сортируем точки по углу относительно центра
    std::sort(points.begin(), points.end(), [cx, cy, eps](const Point& a, const Point& b) {
        double AngleA = atan2(a.y - cy, a.x - cx);
        double AngleB = atan2(b.y - cy, b.x - cx);
        return AngleA - AngleB < eps;
    });
}

bool Figure::AllSidesEqual(double eps) const {
    size_t size = points.size();
    double len = Point::Distance(points[0], points[1]);
    for (size_t i = 2; i < size; ++i) {
        if (std::abs(len - Point::Distance(points[i - 1], points[i])) > eps) {
            return false;
        }
    }
    if (std::abs(len - Point::Distance(points[size - 1], points[0])) > eps) {
        return false;
    }
    return true;
}


Point Figure::Center() const {
    double avg_x = 0, avg_y = 0;
    for (const auto& point : points) {
        avg_x += point.x;
        avg_y += point.y;
    }
    size_t sz = points.size();
    return {avg_x / sz, avg_y / sz};
}

std::ostream& operator<<(std::ostream& os, const Figure& figure) {
    for (const auto& elem : figure.points) {
        os << elem.x << ' ' << elem.y << '\n';
    }
    return os;
}

std::istream& operator>>(std::istream& is, Figure& figure) {
    int size = figure.VertexCount();
    figure.points.resize(size);
    for (size_t i = 0; i < size; ++i) {
        is >> figure.points[i].x >> figure.points[i].y;
    }
    figure.SortPoints();
    if (!figure.AllSidesEqual()) {
        figure.points.clear();
        throw std::invalid_argument("The sides must be equal");
    }
    figure.side = Point::Distance(figure.points[0], figure.points[1]);
    return is;
}

bool Figure::operator==(const Figure& other) const {
    size_t size = points.size(), other_size = other.points.size();
    if (size != other_size) {
        return false;
    }
    for (size_t i = 0; i < size; ++i) {
        if (points[i].x != other.points[i].x || points[i].y != other.points[i].y) {
            return false;
        }
    }
    return true;
}
