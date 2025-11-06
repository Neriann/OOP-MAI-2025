#pragma once

#include <cstddef>
#include <memory>

#include "figure.hpp"

enum {
    BASE_CAPACITY = 10
};

template <ScalarType T>
class FigureArray final {
public:
    FigureArray() = default;

    explicit FigureArray(size_t sz, const std::shared_ptr<Figure<T>>& value = nullptr): size(sz), capacity(sz >= BASE_CAPACITY ? sz : BASE_CAPACITY) {
        auto new_figures = std::make_unique<std::shared_ptr<Figure<T>>[]>(capacity);
        if (value != nullptr) {
            for (size_t i = 0; i < size; ++i) {
                new_figures[i] = value->clone();
            }
        }
        figures = std::move(new_figures);
    }

    FigureArray(const FigureArray& other) {
        auto new_figures = std::make_unique<std::shared_ptr<Figure<T>>[]>(other.capacity);
        // deep copy
        for (size_t i = 0; i < other.size; ++i) {
            if (other.figures[i] != nullptr) {
                new_figures[i] = other.figures[i]->clone();
            }
        }
        figures = std::move(new_figures);
        size = other.size;
        capacity = other.capacity;
    }

    FigureArray& operator=(const FigureArray& other) {
        if (this == &other) {
            return *this;
        }
        // copy and swap
        FigureArray tmp(other);
        std::swap(figures, tmp.figures);
        std::swap(size, tmp.size);
        std::swap(capacity, tmp.capacity);
        return *this;
    }

    std::shared_ptr<Figure<T>>& operator[](size_t id) const {
        if (id >= size) throw std::out_of_range("Array index out of range");

        return figures[id];
    }

    size_t get_size() const {
        return size;
    }
    size_t get_capacity() const {
        return capacity;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity) {
            return;
        }
        // strong exception safety
        auto new_figures = std::make_unique<std::shared_ptr<Figure<T>>[]>(new_capacity);
        for (size_t i = 0; i < size; ++i) {
            if (figures[i] != nullptr) {
                new_figures[i] = figures[i]->clone();
            }

        }
        capacity = new_capacity;
        figures = std::move(new_figures);
    }

    // template <typename U>
    void resize(size_t new_size, const std::shared_ptr<Figure<T>>& value = nullptr) {
        if (new_size <= size) {
            size = new_size;
            return;
        }
        if (new_size > capacity) {
            reserve(new_size);
        }
        if (value != nullptr) {
            for (size_t i = size; i < new_size; ++i) {
                figures[i] = value->clone();
            }
        }
        size = new_size;
    }

    T total_area() const {
        T area = 0;
        for (size_t i = 0; i < size; ++i) {
            if (figures[i] != nullptr) {
                area += figures[i]->area();
            }
        }
        return area;
    }

    void remove_figure(size_t id) {
        if (id >= size) {
            throw std::out_of_range("Shapes::RemoveFigure");
        }

        for (size_t i = id + 1; i < size; ++i) {
            figures[i - 1] = std::move(figures[i]);
        }
        --size;
    }

    ~FigureArray() = default;

private:
    size_t size{};
    size_t capacity{};
    std::unique_ptr<std::shared_ptr<Figure<T>>[]> figures = nullptr;
};
