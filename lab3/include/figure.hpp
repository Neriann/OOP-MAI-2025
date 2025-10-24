#pragma once

#include <vector>
#include <iostream>

#include "point.hpp"

class Figure
{
    friend std::ostream& operator<<(std::ostream& os, const Figure& figure);
    friend std::istream& operator>>(std::istream& is, Figure& figure);

public:
    explicit virtual operator double() const = 0;

    virtual int VertexCount() const = 0;

    bool operator==(const Figure& other) const;

    Point Center() const;

    void SortPoints(double eps = 1e-6);

    bool AllSidesEqual(double eps = 1e-6) const;

    virtual ~Figure() = default;

protected:
    double side{};
    std::vector<Point> points;
};
