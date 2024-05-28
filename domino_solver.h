#pragma once

#include <crow/json.h>
#include "puzzle_solver.h"

/**
 * @file domino_solver.h
 * @brief Contains the function to convert JSON input into a board array for the domino puzzle.
 */

/**
 * @brief Converts a JSON array to a 2D board array.
 * @param board The 2D array to fill with board values.
 * @param x The JSON representation of the board.
 * @return true if conversion is successful, false otherwise.
 */
bool convert_json_to_board(std::vector<std::vector<int> > &board, const crow::json::rvalue &x);
