#pragma once

#include "domino.h"
#include <vector>
/**
 * @file puzzle_solver.h
 * @brief Declaration of the PuzzleSolver class and its methods for solving the domino puzzle.
 */

struct ThreadData {
    const std::vector<std::vector<int>> *board;
    std::vector<std::vector<int>> *placement;
    std::vector<Domino> *dominos;
    int x;
    int y;
};

/**
 * @class PuzzleSolver
 * @brief Provides static methods for solving the domino puzzle.
 */
class PuzzleSolver {
public:
    /**
     * @brief Checks if a domino can be placed at the specified position.
     * @param board The current state of the game board.
     * @param placement The current placement of dominos on the board.
     * @param domino The domino to be placed.
     * @param x The row position for placement.
     * @param y The column position for placement.
     * @param horizontal Whether the domino is to be placed horizontally.
     * @return true if the domino can be placed, false otherwise.
     */
    static bool can_place(const std::vector<std::vector<int> > &board,
                                        const std::vector<std::vector<int> > &placement,
                                        const Domino &domino,
                                        int x, int y,
                                        bool horizontal);

    /**
     * @brief Attempts to solve the domino puzzle.
     * @param board The game board.
     * @param placement The placement of dominos on the board.
     * @param dominoes The array of all dominos to be placed.
     * @param x The current row being considered in the solution.
     * @param y The current column being considered in the solution.
     * @return true if a solution is found, false otherwise.
     */
    static bool solve_puzzle(const std::vector<std::vector<int> > &board,
                             std::vector<std::vector<int> > &placement,
                             std::vector<Domino> &dominos,
                             int x, int y);

    static void* solve_puzzle_thread(void* arg);

    static bool get_solution_found();
};
