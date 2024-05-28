#pragma once

#include <vector>
#include <sstream>
#include "domino.h"

/**
 * @file print_utils.h
 * @brief Provides utilities for printing the domino puzzle board and solutions.
 */

/**
 * @brief Prints the domino board with optional solution.
 * @param board The game board to print.
 * @param placement The placement of dominos on the board, indicating the solution.
 * @param showSolution Whether to show the solution in the output.
 * @param output The output stream to write the board representation.
 */
void print_board_with_solution(const std::vector<std::vector<int>>& board,
                               const std::vector<std::vector<int>>& placement,
                               bool showSolution,
                               std::ostringstream &output);

/**
 * @brief Prints the dominos and their placements.
 * @param dominos The array of dominos to print.
 * @param output The output stream to write the dominos representation.
 */
void print_dominos(const std::vector<Domino>& dominos, std::ostringstream &output, int rows);

void print_dominos(const std::vector<Domino> &dominos, int rows);