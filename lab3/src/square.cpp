#include "square.hpp"

Square::Square(): Polygon(SQUARE_VERTICES) {}

// дополнительный конструктор для квадрата от левой нижней точки - p и длины стороны - a
Square::Square(double a, Point p): Polygon(SQUARE_VERTICES) {
    points.emplace_back(p);
    points.emplace_back(p.x + a, p.y);
    points.emplace_back(p.x + a, p.y + a);
    points.emplace_back(p.x, p.y + a);
    side = a;
}

Square::Square(const std::initializer_list<Point>& points) : Polygon(points, SQUARE_VERTICES) {}
