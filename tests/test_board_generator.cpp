#include <gtest/gtest.h>
#include "board_generator.h"

TEST(BoardGeneratorTest, GenerateDominos_CorrectCount) {
    int maxPips = 6;
    auto dominos = generate_dominos(maxPips - 1);
    EXPECT_EQ(dominos.size(), 21);
}

TEST(BoardGeneratorTest, GenerateBoard_CorrectSize) {
    int rows = 4;
    int cols = 7;
    auto board = generate_board(rows, cols);
    ASSERT_EQ(board.size(), rows);
    ASSERT_EQ(board[0].size(), cols);
}

TEST(DominoGeneration, GeneratesCorrectNumberOfDominos) {
    int max_pips = 6;
    auto dominos = generate_dominos(max_pips);
    EXPECT_EQ(dominos.size(), (max_pips + 1) * (max_pips + 2) / 2);
}

TEST(BoardToJson, FormatsCorrectly) {
    std::vector<std::vector<int>> board = {{0, 1}, {2, 3}};
    std::string expected_json = "[[0,1],[2,3]]";
    EXPECT_EQ(board_to_json_string(board), expected_json);
}

TEST(GenerateBoard, CorrectSizeAndFilling) {
    int rows = 3, cols = 3;
    auto board = generate_board(rows, cols);
    EXPECT_EQ(board.size(), rows);
    for (const auto& row : board) {
        EXPECT_EQ(row.size(), cols);
        for (int num : row) {
            EXPECT_NE(num, -1);  // Ensure the board is completely filled
        }
    }
}

TEST(GenerateAllBoardsTest, UniqueBoardsGenerated) {
    int rows = 2, cols = 3;
    auto boards = generate_all_boards(rows, cols);
    std::set<std::string> board_strings;
    for (const auto& board : boards) {
        std::string str = board_to_json_string(board);
        auto result = board_strings.insert(str);
        EXPECT_TRUE(result.second) << "Duplicate board detected: " << str;
    }
}
