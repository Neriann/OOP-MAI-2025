#include <iostream>

#include "figure_array.hpp"
#include "square.hpp"
#include "rectangle.hpp"
#include "triangle.hpp"

template <typename T>
std::shared_ptr<Figure<T>> create_figure() {
    std::cout << "Choice of figure type: " << std::endl;
    std::cout << "1 - Triangle" << std::endl;
    std::cout << "2 - Rectangle" << std::endl;
    std::cout << "3 - Square" << std::endl;

    std::shared_ptr<Figure<T>> figure;

    std::cout << "<Selected figure type>: ";
    unsigned int code;
    std::cin >> code;
    switch (code) {
    case 1:
        figure = std::make_shared<Triangle<T>>();
        break;
    case 2:
        figure = std::make_shared<Rectangle<T>>();
        break;
    case 3:
        figure = std::make_shared<Square<T>>();
        break;
    default:
        std::cout << "Invalid type, using triangle: " << std::endl;
        figure = std::make_shared<Triangle<T>>();
    }
    std::cin >> *figure;
    return figure;
}


template <typename T>
FigureArray<T> create_array() {
    std::cout << "<Array size>: ";
    int n;
    std::cin >> n;
    if (n < 1) {
        std::cout << "Invalid size, returned Base array" << std::endl;
        return {};
    }

    FigureArray<T> arr(n);
    for (size_t i = 0; i < n; ++i) {
        arr[i] = create_figure<T>();
    }
    return arr;
}

template <typename T>
void handler(const FigureArray<T>& arr) {
    // Демонстрация работы
    std::cout << "Figure array size: " << arr.get_size() << std::endl;
    for (size_t i = 0; i < arr.get_size(); ++i) {
        std::cout << *arr[i] << std::endl;
    }
    std::cout << "Total area: " << arr.total_area() << std::endl;
}


int main() {
    std::cout << "Choice array type:\n";
    std::cout << "1 - int\n";
    std::cout << "2 - long long\n";
    std::cout << "3 - float\n";
    std::cout << "4 - double\n";
    std::cout << "5 - char\n";
    std::cout << "6 - bool\n";

    std::cout << "<Selected array type>: ";

    unsigned int code;
    std::cin >> code;

    // Обрабатываем каждый тип отдельно
    switch (code) {
    case 1: {
        auto arr = create_array<int>();
        handler(arr);
        break;
    }
    case 2: {
        auto arr = create_array<long long>();
        handler(arr);
        break;
    }
    case 3: {
        auto arr = create_array<float>();
        handler(arr);
        break;
    }
    case 4: {
        auto arr = create_array<double>();
        handler(arr);
        break;
    }
    case 5:
    {
        auto arr = create_array<char>();
        handler(arr);
        break;
    }
    case 6:
    {
        auto arr = create_array<bool>();
        handler(arr);
        break;
    }
    default:
        std::cout << "Invalid choice\n";
    }

    return 0;
}
