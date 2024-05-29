#include "print_utils.h"

#include <vector>
#include <sstream>

/**
 * @file print_utils.cpp
 * @brief Implementation of utility functions for printing the domino puzzle board and solutions.
 */

/**
 * @brief Prints the domino board with an optional solution overlay.
 *
 * This function outputs a representation of the domino board to a string stream, including
 * the current state of the board with or without the solution shown, depending on the
 * showSolution parameter.
 *
 * @param board The game board.
 * @param placement The placement of dominos on the board, used to show the solution.
 * @param showSolution Whether to show the solution on the board.
 * @param output The output stream to write the board representation.
 */
void print_board_with_solution(const std::vector<std::vector<int> > &board,
                               const std::vector<std::vector<int> > &placement,
                               bool showSolution,
                               std::ostringstream &output) {
    int rows = board.size();
    int cols = board[0].size(); // Assuming all rows are of equal length

    // Top border
    for (int j = 0; j < cols; ++j) output << "+---";
    output << "+" << "\n";

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Handle placement visibility for solution
            if (showSolution) {
                output << ((j == 0 || placement[i][j] != placement[i][j - 1]) ? "| " : "  ");
            } else {
                output << ((j == 0) ? "| " : "  ");
            }
            output << board[i][j] << " ";
        }
        output << "|" << "\n"; // Right border

        // Inter-row separator or bottom border
        for (int j = 0; j < cols; ++j) {
            output << "+";
            if (i < rows - 1) {
                // If not the last row
                output << ((showSolution && placement[i][j] != placement[i + 1][j]) ? "---" : "   ");
            } else {
                // Last row, always draw bottom border
                output << "---";
            }
        }
        output << "+" << "\n";
    }
}

/**
 * @brief Prints the list of dominos and their positions on the board.
 *
 * Outputs a list of all dominos and their positions to the specified output stream. Each domino is represented
 * by its two side values. This is primarily used for debugging or demonstrating the available dominos.
 *
 * @param dominos The array of dominos.
 * @param output The output stream to write the dominos representation.
 */
void print_dominos(const std::vector<Domino> &dominos, std::ostringstream &output, int rows) {
    for (int i = 0; i < rows; ++i) {
        for (const Domino &domino: dominos) {
            if (domino.side2 == i) {
                output << "[" << domino.side1 << "|" << domino.side2 << "] ";
            }
        }

        output << "\n";
    }
}
