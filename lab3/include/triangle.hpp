#pragma once

#include "polygon.hpp"

class Triangle final: public Polygon
{
public:
    Triangle();

    Triangle(const std::initializer_list<Point>& points);

    ~Triangle() = default;
};
