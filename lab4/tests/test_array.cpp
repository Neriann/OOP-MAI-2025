#include <gtest/gtest.h>
#include "figure_array.hpp"
#include "triangle.hpp"
#include "rectangle.hpp"
#include "square.hpp"

TEST(FigureArrayTest, DefaultConstructor) {
    FigureArray<double> arr;
    EXPECT_EQ(arr.get_size(), 0);
    EXPECT_EQ(arr.get_capacity(), 0);
}

TEST(FigureArrayTest, SizedConstructor) {
    FigureArray<double> arr(5);
    EXPECT_EQ(arr.get_size(), 5);
    EXPECT_GE(arr.get_capacity(), 5);

    FigureArray<double> arr_small(3); // меньше BASE_CAPACITY
    EXPECT_EQ(arr_small.get_size(), 3);
    EXPECT_EQ(arr_small.get_capacity(), BASE_CAPACITY);
}

TEST(FigureArrayTest, IndexOperator) {
    FigureArray<double> arr(3);

    arr[0] = std::make_shared<Triangle<double>>(4.0, 3.0);
    arr[1] = std::make_shared<Rectangle<double>>(5.0, 3.0);
    arr[2] = std::make_shared<Square<double>>(4.0);

    EXPECT_DOUBLE_EQ(arr[0]->area(), 6.0);
    EXPECT_DOUBLE_EQ(arr[1]->area(), 15.0);
    EXPECT_DOUBLE_EQ(arr[2]->area(), 16.0);
}

TEST(FigureArrayTest, IndexOperatorOutOfRange) {
    FigureArray<double> arr(2);

    EXPECT_NO_THROW(arr[0]);
    EXPECT_NO_THROW(arr[1]);
    EXPECT_THROW(arr[2], std::out_of_range);
    EXPECT_THROW(arr[100], std::out_of_range);
}

TEST(FigureArrayTest, CopyConstructor) {
    FigureArray<double> original(2);
    original[0] = std::make_shared<Triangle<double>>(4.0, 3.0);
    original[1] = std::make_shared<Square<double>>(4.0);

    FigureArray<double> copy(original);

    EXPECT_EQ(original.get_size(), copy.get_size());
    EXPECT_DOUBLE_EQ(original.total_area(), copy.total_area());

    // Глубокое копирование
    EXPECT_NE(original[0].get(), copy[0].get());
    EXPECT_TRUE(*original[0] == *copy[0]);
}

TEST(FigureArrayTest, CopyAssignment) {
    FigureArray<double> arr1(2);
    arr1[0] = std::make_shared<Triangle<double>>(4.0, 3.0);

    FigureArray<double> arr2(1);
    arr2[0] = std::make_shared<Square<double>>(4.0);

    arr2 = arr1;

    EXPECT_EQ(arr1.get_size(), arr2.get_size());
    EXPECT_DOUBLE_EQ(arr1.total_area(), arr2.total_area());
    EXPECT_TRUE(*arr1[0] == *arr2[0]);
}

TEST(FigureArrayTest, SelfAssignment) {
    FigureArray<double> arr(2);
    arr[0] = std::make_shared<Triangle<double>>(4.0, 3.0);
    arr[1] = std::make_shared<Square<double>>(4.0);

    double original_area = arr.total_area();
    size_t original_size = arr.get_size();

    arr = arr; // self-assignment

    EXPECT_EQ(arr.get_size(), original_size);
    EXPECT_DOUBLE_EQ(arr.total_area(), original_area);
}

TEST(FigureArrayTest, ReserveIncreaseCapacity) {
    FigureArray<double> arr;
    EXPECT_EQ(arr.get_capacity(), 0);

    arr.reserve(10);

    EXPECT_EQ(arr.get_capacity(), 10);
    EXPECT_EQ(arr.get_size(), 0); // размер не должен измениться
}

TEST(FigureArrayTest, ReserveNoEffectWhenSmaller) {
    FigureArray<double> arr;
    size_t initial_capacity = arr.get_capacity();

    arr.reserve(initial_capacity / 2); // пытаемся уменьшить

    EXPECT_EQ(arr.get_capacity(), initial_capacity); // не должно измениться
}

TEST(FigureArrayTest, ReservePreservesData) {
    FigureArray<double> arr(3);
    arr[0] = std::make_shared<Triangle<double>>(4.0, 3.0);
    arr[1] = std::make_shared<Rectangle<double>>(5.0, 3.0);
    arr[2] = std::make_shared<Square<double>>(4.0);

    double original_area = arr.total_area();
    size_t original_size = arr.get_size();

    arr.reserve(arr.get_capacity() * 2);

    EXPECT_EQ(arr.get_size(), original_size);
    EXPECT_DOUBLE_EQ(arr.total_area(), original_area);

    // Данные должны сохраниться
    EXPECT_DOUBLE_EQ(arr[0]->area(), 6.0);
    EXPECT_DOUBLE_EQ(arr[1]->area(), 15.0);
    EXPECT_DOUBLE_EQ(arr[2]->area(), 16.0);
}

TEST(FigureArrayTest, ResizeIncrease) {
    FigureArray<double> arr(2);
    arr[0] = std::make_shared<Triangle<double>>(4.0, 3.0);
    arr[1] = std::make_shared<Square<double>>(4.0);

    auto default_figure = std::make_shared<Rectangle<double>>(2.0, 2.0);
    arr.resize(5, default_figure);

    EXPECT_EQ(arr.get_size(), 5);
    EXPECT_DOUBLE_EQ(arr[0]->area(), 6.0);  // оригинал
    EXPECT_DOUBLE_EQ(arr[1]->area(), 16.0); // оригинал
    EXPECT_DOUBLE_EQ(arr[2]->area(), 4.0);  // новый (rectangle 2x2)
    EXPECT_DOUBLE_EQ(arr[3]->area(), 4.0);  // новый
    EXPECT_DOUBLE_EQ(arr[4]->area(), 4.0);  // новый
}

TEST(FigureArrayTest, ResizeDecrease) {
    FigureArray<double> arr(3);
    arr[0] = std::make_shared<Triangle<double>>(4.0, 3.0);
    arr[1] = std::make_shared<Rectangle<double>>(5.0, 3.0);
    arr[2] = std::make_shared<Square<double>>(4.0);

    arr.resize(1, nullptr);

    EXPECT_EQ(arr.get_size(), 1);
    EXPECT_DOUBLE_EQ(arr[0]->area(), 6.0); // только первый элемент остался
}

TEST(FigureArrayTest, ResizeWithMove) {
    FigureArray<double> arr(2);

    auto temp_figure = std::make_shared<Triangle<double>>(6.0, 4.0);
    arr.resize(4, std::move(temp_figure));

    EXPECT_EQ(arr.get_size(), 4);
    EXPECT_DOUBLE_EQ(arr[2]->area(), 12.0); // новые элементы
    EXPECT_DOUBLE_EQ(arr[3]->area(), 12.0);

    // temp_figure должен быть перемещен
    EXPECT_EQ(temp_figure, nullptr);
}

TEST(FigureArrayTest, TotalArea) {
    FigureArray<double> arr(3);
    arr[0] = std::make_shared<Triangle<double>>(4.0, 3.0);  // 6.0
    arr[1] = std::make_shared<Rectangle<double>>(5.0, 3.0); // 15.0
    arr[2] = std::make_shared<Square<double>>(4.0);         // 16.0

    EXPECT_DOUBLE_EQ(arr.total_area(), 37.0);
}

TEST(FigureArrayTest, TotalAreaWithNullptr) {
    FigureArray<double> arr(3);
    arr[0] = std::make_shared<Triangle<double>>(4.0, 3.0); // 6.0
    arr[1] = nullptr; // nullptr должен игнорироваться
    arr[2] = std::make_shared<Square<double>>(4.0);        // 16.0

    EXPECT_DOUBLE_EQ(arr.total_area(), 22.0); // 6 + 16
}

TEST(FigureArrayTest, RemoveFigure) {
    FigureArray<double> arr(3);
    arr[0] = std::make_shared<Triangle<double>>(4.0, 3.0);
    arr[1] = std::make_shared<Rectangle<double>>(5.0, 3.0);
    arr[2] = std::make_shared<Square<double>>(4.0);

    arr.remove_figure(1); // удаляем прямоугольник

    EXPECT_EQ(arr.get_size(), 2);
    EXPECT_DOUBLE_EQ(arr[0]->area(), 6.0);  // треугольник остался
    EXPECT_DOUBLE_EQ(arr[1]->area(), 16.0); // квадрат сдвинулся на позицию 1
}

TEST(FigureArrayTest, RemoveFigureOutOfRange) {
    FigureArray<double> arr(2);

    EXPECT_THROW(arr.remove_figure(2), std::out_of_range);
    EXPECT_THROW(arr.remove_figure(100), std::out_of_range);
}

TEST(FigureArrayTest, RemoveFigureFromEmpty) {
    FigureArray<double> arr; // size = 0

    EXPECT_THROW(arr.remove_figure(0), std::out_of_range);
}

TEST(FigureArrayTest, ComplexScenario) {
    FigureArray<double> arr;

    // Начинаем с пустого массива
    EXPECT_EQ(arr.get_size(), 0);

    // Резервируем память
    arr.reserve(5);
    EXPECT_GE(arr.get_capacity(), 5);

    // Изменяем размер и заполняем
    arr.resize(3, std::make_shared<Triangle<double>>(2.0, 1.0));
    EXPECT_EQ(arr.get_size(), 3);
    EXPECT_DOUBLE_EQ(arr.total_area(), 3.0); // 3 треугольника по 1.0

    // Модифицируем элементы
    arr[1] = std::make_shared<Square<double>>(3.0);
    EXPECT_DOUBLE_EQ(arr.total_area(), 1.0 + 9.0 + 1.0); // 11.0

    // Удаляем элемент
    arr.remove_figure(0);
    EXPECT_EQ(arr.get_size(), 2);
    EXPECT_DOUBLE_EQ(arr.total_area(), 9.0 + 1.0); // 10.0

    // Копируем массив
    FigureArray<double> copy = arr;
    EXPECT_EQ(copy.get_size(), 2);
    EXPECT_DOUBLE_EQ(copy.total_area(), 10.0);

    // Проверяем глубокое копирование
    EXPECT_NE(arr[0].get(), copy[0].get());
    EXPECT_TRUE(*arr[0] == *copy[0]);
}
