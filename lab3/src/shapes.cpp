#include "shapes.hpp"
#include "figure.hpp"

Shapes::Shapes(size_t sz): size(sz), figures(new Figure*[size]) {}

Figure*& Shapes::operator[](size_t id) const {
    return figures[id];
}

double Shapes::Area() const {
    double area = 0;
    for (size_t i = 0; i < size; ++i) {
        if (figures[i] != nullptr) {
            area += static_cast<double>(*figures[i]);
        }
    }
    return area;
}

void Shapes::RemoveFigure(size_t id) {
    if (id >= size) {
        throw std::out_of_range("Shapes::RemoveFigure");
    }

    // не нужен delete так, как все фигуры базируются на векторе
    // delete figures[id];
    for (size_t i = id + 1; i < size; ++i) {
        figures[i - 1] = figures[i];
    }
    figures[size - 1] = nullptr;
    --size;
}

Shapes::~Shapes() {
    delete[] figures;
    figures = nullptr;
    size = 0;
}
