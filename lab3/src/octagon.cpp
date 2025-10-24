#include "octagon.hpp"

Octagon::Octagon(): Polygon(OCTAGON_VERTICES) {}

Octagon::Octagon(const std::initializer_list<Point>& points) : Polygon(points, OCTAGON_VERTICES) {}
