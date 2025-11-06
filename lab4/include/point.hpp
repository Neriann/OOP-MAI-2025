#pragma once

#include <type_traits>
#include <iostream>


template <typename T>
concept ScalarType = std::is_scalar_v<T>;

template <ScalarType T>
struct Point {
    Point() = default;

    Point(T x, T y): x(x), y(y) {}

    Point(const Point&) = default;

    Point& operator=(const Point&) = default;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    T get_x() const {
        return x;
    }
    T get_y() const {
        return y;
    }

    void set_x(T x) {
        this->x = x;
    }

    void set_y(T y) {
        this->y = y;
    }


private:
    T x{}, y{};
};


template <ScalarType T>
Point<T> operator+(const Point<T>& a, const Point<T>& b) {
    return {a.get_x() + b.get_x(), a.get_y() + b.get_y()};
}

template <ScalarType T>
std::ostream& operator<<(std::ostream& os, const Point<T>& p) {
    os << "( " << p.get_x() << ", " << p.get_y() << " )";
    return os;
}

template <ScalarType T>
std::istream& operator>>(std::istream& is, Point<T>& p) {
    T x, y;
    is >> x >> y;
    p.set_x(x); p.set_y(y);
    return is;
}
