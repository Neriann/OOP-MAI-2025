#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>
#include <sstream>

#include "triangle.hpp"
#include "square.hpp"
#include "octagon.hpp"

// Вспомогательные функции для сравнения точек
bool PointsEqual(const Point& p1, const Point& p2, double eps = 1e-12) {
    return std::abs(p1.x - p2.x) < eps && std::abs(p1.y - p2.y) < eps;
}

// Тесты для класса Triangle
TEST(TriangleTest, DefaultConstructor) {
    Triangle t;
    EXPECT_EQ(t.VertexCount(), 3);
}

TEST(TriangleTest, ValidTriangleCreation) {
    // Правильный равносторонний треугольник
    EXPECT_NO_THROW({
        Triangle t({{0, 0}, {1, 0}, {0.5, std::sqrt(3)/2}});
        EXPECT_EQ(t.VertexCount(), 3);
        EXPECT_TRUE(t.AllSidesEqual());
    });
}

TEST(TriangleTest, InvalidTriangleThrowsException) {
    // Треугольник с неравными сторонами должен кидать исключение
    EXPECT_THROW(
        Triangle t({{0, 0}, {3, 0}, {0, 4}}), // Разносторонний треугольник
        std::invalid_argument
    );
}

TEST(TriangleTest, WrongNumberOfPointsThrowsException) {
    // Слишком мало точек
    EXPECT_THROW(
        Triangle t({{0, 0}, {1, 0}}),
        std::invalid_argument
    );

    // Слишком много точек
    EXPECT_THROW(
        Triangle t({{0, 0}, {1, 0}, {0.5, 1}, {0.5, 0.5}}),
        std::invalid_argument
    );
}

TEST(TriangleTest, CenterCalculation) {
    // Равносторонний треугольник
    Triangle t{{0, 0}, {2, 0}, {1, std::sqrt(3)}};
    Point center = t.Center();
    EXPECT_DOUBLE_EQ(center.x, 1.0);
    EXPECT_NEAR(center.y, std::sqrt(3)/3, 1e-12);
}

TEST(TriangleTest, AreaCalculation) {
    // Равносторонний треугольник со стороной 2
    Triangle t{{0, 0}, {2, 0}, {1, std::sqrt(3)}};
    double area = static_cast<double>(t);
    double expected_area = std::sqrt(3); // (2² * √3) / 4 = √3
    EXPECT_NEAR(area, expected_area, 1e-12);
}

TEST(TriangleTest, AllSidesEqual) {
    Triangle equilateral{{0, 0}, {1, 0}, {0.5, std::sqrt(3)/2}};
    EXPECT_TRUE(equilateral.AllSidesEqual(1e-12));
}

TEST(TriangleTest, EqualityOperator) {
     Triangle t1{{0, 0}, {1, 0}, {0.5, std::sqrt(3)/2}};
     Triangle t2{{0, 0}, {1, 0}, {0.5, std::sqrt(3)/2}};
     Triangle t3{{0, 0}, {2, 0}, {1, std::sqrt(3)}};

     EXPECT_TRUE(t1 == t2);
     EXPECT_FALSE(t1 == t3);
}

// Тесты для класса Square
TEST(SquareTest, DefaultConstructor) {
     Square s;
     EXPECT_EQ(s.VertexCount(), 4);
}

TEST(SquareTest, ValidSquareInitializerListConstructor) {
     EXPECT_NO_THROW({
         Square s({{0, 0}, {2, 0}, {2, 2}, {0, 2}});
         EXPECT_EQ(s.VertexCount(), 4);
         EXPECT_TRUE(s.AllSidesEqual());
     });
}

TEST(SquareTest, ValidSquareParameterizedConstructor) {
    EXPECT_NO_THROW({
        Square s(2.0, {1, 1});
        EXPECT_EQ(s.VertexCount(), 4);
        EXPECT_TRUE(s.AllSidesEqual());

        Point center = s.Center();
        EXPECT_DOUBLE_EQ(center.x, 2.0);
        EXPECT_DOUBLE_EQ(center.y, 2.0);
    });
}

TEST(SquareTest, InvalidSquareThrowsException) {
    // Не квадрат (разные стороны)
    EXPECT_THROW(
        Square s({{0, 0}, {3, 0}, {3, 2}, {0, 2}}), // Прямоугольник 3x2
        std::invalid_argument
    );
}

TEST(SquareTest, WrongNumberOfPointsThrowsException) {
    // Слишком мало точек
    EXPECT_THROW(
        Square s({{0, 0}, {1, 0}, {1, 1}}),
        std::invalid_argument
    );

    // Слишком много точек
    EXPECT_THROW(
        Square s({{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0.5, 0.5}}),
        std::invalid_argument
    );
}

TEST(SquareTest, AreaCalculation) {
    Square s(3.0, {0, 0});
    double area = static_cast<double>(s);
    EXPECT_DOUBLE_EQ(area, 9.0); // 3 * 3 = 9
}

TEST(SquareTest, AllSidesEqual) {
    Square s{{0, 0}, {2, 0}, {2, 2}, {0, 2}};
    EXPECT_TRUE(s.AllSidesEqual(1e-12));
}

// Тесты для класса Octagon
TEST(OctagonTest, DefaultConstructor) {
    Octagon o;
    EXPECT_EQ(o.VertexCount(), 8);
}

TEST(OctagonTest, ValidOctagonCreation) {
    EXPECT_NO_THROW({
        // Правильный восьмиугольник
        Octagon o({
            {1, 0},
            {std::sqrt(2)/2, std::sqrt(2)/2},
            {0, 1},
            {-std::sqrt(2)/2, std::sqrt(2)/2},
            {-1, 0},
            {-std::sqrt(2)/2, -std::sqrt(2)/2},
            {0, -1},
            {std::sqrt(2)/2, -std::sqrt(2)/2}
        });
        EXPECT_EQ(o.VertexCount(), 8);
        EXPECT_TRUE(o.AllSidesEqual(1e-10));
    });
}

TEST(OctagonTest, InvalidOctagonThrowsException) {
    EXPECT_THROW(
        // Неправильный восьмиугольник (разные стороны)
        Octagon o({
            {1, 0},
            {0.8, 0.6},  // Измененная точка - стороны не равны
            {0, 1},
            {-std::sqrt(2)/2, std::sqrt(2)/2},
            {-1, 0},
            {-std::sqrt(2)/2, -std::sqrt(2)/2},
            {0, -1},
            {std::sqrt(2)/2, -std::sqrt(2)/2}
        }),
        std::invalid_argument
    );
}

TEST(OctagonTest, WrongNumberOfPointsThrowsException) {
    // Слишком мало точек
    EXPECT_THROW(
        Octagon o({{0, 0}, {1, 0}, {1, 1}, {0, 1}}), // 4 точки
        std::invalid_argument
    );

    // Слишком много точек
    EXPECT_THROW(
        Octagon o({
            {1, 0}, {std::sqrt(2)/2, std::sqrt(2)/2}, {0, 1},
            {-std::sqrt(2)/2, std::sqrt(2)/2}, {-1, 0},
            {-std::sqrt(2)/2, -std::sqrt(2)/2}, {0, -1},
            {std::sqrt(2)/2, -std::sqrt(2)/2}, {0.5, 0.5} // 9 точек
        }),
        std::invalid_argument
    );
}

TEST(OctagonTest, CenterCalculation) {
    // Правильный восьмиугольник
    Octagon o({
        {1, 0},
        {std::sqrt(2)/2, std::sqrt(2)/2},
        {0, 1},
        {-std::sqrt(2)/2, std::sqrt(2)/2},
        {-1, 0},
        {-std::sqrt(2)/2, -std::sqrt(2)/2},
        {0, -1},
        {std::sqrt(2)/2, -std::sqrt(2)/2}
    });
    Point center = o.Center();
    EXPECT_NEAR(center.x, 0.0, 1e-12);
    EXPECT_NEAR(center.y, 0.0, 1e-12);
}

TEST(OctagonTest, AreaCalculation) {
     // Правильный восьмиугольник со стороной 1
     double side = hypot(1 - std::sqrt(2)/2, std::sqrt(2)/2);
     double expected_area = 2 * (1 + std::sqrt(2)) * side * side;

     Octagon o({
         {1, 0},
         {std::sqrt(2)/2, std::sqrt(2)/2},
         {0, 1},
         {-std::sqrt(2)/2, std::sqrt(2)/2},
         {-1, 0},
         {-std::sqrt(2)/2, -std::sqrt(2)/2},
         {0, -1},
         {std::sqrt(2)/2, -std::sqrt(2)/2}
     });
     double calculated_area = static_cast<double>(o);
     EXPECT_NEAR(calculated_area, expected_area, 1e-12);
}

// Тесты для операторов ввода/вывода
TEST(IOStreamTest, TriangleOutput) {
     Triangle t{{0, 0}, {1, 0}, {0.5, std::sqrt(3)/2}};
     std::ostringstream oss;
     oss << t;
     std::string output = oss.str();

     // Проверяем, что вывод содержит координаты точек
     EXPECT_TRUE(output.find("0 0") != std::string::npos);
     EXPECT_TRUE(output.find("1 0") != std::string::npos);
}

TEST(IOStreamTest, TriangleInput) {
     std::istringstream iss("0 0\n1 0\n0.5 0.8660254\n");
     Triangle t;

     EXPECT_NO_THROW({
         iss >> t;
     });
    EXPECT_TRUE(t.AllSidesEqual(1e-6));
}

TEST(IOStreamTest, InvalidTriangleInputThrows) {
     std::istringstream iss("0 0\n3 0\n0 4\n"); // Разносторонний треугольник
     Triangle t;

     EXPECT_THROW(
         iss >> t,
         std::invalid_argument
     );
}


// Тесты на особые случаи
TEST(EdgeCasesTest, DegenerateTriangleThrows) {
     // Вырожденный треугольник (все точки на одной линии)
     EXPECT_THROW(
         Triangle t({{0, 0}, {1, 1}, {2, 2}}),
         std::invalid_argument
     );
}

TEST(EdgeCasesTest, CopyConstructor) {
     Triangle t1{{0, 0}, {1, 0}, {0.5, std::sqrt(3)/2}};
     Triangle t2(t1);
     EXPECT_EQ(t2.VertexCount(), 3);
     EXPECT_TRUE(t1 == t2);
}

TEST(EdgeCasesTest, MoveConstructor) {
     Triangle t1{{0, 0}, {1, 0}, {0.5, std::sqrt(3)/2}};
     Triangle t2(std::move(t1));
     EXPECT_EQ(t2.VertexCount(), 3);
}

TEST(EdgeCasesTest, AssignmentOperator) {
    Triangle t1{{0, 0}, {1, 0}, {0.5, std::sqrt(3)/2}};
    Triangle t2;
    t2 = t1;
    EXPECT_TRUE(t1 == t2);
}
