#include "board_generator.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <chrono>

std::vector<Domino> generate_dominos(int max_pips) {
    std::vector<Domino> dominos;
    for (int i = 0; i <= max_pips; ++i) {
        for (int j = i; j <= max_pips; ++j) {
            dominos.emplace_back(i, j);
        }
    }
    return dominos;
}

void shuffle_dominos(std::vector<Domino> &dominos) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(dominos.begin(), dominos.end(), std::default_random_engine(seed));
}

void place_dominos_on_board(std::vector<std::vector<int>> &board, const std::vector<Domino> &dominos, int &domino_index) {
    int rows = board.size();
    int cols = board[0].size();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; j += 2) {
            if (domino_index >= dominos.size()) {
                throw std::runtime_error("Ran out of dominos to place on the board.");
            }
            board[i][j] = dominos[domino_index].side1;
            if (j + 1 < cols) {
                board[i][j + 1] = dominos[domino_index].side2;
            }
            ++domino_index;
        }
    }
}

std::string board_to_json_string(const std::vector<std::vector<int>> &vec) {
    std::string result = "[";
    for (int i = 0; i < vec.size(); ++i) {
        if (i > 0) result += ",";
        result += "[";
        for (int j = 0; j < vec[i].size(); ++j) {
            if (j > 0) result += ",";
            result += std::to_string(vec[i][j]);
        }
        result += "]";
    }
    result += "]";
    return result;
}

std::vector<std::vector<int>> generate_board(int rows, int cols) {
    int max_pips = std::max(rows, cols) - 1;
    auto dominos = generate_dominos(max_pips);
    shuffle_dominos(dominos);
    std::vector<std::vector<int>> board(rows, std::vector<int>(cols, -1));
    int domino_index = 0;
    place_dominos_on_board(board, dominos, domino_index);
    return board;
}

std::vector<std::vector<std::vector<int>>> generate_all_boards(int rows, int cols) {
    int max_pips = std::max(rows, cols) - 1;
    auto dominos = generate_dominos(max_pips);

    std::set<std::string> unique_boards;
    std::vector<std::vector<std::vector<int>>> all_boards;

    do {
        std::vector<std::vector<int>> board(rows, std::vector<int>(cols, -1));
        int domino_index = 0;
        place_dominos_on_board(board, dominos, domino_index);

        std::string board_str = board_to_json_string(board);
        if (unique_boards.find(board_str) == unique_boards.end()) {
            unique_boards.insert(board_str);
            all_boards.push_back(board);
        }

    } while (std::next_permutation(dominos.begin(), dominos.end(), [](const Domino &a, const Domino &b) {
        return std::tie(a.side1, a.side2) < std::tie(b.side1, b.side2);
    }));

    return all_boards;
}
