#pragma once


#include "figure.hpp"

template <ScalarType T>
class Triangle : public Figure<T>
{
public:
    Triangle(): Figure<T>(TRIANGLE_VERTICES) {}

    // point - середина основания, относительно которой вычисляются оставшиеся точки
    // по умолчанию эта точка лежит в начале координат
    explicit Triangle(T base, T height, Point<T> point = {}): Figure<T>(TRIANGLE_VERTICES), height(height), base(base)
    {
        T dx = base / 2, dy = height;
        T x = point.get_x(), y = point.get_y();

        this->vertices[0] = {x, y + dy};
        this->vertices[1] = {x + dx, y};
        this->vertices[2] = {x - dx, y};
    }

    Triangle(T base, T height, const std::initializer_list<Point<T>>& points) : Figure<T>(points, TRIANGLE_VERTICES), height(height), base(base)
    {
    }

    std::shared_ptr<Figure<T>> clone() const override {
        return std::make_shared<Triangle>(*this);
    }

    T area() const override
    {
        return height * base / 2;
    }

    explicit operator double() const
    {
        return area();
    }

    bool equals(const Figure<T>& other) const override {
        auto rec = dynamic_cast<const Triangle&>(other);
        return Figure<T>::equals(other) && base == rec.base && height == rec.height;
    }

    Triangle(const Triangle& other): Figure<T>(other), height(other.height), base(other.base) {}

    Triangle& operator=(const Triangle& other)
    {
        if (this == &other)
        {
            return *this;
        }
        Figure<T>::operator=(other);
        height = other.height;
        base = other.base;
        return *this;
    }

    Triangle(Triangle&& other) noexcept: Figure<T>(std::move(other)), height(other.height), base(other.base)
    {
        other.height = T{};
        other.base = T{};
    }

    Triangle& operator=(Triangle&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        Figure<T>::operator=(std::move(other));
        height = other.height;
        other.height = T{};
        base = other.base;
        other.base = T{};
        return *this;
    }

    ~Triangle() override = default;

private:
    void print(std::ostream& os) const override
    {
        os << "Triangle info: " << "\n";
        os << "Height: " << height << "\n";
        os << "Base: " << base << "\n";
        os << "Vertices count: " << this->vertices_count << "\n";
        for (size_t i = 0; i < this->vertices_count; ++i)
        {
            os << "Point[" << i << "]: " << this->vertices[i] << "\n";
        }
    }

    void read(std::istream& is) override
    {
        std::cout << "Triangle info: " << "\n";
        std::cout << "Height: "; is >> height;
        std::cout << "Base: "; is >> base;
        this->vertices_count = TRIANGLE_VERTICES;
        for (size_t i = 0; i < this->vertices_count; ++i)
        {
            std::cout << "Point[" << i << "]: ";
            is >> this->vertices[i];
        }
    }
private:
    T base{};
    T height{};
};
