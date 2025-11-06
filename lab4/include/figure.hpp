#pragma once

#include <memory>

#include "point.hpp"

enum {
    TRIANGLE_VERTICES = 3,
    SQUARE_VERTICES = 4,
    RECTANGLE_VERTICES = 4
};

template <ScalarType T>
class Figure
{
public:
    friend std::ostream& operator<<(std::ostream& os, const Figure& figure) {
        figure.print(os);
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Figure& figure) {
        figure.read(is);
        return is;
    }

    size_t get_vertices_count() const {
        return vertices_count;
    }

    Point<T> get_vertex(size_t index) const {
        if (index >= vertices_count) {
            throw std::out_of_range("Figure::get_vertex: index out of range");
        }
        return vertices[index];
    }



    // Полиморфное копирование нужной фигуры
    virtual std::shared_ptr<Figure> clone() const = 0;

    virtual T area() const = 0;

    // базовое сравнение, общее для всех фигур
    virtual bool equals(const Figure& other) const {
        if (vertices_count != other.vertices_count) {
            return false;
        }
        for (size_t i = 0; i < vertices_count; ++i) {
            if (vertices[i].get_x() != other.vertices[i].get_x() || vertices[i].get_y() != other.vertices[i].get_y()) {
                return false;
            }
        }
        return true;
    }

    bool operator==(const Figure& other) const {
        return equals(other);
    }

    // функция получения геометрического центра фигуры (общая для всех)
    Point<T> geometric_center() const {
        T avg_x = 0, avg_y = 0;
        for (size_t i = 0; i < vertices_count; ++i) {
            avg_x += vertices[i].get_x();
            avg_y += vertices[i].get_y();
        }
        return {avg_x / vertices_count, avg_y / vertices_count};
    }

protected:
    virtual void print(std::ostream& os) const = 0;

    virtual void read(std::istream& is) = 0;

    explicit Figure(size_t count) : vertices_count(count)
    {
        vertices = std::make_unique<Point<T>[]>(count);
    }

    Figure(const std::initializer_list<Point<T>>& points, size_t count) {
        if (points.size() != count) {
            throw std::invalid_argument("Figure::Figure(): points.size() != vertices_count");
        }
        vertices_count = count;
        vertices = std::make_unique<Point<T>[]>(vertices_count);
        std::copy(points.begin(), points.end(), vertices.get());
    }

    Figure(const Figure& other): vertices_count(other.vertices_count)
    {
        vertices = std::make_unique<Point<T>[]>(other.vertices_count);
        for (size_t i = 0; i < other.vertices_count; ++i)
        {
            vertices[i] = other.vertices[i];
        }
    }

    Figure& operator=(const Figure& other) {
        if (this == &other) {
            return *this;
        }
        // strong exception safety
        auto new_vertices = std::make_unique<Point<T>[]>(other.vertices_count);
        for (size_t i = 0; i < other.vertices_count; ++i) {
            new_vertices[i] = other.vertices[i];
        }
        vertices = std::move(new_vertices);
        vertices_count = other.vertices_count;
        return *this;
    }

    Figure(Figure&& other) noexcept : vertices(std::move(other.vertices)), vertices_count(other.vertices_count) {
        other.vertices_count = 0;
    }

    Figure& operator=(Figure&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        vertices = std::move(other.vertices);
        vertices_count = other.vertices_count;
        other.vertices_count = 0;
        return *this;
    }

    virtual ~Figure() = default;


    std::unique_ptr<Point<T>[]> vertices = nullptr;
    size_t vertices_count = 0;
};
