#pragma once

#include "polygon.hpp"


class Square final: public Polygon
{
public:
    Square();

    Square(double a, Point p);

    Square(const std::initializer_list<Point>& points);
    ~Square() = default;
};
