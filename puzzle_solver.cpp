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

struct ThreadData {
    const std::vector<std::vector<int>> *board;
    std::vector<std::vector<int>> *placement;
    std::vector<Domino> *dominos;
    int x;
    int y;
};

void *PuzzleSolver::solve_puzzle_thread(void *arg) {
    ThreadData *data = static_cast<ThreadData *>(arg);
    const auto &board = *data->board;
    auto &placement = *data->placement;
    auto &dominos = *data->dominos;
    int x = data->x;
    int y = data->y;


    // Initialize the mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        std::cerr << "Failed to initialize mutex" << std::endl;
    }
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
    if (solution_found) return true; // Early exit if solution is found

    int rows = board.size();
    int cols = board[0].size();

    if (y >= cols) {
        y = 0;
        x++;
        if (x >= rows) {
            return true;
        }
    }

    if (placement[x][y] != -1) {
        return solve_puzzle(board, placement, dominos, x, y + 1);
    }

    for (size_t i = 0; i < dominos.size() && !solution_found; ++i) {
        if (dominos[i].used) continue;

        // Try horizontal and vertical placement
        if (y + 1 < cols && can_place(board, placement, dominos[i], x, y, true)) {
            dominos[i].used = true;
            placement[x][y] = placement[x][y + 1] = i;

            ThreadData *data = new ThreadData{&board, &placement, &dominos, x, y + 1};
            pthread_t thread;
            if (pthread_create(&thread, nullptr, solve_puzzle_thread, data) == 0) {
                pthread_join(thread, nullptr);
            } else {
                delete data;
            }

            if (solution_found) return true;

            // Backtrack
            dominos[i].used = false;
            placement[x][y] = placement[x][y + 1] = -1;
        }

        // Vertical placement
        if (x + 1 < rows && can_place(board, placement, dominos[i], x, y, false)) {
            dominos[i].used = true;
            placement[x][y] = placement[x + 1][y] = i;

            ThreadData *data = new ThreadData{&board, &placement, &dominos, x + 1, y};
            pthread_t thread;
            if (pthread_create(&thread, nullptr, solve_puzzle_thread, data) == 0) {
                pthread_join(thread, nullptr);
            } else {
                delete data;
            }

            if (solution_found) return true;

            // Backtrack
            dominos[i].used = false;
            placement[x][y] = placement[x + 1][y] = -1;
        }
    }

    return false;
}
