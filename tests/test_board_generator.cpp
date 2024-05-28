#include <gtest/gtest.h>
#include "board_generator.h"

TEST(BoardGeneratorTest, GenerateDominos_CorrectCount) {
    int maxPips = 6;
    auto dominos = generate_dominos(maxPips);
    EXPECT_EQ(dominos.size(), 21);
}

TEST(BoardGeneratorTest, GenerateBoard_CorrectSize) {
    int rows = 4;
    int cols = 7;
    auto board = generate_board(rows, cols);
    ASSERT_EQ(board.size(), rows);
    ASSERT_EQ(board[0].size(), cols);
}

TEST(BoardGeneratorTest, GenerateBoard_ThrowsWhenInvalidSize) {
    int rows = 4;
    int cols = 5;
    EXPECT_THROW(generate_board(rows, cols), std::runtime_error);
}

TEST(BoardGeneratorTest, GenerateBoard_FilledCorrectly) {
    int rows = 2;
    int cols = 2;
    auto board = generate_board(rows, cols);
    EXPECT_EQ(board[0][0], board[1][0]);
}
