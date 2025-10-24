#pragma once

#include <cstddef>

// forward declaration
class Figure;

class Shapes final {
public:
    Shapes() = default;

    explicit Shapes(size_t sz);

    Figure*& operator[](size_t id) const;

    double Area() const;

    void RemoveFigure(size_t id);

    ~Shapes();

private:
    size_t size;
    Figure** figures = nullptr;
};
