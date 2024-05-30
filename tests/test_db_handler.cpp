#include <gtest/gtest.h>
#include "db_handler.h"
#include <fstream>
#include <unistd.h>
#include <fcntl.h>

TEST(DBHandlerTest, OpenDatabaseSuccess) {
    sqlite3 *db = open_database();
    EXPECT_NE(db, nullptr);
    sqlite3_close(db);
}

TEST(DBHandlerTest, GetBoardByIdNotFound) {
    int id = 999; // Non-existent ID
    std::string retrieved_board = get_board_by_id(id);
    EXPECT_EQ(retrieved_board, "");
}

void CorruptDatabaseFile() {
    // Create a corrupted database file
    std::ofstream file(DB_PATH, std::ios::out | std::ios::trunc);
    file << "corrupted data";
    file.close();
}

TEST(DBHandlerTest, SaveIntoDBFailure) {
    CorruptDatabaseFile();

    int cols = 5;
    int rows = 5;
    std::string board = "XXXXXOOOOOXXXXXOOOOOXXXXX";

    int id = save_into_db(cols, rows, board);
    EXPECT_EQ(id, 0);
}

TEST(DBHandlerTest, GetBoardByIdFailure) {
    CorruptDatabaseFile();

    int id = 1; // Any ID should work
    std::string retrieved_board = get_board_by_id(id);
    ASSERT_EQ(retrieved_board, "");
}

TEST(DBHandlerTest, ExecuteSQLFailure) {
    sqlite3* db = open_database();
    ASSERT_NE(db, nullptr);

    // Invalid SQL statement to induce an error
    const std::string invalid_sql = "INVALID SQL STATEMENT";
    ASSERT_NO_THROW(execute_sql(db, invalid_sql, nullptr, nullptr));

    // There should be an error logged by CROW_LOG_ERROR, but since we can't check logs in a unit test,
    // we assume the function handles the error without throwing.
    sqlite3_close(db);
}

// Test for the callback function
TEST(DBHandlerTest, CallbackFunctionSuccess) {
    std::string data;
    char *argv[] = { const_cast<char*>("test_value") };
    int result = callback(&data, 1, argv, nullptr);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(data, "test_value");
}

TEST(DBHandlerTest, CallbackFunctionNoArgs) {
    std::string data;
    int result = callback(&data, 0, nullptr, nullptr);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(data, "");
}

TEST(DBHandlerTest, CallbackFunctionNullValue) {
    std::string data;
    char *argv[] = { nullptr };
    int result = callback(&data, 1, argv, nullptr);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(data, "");
}

// Test for the open_database function
TEST(DBHandlerTest, OpenDatabaseSuccessMY) {
    sqlite3* db = open_database();
    ASSERT_NE(db, nullptr);
    sqlite3_close(db);
}

TEST(DBHandlerTest, OpenDatabaseCloseFailure) {
    // Create a corrupted database file
    int fd = open(DB_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        write(fd, "corrupted data", 14);
        close(fd);
    }

    sqlite3* db = open_database();
    ASSERT_NE(db, nullptr);
}