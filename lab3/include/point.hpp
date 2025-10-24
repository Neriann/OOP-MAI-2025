#pragma once

struct Point
{
    Point() = default;

    Point(double x, double y);

    // static чтобы мог вызываться вне зависимости от существования объекта
    static double Distance(const Point& p, const Point& q);

    double x{};
    double y{};
};
