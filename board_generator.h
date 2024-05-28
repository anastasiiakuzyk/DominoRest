#pragma once

#include "domino.h"
#include <vector>
#include <string>

std::vector<Domino> generate_dominos(int max_pips);

std::string board_to_json_string(const std::vector<std::vector<int>> &vec);

std::vector<std::vector<int>> generate_board(int rows, int cols);

std::vector<std::vector<std::vector<int>>> generate_all_boards(int rows, int cols);
