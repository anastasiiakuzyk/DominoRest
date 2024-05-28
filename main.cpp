#include "crow.h"
#include "puzzle_solver.h"
#include "print_utils.h"
#include "board_generator.h"
#include "utils.h"
#include "db_handler.h"
#include "auth_handler.h"
#include <sstream>
#include <vector>
#include <openssl/sha.h>
#include <stdexcept>
#include <iostream>

/**
 * @file main.cpp
 * @brief The entry point for the domino puzzle solver server.
 *
 * Sets up the Crow server and routes for solving domino puzzles.
 */

/**
 * @brief Sets up the web server routes.
 * @param app The Crow application instance.
 */
void setup_routes(crow::SimpleApp &app);

/**
 * @brief Solves the domino puzzle given a board configuration.
 * @param board The 2D array representing the domino puzzle board.
 * @return A Crow response object with the solution or an error message.
 */
crow::response solve_domino_puzzle(const std::vector<std::vector<int> > &board);

int main() {
    crow::SimpleApp app;
    setup_routes(app);
    app.port(18080).multithreaded().run();
}

namespace domino_solver {
    bool convert_json_to_board(std::vector<std::vector<int> > &board, const crow::json::rvalue &x) {
        try {
            size_t rows = x.size();
            board.resize(rows);

            for (size_t i = 0; i < rows; ++i) {
                const auto &row = x[i];
                size_t cols = row.size();
                board[i].resize(cols);

                for (size_t j = 0; j < cols; ++j) {
                    board[i][j] = row[j].i();
                }
            }
        } catch (const std::exception &e) {
            CROW_LOG_ERROR << "Failed to convert JSON to board: " << e.what();
            return false;
        }
        return true;
    }
}

/**
 * @brief Route for solving the domino puzzle.
 *
 * This route accepts a POST request with a JSON body representing the domino puzzle board.
 * The board is a 2D array of integers. The function attempts to solve the puzzle and returns
 * the solution or an error message.
 */
crow::response solve_route(const crow::request &req) {
    std::string token = req.get_header_value("Authorization");
    std::string dev_key = req.get_header_value("Dev-Key");

    if (!auth::authenticate(token) && !auth::verifyDevKey(dev_key)) {
        CROW_LOG_ERROR << "Unauthorized: Invalid or missing token/dev-key.";
        return crow::response(401, "Unauthorized: Invalid or missing token/dev-key.");
    }

    auto x = crow::json::load(req.body);
    if (!x) {
        CROW_LOG_ERROR << "Unable to parse JSON.";
        return crow::response(400, "Bad Request: Unable to parse JSON.");
    }

    std::vector<std::vector<int>> board;
    if (!domino_solver::convert_json_to_board(board, x)) {
        CROW_LOG_ERROR << "Failed to convert JSON to board.";
        return crow::response(400, "Bad Request: Invalid board dimensions or row length.");
    }

    return solve_domino_puzzle(board);
}

/**
 * @brief Route for generating a domino puzzle board.
 *
 * This route accepts a GET request with 'rows' and 'cols' parameters in the URL.
 * The function generates a random domino puzzle board of the specified size and returns
 * it as a JSON array.
 */
crow::response generate_board_route(const crow::request &req) {
    try {
        std::string rowsParam = req.url_params.get("rows");
        std::string colsParam = req.url_params.get("cols");

        if (rowsParam.empty() || colsParam.empty()) {
            CROW_LOG_ERROR << "Bad Request: 'rows' and 'cols' parameters are required.";
            return crow::response(400, "Bad Request: 'rows' and 'cols' parameters are required.");
        }

        int rows = std::stoi(rowsParam);
        int cols = std::stoi(colsParam);

        if (rows <= 0 || cols <= 0) {
            CROW_LOG_ERROR << "Bad Request: 'rows' and 'cols' must be positive integers.";
            return crow::response(400, "Bad Request: 'rows' and 'cols' must be positive integers.");
        }

        if (rows * cols % 2 != 0) {
            CROW_LOG_ERROR << "Bad Request: Size of board must be even.";
            return crow::response(400, "Bad Request: Size of board must be even.");
        }

        auto board = generate_board(rows, cols);
        crow::json::wvalue dto;
        for (size_t i = 0; i < board.size(); ++i) {
            for (size_t j = 0; j < board[i].size(); ++j) {
                dto["board"][i][j] = board[i][j];
            }
        }

        std::string string_board = dto["board"].dump();
        int board_id = db_handler::save_into_db(cols, rows, string_board);

        dto["id"] = board_id;

        if (board_id == -1) {
            CROW_LOG_ERROR << "Internal Server Error: Failed to save board.";
            return crow::response(500, "Internal Server Error: Failed to save board.");
        }
        return crow::response{dto};
    } catch (const std::invalid_argument &e) {
        CROW_LOG_ERROR << "Bad Request: 'rows' and 'cols' must be integers.";
        return crow::response(400, "Bad Request: 'rows' and 'cols' must be integers.");
    } catch (const std::out_of_range &e) {
        CROW_LOG_ERROR << "Bad Request: 'rows' and 'cols' values are out of range.";
        return crow::response(400, "Bad Request: 'rows' and 'cols' values are out of range.");
    } catch (const std::exception &e) {
        CROW_LOG_ERROR << "Server Error: Unable to generate board.";
        return crow::response(500, "Server Error: Unable to generate board.");
    }
}

crow::response generate_all_boards_route(const crow::request &req) {
    try {
        std::string rowsParam = req.url_params.get("rows");
        std::string colsParam = req.url_params.get("cols");

        if (rowsParam.empty() || colsParam.empty()) {
            CROW_LOG_ERROR << "Bad Request: 'rows' and 'cols' parameters are required.";
            return crow::response(400, "Bad Request: 'rows' and 'cols' parameters are required.");
        }

        int rows = std::stoi(rowsParam);
        int cols = std::stoi(colsParam);

        if (rows <= 0 || cols <= 0) {
            CROW_LOG_ERROR << "Bad Request: 'rows' and 'cols' must be positive integers.";
            return crow::response(400, "Bad Request: 'rows' and 'cols' must be positive integers.");
        }

        if (rows * cols % 2 != 0) {
            CROW_LOG_ERROR << "Bad Request: Size of board must be even.";
            return crow::response(400, "Bad Request: Size of board must be even.");
        }

        const std::vector<std::vector<std::vector<int>>> &allBoards = generate_all_boards(cols, rows);
        auto board = generate_board(rows, cols);
        crow::json::wvalue dto;
        for (size_t i = 0; i < allBoards.size(); ++i) {
            int board_id = db_handler::save_into_db(cols, rows, board_to_json_string(allBoards[i]));
            dto["boards"][i]["id"] = board_id;
        }
        return crow::response{dto};
    } catch (const std::invalid_argument &e) {
        CROW_LOG_ERROR << "Bad Request: 'rows' and 'cols' must be integers.";
        return crow::response(400, "Bad Request: 'rows' and 'cols' must be integers.");
    } catch (const std::out_of_range &e) {
        CROW_LOG_ERROR << "Bad Request: 'rows' and 'cols' values are out of range.";
        return crow::response(400, "Bad Request: 'rows' and 'cols' values are out of range.");
    } catch (const std::exception &e) {
        CROW_LOG_ERROR << "Server Error: Unable to generate board.";
        return crow::response(500, "Server Error: Unable to generate board.");
    }
}

crow::response get_board_by_id_route(int board_id) {
    std::string board_json = db_handler::get_board_by_id(board_id);
    if (board_json.empty()) {
        CROW_LOG_ERROR << "Not Found: Board does not exist.";
        return crow::response(404, "Not Found: Board does not exist.");
    }
    return crow::response{board_json};
}

void setup_routes(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::Post)(auth::register_route);
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::Post)(auth::login_route);
    CROW_ROUTE(app, "/solve").methods(crow::HTTPMethod::Post)(solve_route);
    CROW_ROUTE(app, "/generate_board").methods(crow::HTTPMethod::Get)(generate_board_route);
    CROW_ROUTE(app, "/get_board_by_id/<int>").methods(crow::HTTPMethod::Get)(get_board_by_id_route);
    CROW_ROUTE(app, "/generate_all_boards").methods(crow::HTTPMethod::Get)(generate_all_boards_route);
    CROW_ROUTE(app, "/create_dev_key").methods(crow::HTTPMethod::Post)(auth::create_dev_key_route);
}

crow::response solve_domino_puzzle(const std::vector<std::vector<int> > &board) {
    int rows = board.size();
    int cols = board.empty() ? 0 : board[0].size();

    std::vector placement(rows, std::vector<int>(cols, -1));

    int maxPips = find_max_pips(board);
    std::vector<Domino> dominos = generate_dominos(maxPips + 1);

    std::ostringstream output;
    output << "Domino Board:" << std::endl;
    print_board_with_solution(board, placement, false, output);

    output << std::endl << "Dominos:" << std::endl;
    print_dominos(dominos, output, maxPips + 1);

    bool solved = PuzzleSolver::solve_puzzle(board, placement, dominos, 0, 0);

    if (solved) {
        output << std::endl << "Solution:" << std::endl;
        print_board_with_solution(board, placement, true, output);
        CROW_LOG_INFO << "Solution found for the domino puzzle.";
        return crow::response{output.str()};
    } else {
        output << std::endl << "No solution exists." << std::endl;
        CROW_LOG_INFO << "No solution exists for the domino puzzle.";
        return crow::response{output.str()};
    }
}

