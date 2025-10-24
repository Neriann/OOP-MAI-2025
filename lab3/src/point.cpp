#include <cmath>

#include "point.hpp"

Point::Point(double x, double y): x(x), y(y) {}

double Point::Distance(const Point& p, const Point& q) {
    return hypot(p.x - q.x, p.y - q.y);
}
