#pragma once


#include "figure.hpp"

template <ScalarType T>
class Rectangle : public Figure<T>
{
public:
    Rectangle(): Figure<T>(RECTANGLE_VERTICES) {}

    explicit Rectangle(T length, T width, Point<T> point = {}): Figure<T>(RECTANGLE_VERTICES), length(length), width(width)
    {
        T x = point.get_x(), y = point.get_y();
        this->vertices[0] = point;
        this->vertices[1] = {x + length, y};
        this->vertices[2] = {x, y + width};
        this->vertices[3] = {x + length, y + width};
    }

    Rectangle(T length, T width, const std::initializer_list<Point<T>>& points) : Figure<T>(points, RECTANGLE_VERTICES), length(length), width(width)
    {}

    std::shared_ptr<Figure<T>> clone() const override {
        return std::make_shared<Rectangle>(*this);
    }

    T area() const override
    {
        return length * width;
    }

    explicit operator double() const
    {
        return area();
    }

    bool equals(const Figure<T>& other) const override {
        auto rec = dynamic_cast<const Rectangle&>(other);
        return Figure<T>::equals(other) && rec.length == length && rec.width == width;
    }

    Rectangle(const Rectangle& other): Figure<T>(other), length(other.length), width(other.width) {}

    Rectangle& operator=(const Rectangle& other)
    {
        if (this == &other)
        {
            return *this;
        }
        Figure<T>::operator=(other);
        length = other.length;
        width = other.width;
        return *this;
    }

    Rectangle(Rectangle&& other) noexcept: Figure<T>(std::move(other)), length(other.length), width(other.width)
    {
        other.length = T{};
        other.width = T{};
    }

    Rectangle& operator=(Rectangle&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        Figure<T>::operator=(std::move(other));
        length = other.length;
        other.length = T{};
        width = other.width;
        other.width = T{};
        return *this;
    }

    ~Rectangle() override = default;

private:
    void print(std::ostream& os) const override
    {
        os << "Rectangle info: " << "\n";
        os << "Length: " << length << "\n";
        os << "Width: " << width << "\n";
        os << "Vertices count: " << this->vertices_count << "\n";
        for (size_t i = 0; i < this->vertices_count; ++i)
        {
            os << "Point[" << i << "]: " << this->vertices[i] << "\n";
        }
    }

    void read(std::istream& is) override
    {
        std::cout << "Rectangle info: " << "\n";
        std::cout << "Length: "; is >> length;
        std::cout << "Width: "; is >> width;
        this->vertices_count = RECTANGLE_VERTICES;
        for (size_t i = 0; i < this->vertices_count; ++i)
        {
            std::cout << "Point[" << i << "]: ";
            is >> this->vertices[i];
        }
    }
protected:
    T length{};
    T width{};
};
