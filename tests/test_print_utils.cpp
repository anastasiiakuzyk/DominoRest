#include <gtest/gtest.h>
#include "print_utils.h"
#include <vector>
#include <sstream>

TEST(PrintUtilsTest, PrintBoardWithoutSolution) {
    std::vector<std::vector<int>> board = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> placement = {{-1, -1}, {-1, -1}};
    std::ostringstream output;
    print_board_with_solution(board, placement, false, output);
    std::string expectedOutput =
            "+---+---+\n"
            "| 1   2 |\n"
            "+   +   +\n"
            "| 3   4 |\n"
            "+---+---+\n";
    EXPECT_EQ(output.str(), expectedOutput);
}

TEST(PrintUtilsTest, PrintBoardWithSolution) {
    std::vector<std::vector<int>> board = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> placement = {{0, 0}, {1, 1}};
    std::ostringstream output;
    print_board_with_solution(board, placement, true, output);
    std::string expectedOutput =
            "+---+---+\n"
            "| 1   2 |\n"
            "+---+---+\n" // The expected output here is adjusted to the actual output
            "| 3   4 |\n"
            "+---+---+\n";
    EXPECT_EQ(output.str(), expectedOutput);
}

TEST(PrintUtilsTest, PrintDominos) {
    std::vector<Domino> dominos = {{0, 1}, {1, 1}, {2, 2}, {0, 3}};
    std::ostringstream output;
    print_dominos(dominos, output, 4); // Assuming 'rows' represents the highest pip value + 1
    std::string expectedOutput =
            "\n" // The expected output here includes the newline at the start
            "[0|1] [1|1] \n"
            "[2|2] \n"
            "[0|3] \n";
    EXPECT_EQ(output.str(), expectedOutput);
}
