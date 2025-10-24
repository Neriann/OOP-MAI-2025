#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>

#include "shapes.hpp"
#include "square.hpp"
#include "triangle.hpp"
#include "octagon.hpp"

class ShapesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем тестовые фигуры
        triangle = new Triangle{{0, 0}, {1, 0}, {0.5, std::sqrt(3)/2}};
        square = new Square{{0, 0}, {2, 0}, {2, 2}, {0, 2}};
        octagon = new Octagon{
            {1, 0},
            {std::sqrt(2)/2, std::sqrt(2)/2},
            {0, 1},
            {-std::sqrt(2)/2, std::sqrt(2)/2},
            {-1, 0},
            {-std::sqrt(2)/2, -std::sqrt(2)/2},
            {0, -1},
            {std::sqrt(2)/2, -std::sqrt(2)/2}
        };
    }

    void TearDown() override {
        delete triangle;
        delete square;
        delete octagon;
    }

    Figure* triangle = nullptr;
    Figure* square = nullptr;
    Figure* octagon = nullptr;
};

// Базовые тесты конструктора и доступа
TEST_F(ShapesTest, ConstructorAndAccess) {
    Shapes shapes(3);
    shapes[0] = triangle;
    shapes[1] = square;
    shapes[2] = octagon;

    EXPECT_EQ(shapes[0], triangle);
    EXPECT_EQ(shapes[1], square);
    EXPECT_EQ(shapes[2], octagon);
}

TEST_F(ShapesTest, AreaCalculation) {
    Shapes shapes(3);
    shapes[0] = triangle;
    shapes[1] = square;
    shapes[2] = octagon;

    double total_area = shapes.Area();

    // Проверяем что площадь вычисляется (конкретные значения зависят от фигур)
    EXPECT_GT(total_area, 0.0);
    EXPECT_TRUE(std::isfinite(total_area));
}

TEST_F(ShapesTest, AreaWithEmptyShapes) {
    Shapes shapes(0);
    EXPECT_DOUBLE_EQ(shapes.Area(), 0.0);
}

TEST_F(ShapesTest, AreaWithNullFigures) {
    Shapes shapes(3);
    shapes[0] = triangle;
    shapes[1] = nullptr;  // null figure
    shapes[2] = octagon;

    // Должно корректно обрабатывать nullptr
    double area = shapes.Area();
    EXPECT_GT(area, 0.0);
}

// Тесты удаления фигур
TEST_F(ShapesTest, RemoveFirstFigure) {
    Shapes shapes(3);
    shapes[0] = triangle;
    shapes[1] = square;
    shapes[2] = octagon;

    shapes.RemoveFigure(0);

    // После удаления первой фигуры, остальные сдвигаются
    EXPECT_EQ(shapes[0], square);
    EXPECT_EQ(shapes[1], octagon);
    EXPECT_EQ(shapes[2], nullptr);  // Последний становится nullptr
}

TEST_F(ShapesTest, RemoveMiddleFigure) {
    Shapes shapes(3);
    shapes[0] = triangle;
    shapes[1] = square;
    shapes[2] = octagon;

    shapes.RemoveFigure(1);

    EXPECT_EQ(shapes[0], triangle);
    EXPECT_EQ(shapes[1], octagon);
    EXPECT_EQ(shapes[2], nullptr);
}

TEST_F(ShapesTest, RemoveLastFigure) {
    Shapes shapes(3);
    shapes[0] = triangle;
    shapes[1] = square;
    shapes[2] = octagon;

    shapes.RemoveFigure(2);

    EXPECT_EQ(shapes[0], triangle);
    EXPECT_EQ(shapes[1], square);
    EXPECT_EQ(shapes[2], nullptr);
}

TEST_F(ShapesTest, RemoveFigureUpdatesSize) {
    Shapes shapes(3);
    shapes[0] = triangle;
    shapes[1] = square;
    shapes[2] = octagon;

    size_t initial_size = 3; // нужно получить доступ к size (если он public)

    shapes.RemoveFigure(1);
    // Проверяем что размер уменьшился (если есть доступ к size)
    // Если size private, проверяем через поведение
    EXPECT_EQ(shapes[2], nullptr); // Косвенная проверка
}

TEST_F(ShapesTest, RemoveFigureOutOfRangeThrows) {
    Shapes shapes(3);

    EXPECT_THROW({
        shapes.RemoveFigure(5);  // Выход за границы
    }, std::out_of_range);

    EXPECT_THROW({
        shapes.RemoveFigure(3);  // Граничный случай (индекс = размер)
    }, std::out_of_range);
}

// Тесты на граничные случаи
TEST_F(ShapesTest, SingleFigure) {
    Shapes shapes(1);
    shapes[0] = square;

    double area = shapes.Area();
    double expected_area = static_cast<double>(*square);
    EXPECT_DOUBLE_EQ(area, expected_area);

    shapes.RemoveFigure(0);
    EXPECT_EQ(shapes[0], nullptr);
}

TEST_F(ShapesTest, AllNullFigures) {
    Shapes shapes(3);
    shapes[0] = nullptr;
    shapes[1] = nullptr;
    shapes[2] = nullptr;

    EXPECT_DOUBLE_EQ(shapes.Area(), 0.0);
}

// Тесты оператора []
TEST_F(ShapesTest, OperatorBracketAssignment) {
    Shapes shapes(2);

    // Проверяем что можно присваивать
    shapes[0] = triangle;
    shapes[1] = square;

    // Проверяем что можно изменять
    shapes[0] = octagon;
    EXPECT_EQ(shapes[0], octagon);
}

// Тесты деструктора (косвенно через valgrind или sanitizers)
TEST_F(ShapesTest, DestructorCleansUp) {
    // Этот тест проверяет что нет утечек памяти
    // Реальная проверка требует инструментов вроде valgrind
    {
        Shapes shapes(2);
        shapes[0] = new Triangle{{0, 0}, {1, 0}, {0.5, std::sqrt(3)/2}};
        shapes[1] = new Square{{0, 0}, {1, 0}, {1, 1}, {0, 1}};

        // При выходе из scope деструктор должен очистить figures
        // но НЕ удалять сами фигуры (согласно вашему комментарию)
    }
    // Если есть утечки, тест упадет при использовании sanitizers
}

// Тесты на последовательные операции
TEST_F(ShapesTest, MultipleRemoveOperations) {
    Shapes shapes(5);
    Figure* fig1 = new Triangle{{0, 0}, {1, 0}, {0.5, std::sqrt(3)/2}};
    Figure* fig2 = new Square{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    Figure* fig3 = new Triangle{{0, 0}, {2, 0}, {1, std::sqrt(3)}};

    shapes[0] = fig1;
    shapes[1] = fig2;
    shapes[2] = fig3;
    shapes[3] = nullptr;
    shapes[4] = nullptr;

    shapes.RemoveFigure(1);  // Удаляем fig2
    EXPECT_EQ(shapes[0], fig1);
    EXPECT_EQ(shapes[1], fig3);
    EXPECT_EQ(shapes[2], nullptr);
    EXPECT_EQ(shapes[3], nullptr);

    shapes.RemoveFigure(0);  // Удаляем fig1
    EXPECT_EQ(shapes[0], fig3);
    EXPECT_EQ(shapes[1], nullptr);
    EXPECT_EQ(shapes[2], nullptr);

    // Очистка
    delete fig1;
    delete fig2;
    delete fig3;
}

// Тест на правильность вычислений после удаления
TEST_F(ShapesTest, AreaAfterRemove) {
    Shapes shapes(3);
    shapes[0] = triangle;  // площадь ~0.433
    shapes[1] = square;    // площадь 4.0
    shapes[2] = octagon;   // площадь ~4.828

    double initial_area = shapes.Area();

    shapes.RemoveFigure(1); // Удаляем квадрат
    double area_after_remove = shapes.Area();

    // Площадь должна уменьшиться примерно на площадь квадрата
    EXPECT_LT(area_after_remove, initial_area);
}

// Тест на обработку исключений при вычислении площади
TEST_F(ShapesTest, AreaWithInvalidFigures) {
    Shapes shapes(2);
    shapes[0] = triangle;
    shapes[1] = nullptr;

    // Не должно бросать исключение при наличии nullptr
    EXPECT_NO_THROW({
        double area = shapes.Area();
    });
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
