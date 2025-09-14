#include <gtest/gtest.h>
#include "../include/replacer.hpp"

TEST(ExampleTest, BasicTest1) {
    EXPECT_EQ(replace_symbol("MAI is the best university in the world!", 2, 'e', '@'), "MAI is the b@st university in th@ world!");
}

TEST(ExampleTest, BasicTest2) {
    EXPECT_EQ(replace_symbol("all programmers will be replaced by chatgpt hi-hi-hi-ha", 1, 'a', '#'), "#ll progr#mmers will be repl#ced by ch#tgpt hi-hi-hi-h#");
}

TEST(ExampleTest, BasicTest3) {
    EXPECT_EQ(replace_symbol("Tyring machine forever!", 3, 'r', 'R'), "Tyring machine foreveR!");
}

TEST(ExampleTest, ZeroTest) {
    EXPECT_EQ(replace_symbol("MAI is the best university in the world!", 0, 'e', '@'), "MAI is the best university in the world!");
}

TEST(ExampleTest, NegativeTest) {
    EXPECT_EQ(replace_symbol("MAI is the best university in the world!", -5, 'e', '@'), "MAI is the best university in the world!");
}

TEST(ExampleTest, GreatTest) {
    EXPECT_EQ(replace_symbol("MAI is the best university in the world!", 10, 'e', '@'), "MAI is the best university in the world!");
}