#include <gtest/gtest.h>
#include "utils.h"

TEST(FindMaxPipsTest, EmptyBoard) {
    std::vector<std::vector<int>> board; // Empty board
    EXPECT_EQ(find_max_pips(board), 0); // Expect 0 for empty board
}

TEST(FindMaxPipsTest, UniformBoard) {
    std::vector<std::vector<int>> board = {
            {2, 2, 2},
            {2, 2, 2},
            {2, 2, 2}
    };
    EXPECT_EQ(find_max_pips(board), 2); // Expect 2 for uniform board
}

TEST(FindMaxPipsTest, VaryingPips) {
    std::vector<std::vector<int>> board = {
            {1, 3, 5},
            {6, 4, 2},
            {3, 7, 1}
    };
    EXPECT_EQ(find_max_pips(board), 7); // Expect 7 as the max pips
}

TEST(FindMaxPipsTest, SingleRowBoard) {
    std::vector<std::vector<int>> board = {
            {0, 1, 2, 3, 4, 5, 6}
    };
    EXPECT_EQ(find_max_pips(board), 6); // Expect 6 on a single row
}

TEST(FindMaxPipsTest, SingleColumnBoard) {
    std::vector<std::vector<int>> board = {
            {0},
            {1},
            {2},
            {3},
            {6}
    };
    EXPECT_EQ(find_max_pips(board), 6); // Expect 6 on a single column
}
