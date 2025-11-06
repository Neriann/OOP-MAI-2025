#pragma once

#include "rectangle.hpp"


template <ScalarType T>
class Square final : public Rectangle<T>
{
public:
    Square(): Rectangle<T>(T{}, T{}) {}

    explicit Square(T side, Point<T> point = {}): Rectangle<T>(side, side, point) {}

    Square(T side, const std::initializer_list<Point<T>>& points) : Rectangle<T>(side, side, points) {}

    std::shared_ptr<Figure<T>> clone() const override {
        return std::make_shared<Square>(*this);
    }

    // наследуем все конструкторы (копирования, перемещения) от прямоугольника
    using Rectangle<T>::Rectangle;

private:
    void print(std::ostream& os) const override
    {
        os << "Square info: " << "\n";
        os << "Side: " << this->length << "\n";
        os << "Vertices count: " << this->vertices_count << "\n";
        for (size_t i = 0; i < this->vertices_count; ++i)
        {
            os << "Point[" << i << "]: " << this->vertices[i] << "\n";
        }
    }

    void read(std::istream& is) override
    {
        std::cout << "Square info: " << "\n";
        std::cout << "Side: ";
        T side;
        is >> side;
        this->length = side;
        this->width = side;
        this->vertices_count = SQUARE_VERTICES;
        for (size_t i = 0; i < this->vertices_count; ++i)
        {
            std::cout << "Point[" << i << "]: ";
            is >> this->vertices[i];
        }
    }
};
