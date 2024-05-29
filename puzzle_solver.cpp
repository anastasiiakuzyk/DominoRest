#include "puzzle_solver.h"

#include <pthread.h>
#include <vector>
#include <iostream>
#include <chrono>

/**
 * @file puzzle_solver.cpp
 * @brief Implementation of PuzzleSolver class methods.
 */

// Mutex for shared data protection
pthread_mutex_t mutex;
// Shared variable to stop threads when solution is found
volatile bool solution_found = false;

bool PuzzleSolver::get_solution_found() {
    return solution_found;
}

void *PuzzleSolver::solve_puzzle_thread(void *arg) {
    ThreadData *data = static_cast<ThreadData *>(arg);
    const auto &board = *data->board;
    auto &placement = *data->placement;
    auto &dominos = *data->dominos;
    int x = data->x;
    int y = data->y;

    pthread_mutex_init(&mutex, nullptr);
    pthread_mutex_lock(&mutex);
    bool result = solve_puzzle(board, placement, dominos, x, y);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_destroy(&mutex);
    if (result) {
        solution_found = true;
    }

    delete data;
    return nullptr;
}


/**
 * @brief Checks if a domino can be placed at the specified position on the board.
 *
 * This method determines whether a domino can be placed at a given position (x, y) on the board,
 * considering the orientation (horizontal or vertical) and ensuring it does not overlap with already placed dominos.
 *
 * @param board The current game board.
 * @param placement The array representing the placement of dominos on the board.
 * @param domino The domino to check placement for.
 * @param x The x-coordinate (row) on the board.
 * @param y The y-coordinate (column) on the board.
 * @param horizontal A boolean indicating the orientation of the domino (true for horizontal).
 * @return true if the domino can be placed, false otherwise.
 */
bool PuzzleSolver::can_place(const std::vector<std::vector<int>> &board,
                             const std::vector<std::vector<int>> &placement,
                             const Domino &domino,
                             int x, int y,
                             bool horizontal) {
    int rows = board.size();
    int cols = board[0].size();
    if (horizontal) {
        if (y + 1 >= cols || placement[x][y] != -1 || placement[x][y + 1] != -1)
            return false;
        return (board[x][y] == domino.side1 && board[x][y + 1] == domino.side2) ||
               (board[x][y] == domino.side2 && board[x][y + 1] == domino.side1);
    } else {
        if (x + 1 >= rows || placement[x][y] != -1 || placement[x + 1][y] != -1)
            return false;
        return (board[x][y] == domino.side1 && board[x + 1][y] == domino.side2) ||
               (board[x][y] == domino.side2 && board[x + 1][y] == domino.side1);
    }
}

/**
 * @brief Tries to solve the domino puzzle by placing dominos on the board recursively.
 *
 * This method attempts to solve the puzzle by recursively placing each domino in the correct position on the board.
 * It uses backtracking to explore all possible placements until a solution is found or all options are exhausted.
 *
 * @param board The game board.
 * @param placement The array to record the placement of each domino.
 * @param dominoes The array of all dominos available for the puzzle.
 * @param x The current x-coordinate (row) being considered.
 * @param y The current y-coordinate (column) being considered.
 * @return true if the puzzle is solved, false if no solution is found.
 */
bool PuzzleSolver::solve_puzzle(const std::vector<std::vector<int>> &board,
                                std::vector<std::vector<int>> &placement,
                                std::vector<Domino> &dominos,
                                int x, int y) {
    int rows = board.size();
    int cols = board[0].size();

    if (x >= rows) return true; // Reached the end of the board

    if (y >= cols) {
        return solve_puzzle(board, placement, dominos, x + 1, 0); // Move to the next row
    }

    if (placement[x][y] != -1) {
        return solve_puzzle(board, placement, dominos, x, y + 1); // Skip filled cell
    }

    for (Domino &domino: dominos) {
        if (domino.used) continue;

        // Try horizontal placement
        if (can_place(board, placement, domino, x, y, true)) {
            domino.used = true;
            placement[x][y] = placement[x][y +
                                           1] = domino.side1; // Use domino index or a unique identifier instead of side1
            if (solve_puzzle(board, placement, dominos, x, y + 2)) return true;
            placement[x][y] = placement[x][y + 1] = -1;
            domino.used = false;
        }

        // Try vertical placement
        if (can_place(board, placement, domino, x, y, false)) {
            domino.used = true;
            placement[x][y] = placement[x +
                                        1][y] = domino.side1; // Use domino index or a unique identifier instead of side1
            if (solve_puzzle(board, placement, dominos, x, y + 1)) return true;
            placement[x][y] = placement[x + 1][y] = -1;
            domino.used = false;
        }
    }

    return false; // No placement found
}
