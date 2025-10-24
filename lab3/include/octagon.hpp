#pragma once

#include "polygon.hpp"

class Octagon final: public Polygon {
public:
    Octagon();

    Octagon(const std::initializer_list<Point>& points);

    ~Octagon() = default;

};
