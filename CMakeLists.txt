cmake_minimum_required(VERSION 3.27.8)
project(DominoRest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(/opt/homebrew/Cellar/asio/1.28.1/include)
link_directories(/opt/homebrew/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/crow/include)

# Add the Google Test subdirectory
add_subdirectory(libs/googletest)

# Include Google Test and your project headers
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Create the main executable
add_executable(${PROJECT_NAME}
        main.cpp
        puzzle_solver.cpp
        domino.cpp
        print_utils.cpp
        board_generator.cpp
        utils.cpp
        db_handler.cpp
        db_handler.h
        auth_handler.cpp
        auth_handler.h
)

# Create the test executable
add_executable(DominoRestTests
        tests/test_puzzle_solver.cpp
        tests/test_print_utils.cpp
        tests/test_board_generator.cpp
        tests/test_utils.cpp
        puzzle_solver.cpp
        domino.cpp
        print_utils.cpp
        board_generator.cpp
        utils.cpp
)

# Link test executable with GoogleTest
target_link_libraries(DominoRestTests gtest gtest_main)
find_package(OpenSSL REQUIRED)

# Find SQLite
find_package(SQLite3 REQUIRED)
include_directories(${SQLite3_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)

# Link SQLite3 library
target_link_libraries(${PROJECT_NAME} ${SQLite3_LIBRARIES})