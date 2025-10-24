#include <iostream>

#include "octagon.hpp"
#include "square.hpp"
#include "triangle.hpp"

#include "shapes.hpp"

int main() {
    Shapes arr(3);

    Square square;
    std::cin >> square;
    arr[0] = &square;

    Triangle triangle;
    std::cin >> triangle;
    arr[1] = &triangle;

    Octagon octagon;
    std::cin >> octagon;
    arr[2] = &octagon;

    std::cout << "Square Points:\n" << *arr[0] << "Square Area: " << static_cast<double>(*arr[0]) << std::endl;
    std::cout << "Triangle Points:\n" << *arr[1] << "Triangle Area: " << static_cast<double>(*arr[1]) << std::endl;
    std::cout << "Octagon Points:\n" << *arr[2] << "Octagon Area: " << static_cast<double>(*arr[2]) << std::endl;
    std::cout << "Summary Area: " << arr.Area() << std::endl;
    arr.RemoveFigure(2);
    std::cout << "Summary Area before remove Octagon: " << arr.Area() << std::endl;
    return 0;
}
