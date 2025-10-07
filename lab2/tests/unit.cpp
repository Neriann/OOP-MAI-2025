#include <gtest/gtest.h>

#include <sstream>

#include "money.hpp"

class MoneyTest : public ::testing::Test {
protected:
    void SetUp() override {
        money1 = {'1', '2'};
        money2 = {'2', '4'};
        money3 = {'1', '2'};
        money4 = {'3', '2', '1'};
        money5 = {'0'};
    }
    Money money1;
    Money money2;
    Money money3;
    Money money4;
    Money money5;
};

TEST_F(MoneyTest, TestEqual) {
    // 12 != 24
    ASSERT_NE(money1, money2);

    // 12 == 12
    ASSERT_EQ(money1, money3);
}

TEST_F(MoneyTest, TestGreater) {
    // 312 > 24
    ASSERT_GT(money4, money2);

    // 12 >= 12
    ASSERT_GE(money1, money3);

    // 24 > 12
    ASSERT_GT(money2, money1);
}

TEST_F(MoneyTest, TestLess) {
    // 12 < 24
    ASSERT_LT(money1, money2);

    // 12 <= 24
    ASSERT_LE(money1, money2);

    // 12 <= 12
    ASSERT_LE(money1, money3);

    // 24 <= 321
    ASSERT_LE(money2, money4);
}

TEST_F(MoneyTest, TestAdd) {
    // 12 + 12 == 24
    ASSERT_EQ(money1 + money3, money2);

    // 12 + 24 > 12
    ASSERT_GT(money1 + money2, money3);

    // 12 + 24 + 12 < 321
    ASSERT_LT(money1 + money2 + money3, money4);

    // 12 + 12 + 24 == 48
    ASSERT_EQ(money1 + money3 + money2, Money("48"));
}

TEST_F(MoneyTest, TestSub) {
    // 12 - 12 == 0
    ASSERT_EQ(money1 - money3, Money("0"));

    // 24 > 24 - 12
    ASSERT_GT(money2, money2 - money1);

    // 12 - 24 < 0
    ASSERT_LT(money1 - money2, Money());

    // 321 - 12 = 309
    ASSERT_EQ(money4 - money1, Money("309"));

    // 1111 - 228 = 883
    ASSERT_EQ(Money(4, '1') - Money({'2', '2', '8'}), Money("883"));
}

TEST_F(MoneyTest, TestPrint) {
    std::stringstream ss;
    ss << money1;
    ASSERT_EQ(ss.str(), "12");

    ss.str("");
    ss << money4;
    ASSERT_EQ(ss.str(), "321");

    ss.str("");
    ss << money5;
    ASSERT_EQ(ss.str(), "0");
}

TEST_F(MoneyTest, TestConstructors) {
    // Конструктор по умолчанию
    Money empty;
    ASSERT_EQ(empty.GetLength(), 0);
    ASSERT_EQ(empty.GetMoney(), nullptr);

    // Конструктор от строки
    Money fromString("123");
    ASSERT_EQ(fromString, Money({'1', '2', '3'}));

    // Конструктор от initializer_list
    Money fromList{'4', '5', '6'};
    ASSERT_EQ(fromList, Money("456"));

    // Конструктор копирования
    Money copy(money4);
    ASSERT_EQ(copy, money4);

    // Конструктор перемещения
    Money moved(std::move(Money("789")));
    ASSERT_EQ(moved, Money("789"));
}

TEST_F(MoneyTest, TestAssignmentOperators) {
    // Оператор копирующего присваивания
    Money copyAssigned;
    copyAssigned = money1;
    ASSERT_EQ(copyAssigned, money1);

    // Оператор перемещающего присваивания
    Money moveAssigned;
    moveAssigned = Money("999");
    ASSERT_EQ(moveAssigned, Money("999"));

    // Self-assignment
    money1 = money1;
    ASSERT_EQ(money1, Money("12"));
}

TEST_F(MoneyTest, TestGetMethods) {
    ASSERT_EQ(money1.GetLength(), 2);
    ASSERT_EQ(money4.GetLength(), 3);
    ASSERT_EQ(money5.GetLength(), 1);

    const unsigned char* data1 = money1.GetMoney();
    ASSERT_EQ(data1[0], '1');
    ASSERT_EQ(data1[1], '2');

    const unsigned char* data5 = money5.GetMoney();
    ASSERT_EQ(data5[0], '0');
}

TEST_F(MoneyTest, TestEdgeCases) {
    // Работа с нулем
    Money zero("0");
    ASSERT_EQ(zero + zero, zero);
    ASSERT_EQ(zero - zero, zero);
    ASSERT_EQ(zero, money5);

    // Большие числа
    Money big1("999999999");
    Money big2("1");
    Money result = big1 + big2;
    ASSERT_EQ(result, Money("1000000000"));

    // Вычитание до отрицательного
    Money small("10");
    Money large("100");
    Money negativeResult = small - large;
    ASSERT_LT(negativeResult, Money("0"));
}

TEST_F(MoneyTest, TestInvalidArguments) {
    // Проверка исключений для невалидных строк
    ASSERT_THROW(Money(""), std::invalid_argument);
    ASSERT_THROW(Money("12a3"), std::invalid_argument);
    ASSERT_THROW(Money("abc"), std::invalid_argument);

    // Проверка исключений для невалидного initializer_list
    ASSERT_THROW(Money({'1', 'a', '3'}), std::invalid_argument);
}

TEST_F(MoneyTest, TestComparisonConsistency) {
    // Проверка консистентности сравнений
    ASSERT_TRUE(money1 == money3);
    ASSERT_FALSE(money1 != money3);
    ASSERT_FALSE(money1 < money3);
    ASSERT_FALSE(money1 > money3);
    ASSERT_TRUE(money1 <= money3);
    ASSERT_TRUE(money1 >= money3);

    ASSERT_TRUE(money1 < money2);
    ASSERT_TRUE(money2 > money1);
    ASSERT_FALSE(money1 > money2);
    ASSERT_FALSE(money2 < money1);
}

TEST_F(MoneyTest, TestArithmeticProperties) {
    // Коммутативность сложения
    ASSERT_EQ(money1 + money2, money2 + money1);

    // Ассоциативность сложения
    Money sum1 = (money1 + money2) + money3;
    Money sum2 = money1 + (money2 + money3);
    ASSERT_EQ(sum1, sum2);

    // a - a = 0
    ASSERT_EQ(money1 - money1, Money("0"));
    ASSERT_EQ(money4 - money4, Money("0"));
}
