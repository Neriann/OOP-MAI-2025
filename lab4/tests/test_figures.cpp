#include <gtest/gtest.h>

#include "figure_array.hpp"
#include "point.hpp"
#include "triangle.hpp"
#include "rectangle.hpp"
#include "square.hpp"

// Тесты для Point
TEST(PointTest, DefaultConstructor) {
    Point<int> p;
    EXPECT_EQ(p.get_x(), 0);
    EXPECT_EQ(p.get_y(), 0);
}

TEST(PointTest, ParameterizedConstructor) {
    Point<double> p(3.5, 2.1);
    EXPECT_DOUBLE_EQ(p.get_x(), 3.5);
    EXPECT_DOUBLE_EQ(p.get_y(), 2.1);
}

TEST(PointTest, ComparisonOperators) {
    Point<int> p1(1, 2);
    Point<int> p2(1, 2);
    Point<int> p3(3, 4);

    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 == p3);
}

// Тесты для Triangle
TEST(TriangleTest, ConstructorAndArea) {
    Triangle<double> t(4.0, 3.0);
    EXPECT_DOUBLE_EQ(t.area(), 6.0); // (4 * 3) / 2 = 6
}

TEST(TriangleTest, CopyConstructor) {
    Triangle<double> original(4.0, 3.0);
    Triangle<double> copy(original);

    EXPECT_DOUBLE_EQ(original.area(), copy.area());
    EXPECT_TRUE(original == copy);
}

TEST(TriangleTest, MoveConstructor) {
    Triangle<double> original(4.0, 3.0);
    double original_area = original.area();

    Triangle<double> moved(std::move(original));

    EXPECT_DOUBLE_EQ(moved.area(), original_area);
}

TEST(TriangleTest, Clone) {
    auto original = std::make_shared<Triangle<double>>(4.0, 3.0);
    auto clone = original->clone();

    EXPECT_DOUBLE_EQ(original->area(), clone->area());
    EXPECT_TRUE(*original == *clone);
    EXPECT_NE(original.get(), clone.get()); // разные объекты
}

TEST(TriangleTest, GeometricCenter) {
    Triangle<double> t(4.0, 3.0);
    Point<double> center = t.geometric_center();

    // Для равнобедренного треугольника с основанием 4 и высотой 3
    // Центр должен быть в (0, 1) относительно начала координат
    EXPECT_DOUBLE_EQ(center.get_x(), 0.0);
    EXPECT_DOUBLE_EQ(center.get_y(), 1.0);
}

// Тесты для Rectangle
TEST(RectangleTest, ConstructorAndArea) {
    Rectangle<double> r(5.0, 3.0);
    EXPECT_DOUBLE_EQ(r.area(), 15.0); // 5 * 3 = 15
}

TEST(RectangleTest, SquareCase) {
    Rectangle<double> r(4.0, 4.0); // квадрат
    EXPECT_DOUBLE_EQ(r.area(), 16.0);
}

TEST(RectangleTest, Comparison) {
    Rectangle<double> r1(5.0, 3.0);
    Rectangle<double> r2(5.0, 3.0);
    Rectangle<double> r3(4.0, 4.0);

    EXPECT_TRUE(r1 == r2);
    EXPECT_FALSE(r1 == r3);
}

TEST(RectangleTest, Clone) {
    auto original = std::make_shared<Rectangle<double>>(5.0, 3.0);
    auto clone = original->clone();

    EXPECT_DOUBLE_EQ(original->area(), clone->area());
    EXPECT_TRUE(*original == *clone);
}

// Тесты для Square
TEST(SquareTest, ConstructorAndArea) {
    Square<double> s(4.0);
    EXPECT_DOUBLE_EQ(s.area(), 16.0); // 4 * 4 = 16
}

TEST(SquareTest, Inheritance) {
    Square<double> s(4.0);
    // Проверяем, что Square действительно наследуется от Rectangle
    EXPECT_DOUBLE_EQ(s.area(), 16.0);
}

TEST(SquareTest, Clone) {
    auto original = std::make_shared<Square<double>>(4.0);
    auto clone = original->clone();

    EXPECT_DOUBLE_EQ(original->area(), clone->area());
    EXPECT_TRUE(*original == *clone);
}

// Тесты полиморфного поведения
TEST(PolymorphismTest, FigureBaseClass) {
    std::shared_ptr<Figure<double>> triangle = std::make_shared<Triangle<double>>(4.0, 3.0);
    std::shared_ptr<Figure<double>> rectangle = std::make_shared<Rectangle<double>>(5.0, 3.0);
    std::shared_ptr<Figure<double>> square = std::make_shared<Square<double>>(4.0);

    EXPECT_DOUBLE_EQ(triangle->area(), 6.0);
    EXPECT_DOUBLE_EQ(rectangle->area(), 15.0);
    EXPECT_DOUBLE_EQ(square->area(), 16.0);
}

TEST(PolymorphismTest, ClonePolymorphism) {
    std::vector<std::shared_ptr<Figure<double>>> figures;
    figures.push_back(std::make_shared<Triangle<double>>(4.0, 3.0));
    figures.push_back(std::make_shared<Rectangle<double>>(5.0, 3.0));
    figures.push_back(std::make_shared<Square<double>>(4.0));

    std::vector<std::shared_ptr<Figure<double>>> clones;
    for (const auto& figure : figures) {
        clones.push_back(figure->clone());
    }

    // Проверяем, что клоны имеют те же площади
    for (size_t i = 0; i < figures.size(); ++i) {
        EXPECT_DOUBLE_EQ(figures[i]->area(), clones[i]->area());
        EXPECT_TRUE(*figures[i] == *clones[i]);
        // Должны быть разные указатели
        EXPECT_NE(figures[i], clones[i]);
    }

    // Освобождаем массив фигур
    for (size_t i = 0; i < figures.size(); ++i) {
        figures[i].reset();
    }
    // Проверяем, что копии по прежнему валидны
    for (size_t i = 0; i < clones.size(); ++i) {
        EXPECT_TRUE(clones[i]->get_vertices_count() > 0);
        EXPECT_TRUE(clones[i]->area() > 0);
    }
}

// Тесты операций ввода/вывода
TEST(IOTest, PointOutput) {
    Point<double> p(3.5, 2.1);
    std::stringstream ss;
    ss << p;
    EXPECT_FALSE(ss.str().empty());
}

TEST(IOTest, FigureOutput) {
    Triangle<double> t(4.0, 3.0);
    std::stringstream ss;
    ss << t;
    EXPECT_FALSE(ss.str().empty());
    EXPECT_NE(ss.str().find("Triangle"), std::string::npos);
}

// Тесты конструкторов с точками
TEST(PointConstructorTest, TriangleWithCustomPoint) {
    Point<double> center(10.0, 20.0);
    Triangle<double> t(4.0, 3.0, center);

    // Проверяем, что вершины правильно смещены
    // Для равнобедренного треугольника с основанием 4 и высотой 3
    // относительно точки (10, 20) вершины должны быть:
    // (10, 23) - вершина
    // (12, 20) - правый угол основания
    // (8, 20)  - левый угол основания

    EXPECT_DOUBLE_EQ(t.get_vertex(0).get_x(), 10.0);  // вершина X
    EXPECT_DOUBLE_EQ(t.get_vertex(0).get_y(), 23.0);  // вершина Y (20 + 3)

    EXPECT_DOUBLE_EQ(t.get_vertex(1).get_x(), 12.0);  // правый угол (10 + 2)
    EXPECT_DOUBLE_EQ(t.get_vertex(1).get_y(), 20.0);  // основание Y

    EXPECT_DOUBLE_EQ(t.get_vertex(2).get_x(), 8.0);   // левый угол (10 - 2)
    EXPECT_DOUBLE_EQ(t.get_vertex(2).get_y(), 20.0);  // основание Y
}

TEST(PointConstructorTest, TriangleWithInitializerList) {
    Triangle<double> t(4.0, 3.0, {
        Point<double>(0, 3),   // вершина
        Point<double>(2, 0),   // правый угол
        Point<double>(-2, 0)   // левый угол
    });

    EXPECT_DOUBLE_EQ(t.area(), 6.0);
    EXPECT_EQ(t.get_vertices_count(), 3);

    // Проверяем, что вершины установлены правильно
    EXPECT_DOUBLE_EQ(t.get_vertex(0).get_x(), 0.0);
    EXPECT_DOUBLE_EQ(t.get_vertex(0).get_y(), 3.0);

    EXPECT_DOUBLE_EQ(t.get_vertex(1).get_x(), 2.0);
    EXPECT_DOUBLE_EQ(t.get_vertex(1).get_y(), 0.0);

    EXPECT_DOUBLE_EQ(t.get_vertex(2).get_x(), -2.0);
    EXPECT_DOUBLE_EQ(t.get_vertex(2).get_y(), 0.0);
}

TEST(PointConstructorTest, RectangleWithCustomPoint) {
    Point<double> bottomLeft(5.0, 10.0);
    Rectangle<double> r(6.0, 4.0, bottomLeft);

    // Прямоугольник с началом в (5,10), длиной 6, шириной 4
    // Вершины должны быть:
    // (5, 10)    - левый нижний
    // (11, 10)   - правый нижний (5 + 6)
    // (5, 14)    - левый верхний (10 + 4)
    // (11, 14)   - правый верхний

    EXPECT_DOUBLE_EQ(r.get_vertex(0).get_x(), 5.0);
    EXPECT_DOUBLE_EQ(r.get_vertex(0).get_y(), 10.0);

    EXPECT_DOUBLE_EQ(r.get_vertex(1).get_x(), 11.0);
    EXPECT_DOUBLE_EQ(r.get_vertex(1).get_y(), 10.0);

    EXPECT_DOUBLE_EQ(r.get_vertex(2).get_x(), 5.0);
    EXPECT_DOUBLE_EQ(r.get_vertex(2).get_y(), 14.0);

    EXPECT_DOUBLE_EQ(r.get_vertex(3).get_x(), 11.0);
    EXPECT_DOUBLE_EQ(r.get_vertex(3).get_y(), 14.0);
}

TEST(PointConstructorTest, RectangleWithInitializerList) {
    Rectangle<double> r(5.0, 3.0, {
        Point<double>(0, 0),
        Point<double>(5, 0),
        Point<double>(0, 3),
        Point<double>(5, 3)
    });

    EXPECT_DOUBLE_EQ(r.area(), 15.0);
    EXPECT_EQ(r.get_vertices_count(), 4);

    // Проверяем вершины
    EXPECT_DOUBLE_EQ(r.get_vertex(0).get_x(), 0.0);
    EXPECT_DOUBLE_EQ(r.get_vertex(0).get_y(), 0.0);

    EXPECT_DOUBLE_EQ(r.get_vertex(1).get_x(), 5.0);
    EXPECT_DOUBLE_EQ(r.get_vertex(1).get_y(), 0.0);

    EXPECT_DOUBLE_EQ(r.get_vertex(2).get_x(), 0.0);
    EXPECT_DOUBLE_EQ(r.get_vertex(2).get_y(), 3.0);

    EXPECT_DOUBLE_EQ(r.get_vertex(3).get_x(), 5.0);
    EXPECT_DOUBLE_EQ(r.get_vertex(3).get_y(), 3.0);
}

TEST(PointConstructorTest, SquareWithCustomPoint) {
    Point<double> bottomLeft(2.0, 3.0);
    Square<double> s(4.0, bottomLeft);

    // Квадрат с началом в (2,3), стороной 4
    // Вершины должны быть:
    // (2, 3) - левый нижний
    // (6, 3) - правый нижний (2 + 4)
    // (2, 7) - левый верхний (3 + 4)
    // (6, 7) - правый верхний

    EXPECT_DOUBLE_EQ(s.get_vertex(0).get_x(), 2.0);
    EXPECT_DOUBLE_EQ(s.get_vertex(0).get_y(), 3.0);

    EXPECT_DOUBLE_EQ(s.get_vertex(1).get_x(), 6.0);
    EXPECT_DOUBLE_EQ(s.get_vertex(1).get_y(), 3.0);

    EXPECT_DOUBLE_EQ(s.get_vertex(2).get_x(), 2.0);
    EXPECT_DOUBLE_EQ(s.get_vertex(2).get_y(), 7.0);

    EXPECT_DOUBLE_EQ(s.get_vertex(3).get_x(), 6.0);
    EXPECT_DOUBLE_EQ(s.get_vertex(3).get_y(), 7.0);

    EXPECT_DOUBLE_EQ(s.area(), 16.0);
}

TEST(PointConstructorTest, SquareWithInitializerList) {
    Square<double> s(4.0, {
        Point<double>(1, 1),
        Point<double>(5, 1),
        Point<double>(1, 5),
        Point<double>(5, 5)
    });

    EXPECT_DOUBLE_EQ(s.area(), 16.0);
    EXPECT_EQ(s.get_vertices_count(), 4);

    // Проверяем вершины
    EXPECT_DOUBLE_EQ(s.get_vertex(0).get_x(), 1.0);
    EXPECT_DOUBLE_EQ(s.get_vertex(0).get_y(), 1.0);

    EXPECT_DOUBLE_EQ(s.get_vertex(1).get_x(), 5.0);
    EXPECT_DOUBLE_EQ(s.get_vertex(1).get_y(), 1.0);

    EXPECT_DOUBLE_EQ(s.get_vertex(2).get_x(), 1.0);
    EXPECT_DOUBLE_EQ(s.get_vertex(2).get_y(), 5.0);

    EXPECT_DOUBLE_EQ(s.get_vertex(3).get_x(), 5.0);
    EXPECT_DOUBLE_EQ(s.get_vertex(3).get_y(), 5.0);
}

TEST(PointConstructorTest, DefaultPointIsOrigin) {
    // Проверяем, что по умолчанию используется точка (0,0)
    Triangle<double> t_default(4.0, 3.0);
    Triangle<double> t_origin(4.0, 3.0, Point<double>(0, 0));

    // Фигуры с одинаковыми параметрами и точкой отсчета должны быть равны
    EXPECT_TRUE(t_default == t_origin);
}

TEST(PointConstructorTest, GeometricCenterConsistency) {
    // Центр масс должен правильно вычисляться для смещенных фигур
    Point<double> offset(10.0, 20.0);

    Triangle<double> t(4.0, 3.0, offset);
    Point<double> t_center = t.geometric_center();

    // Для треугольника центр должен быть смещен на offset
    EXPECT_DOUBLE_EQ(t_center.get_x(), 10.0);
    EXPECT_DOUBLE_EQ(t_center.get_y(), 21.0); // (20 + 1) т.к. высота 3

    Rectangle<double> r(6.0, 4.0, offset);
    Point<double> r_center = r.geometric_center();

    // Для прямоугольника центр = offset + (width/2, height/2)
    EXPECT_DOUBLE_EQ(r_center.get_x(), 13.0); // 10 + 6/2
    EXPECT_DOUBLE_EQ(r_center.get_y(), 22.0); // 20 + 4/2

    Square<double> s(4.0, offset);
    Point<double> s_center = s.geometric_center();

    // Для квадрата центр = offset + (side/2, side/2)
    EXPECT_DOUBLE_EQ(s_center.get_x(), 12.0); // 10 + 4/2
    EXPECT_DOUBLE_EQ(s_center.get_y(), 22.0); // 20 + 4/2
}

// Тесты исключений для неправильных initializer_list
TEST(PointConstructorTest, InvalidInitializerListThrows) {
    // Слишком мало точек для треугольника
    EXPECT_THROW({
        Triangle<double> t(4.0, 3.0, {
            Point<double>(0, 3),
            Point<double>(2, 0)
            // не хватает третьей точки
        });
    }, std::invalid_argument);

    // Слишком много точек для треугольника
    EXPECT_THROW({
        Triangle<double> t(4.0, 3.0, {
            Point<double>(0, 3),
            Point<double>(2, 0),
            Point<double>(-2, 0),
            Point<double>(0, 1) // лишняя точка
        });
    }, std::invalid_argument);

    // Слишком мало точек для прямоугольника
    EXPECT_THROW({
        Rectangle<double> r(5.0, 3.0, {
            Point<double>(0, 0),
            Point<double>(5, 0),
            Point<double>(0, 3)
            // не хватает четвертой точки
        });
    }, std::invalid_argument);
}

// Тесты копирования и перемещения с custom points
TEST(PointConstructorTest, CopyWithCustomPoints) {
    Point<double> customPoint(5.0, 10.0);
    Triangle<double> original(4.0, 3.0, customPoint);
    Triangle<double> copy(original);

    // Проверяем, что вершины скопированы правильно
    for (size_t i = 0; i < original.get_vertices_count(); ++i) {
        EXPECT_DOUBLE_EQ(original.get_vertex(i).get_x(), copy.get_vertex(i).get_x());
        EXPECT_DOUBLE_EQ(original.get_vertex(i).get_y(), copy.get_vertex(i).get_y());
    }

    EXPECT_TRUE(original == copy);
}

TEST(PointConstructorTest, MoveWithCustomPoints) {
    Point<double> customPoint(5.0, 10.0);
    Triangle<double> original(4.0, 3.0, customPoint);
    double original_area = original.area();

    Triangle<double> moved(std::move(original));

    // Проверяем, что площадь сохранилась
    EXPECT_DOUBLE_EQ(moved.area(), original_area);

    // Проверяем, что вершины переместились правильно
    EXPECT_DOUBLE_EQ(moved.get_vertex(0).get_x(), 5.0);
    EXPECT_DOUBLE_EQ(moved.get_vertex(0).get_y(), 13.0);
}

// Тесты для разных скалярных типов с точками
TEST(PointConstructorTest, DifferentScalarTypesWithPoints) {
    // int
    Point<int> int_point(5, 10);
    Triangle<int> t_int(4, 3, int_point);
    EXPECT_EQ(t_int.area(), 6);

    // float
    Point<float> float_point(5.0f, 10.0f);
    Triangle<float> t_float(4.0f, 3.0f, float_point);
    EXPECT_FLOAT_EQ(t_float.area(), 6.0f);

    // double
    Point<double> double_point(5.0, 10.0);
    Triangle<double> t_double(4.0, 3.0, double_point);
    EXPECT_DOUBLE_EQ(t_double.area(), 6.0);
}

// Интеграционные тесты с массивом
TEST(PointConstructorTest, ArrayWithCustomPoints) {
    FigureArray<double> arr(3);

    // Добавляем фигуры с разными точками отсчета
    arr[0] = std::make_shared<Triangle<double>>(4.0, 3.0, Point<double>(1, 1));
    arr[1] = std::make_shared<Rectangle<double>>(5.0, 3.0, Point<double>(2, 2));
    arr[2] = std::make_shared<Square<double>>(4.0, Point<double>(3, 3));

    EXPECT_EQ(arr.get_size(), 3);

    // Проверяем, что площади вычисляются правильно
    EXPECT_DOUBLE_EQ(arr.total_area(), 6.0 + 15.0 + 16.0);

    // Проверяем полиморфное копирование
    auto cloned_figure = arr[0]->clone();
    EXPECT_DOUBLE_EQ(arr[0]->area(), cloned_figure->area());
    EXPECT_TRUE(*arr[0] == *cloned_figure);
}

// Тесты для проверки, что геометрический центр вычисляется правильно
// для фигур со смещенными координатами
TEST(PointConstructorTest, GeometricCenterAccuracy) {
    // Треугольник со смещением
    Point<double> triangle_offset(10.0, 20.0);
    Triangle<double> triangle(6.0, 4.0, triangle_offset);

    Point<double> triangle_center = triangle.geometric_center();

    // Для треугольника: среднее арифметическое координат вершин
    double expected_x = (10.0 + 13.0 + 7.0) / 3.0;  // (10 + (10+3) + (10-3)) / 3
    double expected_y = (24.0 + 20.0 + 20.0) / 3.0; // ((20+4) + 20 + 20) / 3

    EXPECT_DOUBLE_EQ(triangle_center.get_x(), expected_x);
    EXPECT_DOUBLE_EQ(triangle_center.get_y(), expected_y);

    // Прямоугольник со смещением
    Point<double> rect_offset(5.0, 10.0);
    Rectangle<double> rectangle(8.0, 6.0, rect_offset);

    Point<double> rect_center = rectangle.geometric_center();

    // Для прямоугольника: offset + (width/2, height/2)
    EXPECT_DOUBLE_EQ(rect_center.get_x(), 5.0 + 4.0);  // 5 + 8/2
    EXPECT_DOUBLE_EQ(rect_center.get_y(), 10.0 + 3.0); // 10 + 6/2

    // Квадрат со смещением
    Point<double> square_offset(2.0, 3.0);
    Square<double> square(5.0, square_offset);

    Point<double> square_center = square.geometric_center();

    // Для квадрата: offset + (side/2, side/2)
    EXPECT_DOUBLE_EQ(square_center.get_x(), 2.0 + 2.5);  // 2 + 5/2
    EXPECT_DOUBLE_EQ(square_center.get_y(), 3.0 + 2.5);  // 3 + 5/2
}
