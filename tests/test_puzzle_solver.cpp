#include <gtest/gtest.h>
#include "puzzle_solver.h" // Include the correct path to your PuzzleSolver definition

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> create_empty_board_and_placement(int rows, int cols) {
    std::vector<std::vector<int>> board(rows, std::vector<int>(cols, -1));
    std::vector<std::vector<int>> placement(rows, std::vector<int>(cols, -1));
    return {board, placement};
}

TEST(PuzzleSolverTest, CanPlace_HorizontalTrue) {
    auto [board, placement] = create_empty_board_and_placement(2, 2);
    board[0][0] = 1; board[0][1] = 2;
    Domino domino(1, 2);

    EXPECT_TRUE(PuzzleSolver::can_place(board, placement, domino, 0, 0, true));
}

TEST(PuzzleSolverTest, CanPlace_HorizontalFalse_Occupied) {
    auto [board, placement] = create_empty_board_and_placement(2, 2);
    board[0][0] = 1; board[0][1] = 2;
    placement[0][0] = 0; // Occupied
    Domino domino(1, 2);

    EXPECT_FALSE(PuzzleSolver::can_place(board, placement, domino, 0, 0, true));
}

TEST(PuzzleSolverTest, CanPlace_HorizontalFalse_OutOfBounds) {
    auto [board, placement] = create_empty_board_and_placement(2, 1); // Only 1 column
    board[0][0] = 1;
    Domino domino(1, 2);

    EXPECT_FALSE(PuzzleSolver::can_place(board, placement, domino, 0, 0, true));
}

TEST(PuzzleSolverTest, CanPlace_VerticalTrue) {
    auto [board, placement] = create_empty_board_and_placement(2, 2);
    board[0][0] = 1; board[1][0] = 2;
    Domino domino(1, 2);

    EXPECT_TRUE(PuzzleSolver::can_place(board, placement, domino, 0, 0, false));
}

TEST(PuzzleSolverTest, CanPlace_VerticalFalse_Occupied) {
    auto [board, placement] = create_empty_board_and_placement(2, 2);
    board[0][0] = 1; board[1][0] = 2;
    placement[1][0] = 0; // Occupied
    Domino domino(1, 2);

    EXPECT_FALSE(PuzzleSolver::can_place(board, placement, domino, 0, 0, false));
}

TEST(PuzzleSolverTest, CanPlace_VerticalFalse_OutOfBounds) {
    auto [board, placement] = create_empty_board_and_placement(1, 2); // Only 1 row
    board[0][0] = 1;
    Domino domino(1, 2);

    EXPECT_FALSE(PuzzleSolver::can_place(board, placement, domino, 0, 0, false));
}

TEST(PuzzleSolverTest, SolvePuzzle_SolvableBoard) {
    auto [board, placement] = create_empty_board_and_placement(2, 2);
    board[0][0] = 1; board[0][1] = 2;
    board[1][0] = 2; board[1][1] = 1;
    std::vector<Domino> dominos = {Domino(1, 2), Domino(2, 1)};

    EXPECT_TRUE(PuzzleSolver::solve_puzzle(board, placement, dominos, 0, 0));
    // Optionally verify that placement is as expected
}

TEST(PuzzleSolverTest, SolvePuzzle_UnsolvableBoard) {
    auto [board, placement] = create_empty_board_and_placement(2, 2);
    board[0][0] = 1; board[0][1] = 2;
    board[1][0] = 3; board[1][1] = 1;
    std::vector<Domino> dominos = {Domino(1, 2), Domino(2, 3)};

    EXPECT_FALSE(PuzzleSolver::solve_puzzle(board, placement, dominos, 0, 0));
}

TEST(PuzzleSolverTest, SolvePuzzle_VerticalPlacement_ValidNoSolution) {
    std::vector<std::vector<int>> board = {
            {1, 3},
            {2, -1}
    };
    std::vector<std::vector<int>> placement = {
            {-1, -1},
            {-1, -1}
    };
    std::vector<Domino> dominos = {Domino(1, 2), Domino(3, 4)};

    EXPECT_FALSE(PuzzleSolver::solve_puzzle(board, placement, dominos, 0, 0));
}

TEST(PuzzleSolverTest, SolvePuzzle_VerticalPlacement_InvalidBoundary) {
    std::vector<std::vector<int>> board = {
            {1},
            {2}
    };
    std::vector<std::vector<int>> placement = {
            {-1},
            {-1}
    };
    std::vector<Domino> dominos = {Domino(1, 2)};

    // At the bottom edge, no room for vertical placement
    EXPECT_FALSE(PuzzleSolver::solve_puzzle(board, placement, dominos, 1, 0));
}

TEST(PuzzleSolverTest, SolvePuzzle_VerticalPlacement_InvalidOccupied) {
    std::vector<std::vector<int>> board = {
            {1, -1},
            {2, -1}
    };
    std::vector<std::vector<int>> placement = {
            {-1, -1},
            {0, -1} // Position already occupied by another domino
    };
    std::vector<Domino> dominos = {Domino(1, 2)};

    EXPECT_FALSE(PuzzleSolver::solve_puzzle(board, placement, dominos, 0, 0));
}

TEST(PuzzleSolverTest, CanPlace_Horizontal_Success) {
    std::vector<std::vector<int>> board = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> placement = {{-1, -1}, {-1, -1}};
    Domino domino(1, 2);

    bool canPlace = PuzzleSolver::can_place(board, placement, domino, 0, 0, true);
    EXPECT_TRUE(canPlace);
}

TEST(PuzzleSolverTest, CanPlace_Horizontal_Fail_Occupied) {
    std::vector<std::vector<int>> board = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> placement = {{0, 0}, {-1, -1}};
    Domino domino(1, 2);

    bool canPlace = PuzzleSolver::can_place(board, placement, domino, 0, 0, true);
    EXPECT_FALSE(canPlace);
}

TEST(PuzzleSolverTest, CanPlace_Horizontal_Fail_OutOfBounds) {
    std::vector<std::vector<int>> board = {{1}};
    std::vector<std::vector<int>> placement = {{-1}};
    Domino domino(1, 2);

    bool canPlace = PuzzleSolver::can_place(board, placement, domino, 0, 0, true);
    EXPECT_FALSE(canPlace);
}

TEST(PuzzleSolverTest, CanPlace_Vertical_Success) {
    std::vector<std::vector<int>> board = {{1}, {2}};
    std::vector<std::vector<int>> placement = {{-1}, {-1}};
    Domino domino(1, 2);

    bool canPlace = PuzzleSolver::can_place(board, placement, domino, 0, 0, false);
    EXPECT_TRUE(canPlace);
}

TEST(PuzzleSolverTest, CanPlace_Vertical_Fail_Occupied) {
    std::vector<std::vector<int>> board = {{1}, {2}};
    std::vector<std::vector<int>> placement = {{0}, {-1}};
    Domino domino(1, 2);

    bool canPlace = PuzzleSolver::can_place(board, placement, domino, 0, 0, false);
    EXPECT_FALSE(canPlace);
}

TEST(PuzzleSolverTest, CanPlace_Vertical_Fail_OutOfBounds) {
    std::vector<std::vector<int>> board = {{1}};
    std::vector<std::vector<int>> placement = {{-1}};
    Domino domino(1, 2);

    bool canPlace = PuzzleSolver::can_place(board, placement, domino, 0, 0, false);
    EXPECT_FALSE(canPlace);
}

TEST(PuzzleSolverTest, SolvePuzzleReturnsTrueWhenSolutionExists) {
    // Arrange
    PuzzleSolver solver;
    std::vector<std::vector<int>> board = {
            {4, 5, 5, 6, 6, 3, 1, 4, 7},
            {6, 1, 2, 3, 2, 2, 2, 1, 1},
            {1, 0, 0, 0, 3, 3, 1, 1, 6},
            {6, 0, 4, 3, 7, 6, 7, 3, 2},
            {6, 0, 2, 4, 7, 7, 7, 2, 1},
            {1, 0, 0, 0, 7, 5, 7, 2, 3},
            {3, 4, 5, 4, 5, 5, 4, 2, 6},
            {0, 3, 5, 6, 5, 4, 4, 5, 7}
    };
    std::vector<std::vector<int>> placement = {
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1}
    };
    std::vector<Domino> dominos = {
            Domino(0, 0),
            Domino(0, 1), Domino(1, 1),
            Domino(0, 2), Domino(1, 2), Domino(2, 2),
            Domino(0, 3), Domino(1, 3), Domino(2, 3), Domino(3, 3),
            Domino(0, 4), Domino(1, 4), Domino(2, 4), Domino(3, 4), Domino(4, 4),
            Domino(0, 5), Domino(1, 5), Domino(2, 5), Domino(3, 5), Domino(4, 5), Domino(5, 5),
            Domino(0, 6), Domino(1, 6), Domino(2, 6), Domino(3, 6), Domino(4, 6), Domino(5, 6), Domino(6, 6),
            Domino(0, 7), Domino(1, 7), Domino(2, 7), Domino(3, 7), Domino(4, 7), Domino(5, 7), Domino(6, 7), Domino(7, 7)
    };

    // Act
    bool result = solver.solve_puzzle(board, placement, dominos, 0, 0);

    // Assert
    EXPECT_TRUE(result);
}