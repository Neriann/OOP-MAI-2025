#include "triangle.hpp"

Triangle::Triangle(): Polygon(TRIANGLE_VERTICES) {}

Triangle::Triangle(const std::initializer_list<Point>& points) : Polygon(points, TRIANGLE_VERTICES) {}
