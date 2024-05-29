#include "db_handler.h"
#include "crow/logging.h"
#include <iostream>
#include <sstream>

namespace db_handler {

    static int callback(void *data, int argc, char **argv, char **azColName) {
        auto *result = static_cast<std::string *>(data);
        if (argc > 0 && argv[0]) {
            *result = argv[0];
        }
        return 0;
    }

    sqlite3* open_database() {
        sqlite3 *db;
        if (sqlite3_open(DB_PATH, &db) != SQLITE_OK) {
            CROW_LOG_ERROR << "Can't open database: " << sqlite3_errmsg(db);
            sqlite3_close(db);
            return nullptr;
        }
        return db;
    }

    void execute_sql(sqlite3* db, const std::string& sql, int (*callback)(void*, int, char**, char**), void* callback_arg) {
        char *zErrMsg = 0;
        if (sqlite3_exec(db, sql.c_str(), callback, callback_arg, &zErrMsg) != SQLITE_OK) {
            CROW_LOG_ERROR << "SQL error: " << zErrMsg;
            sqlite3_free(zErrMsg);
        }
    }

    int save_into_db(int cols, int rows, const std::string &board) {
        int last_id = -1;
        sqlite3* db = open_database();
        if (!db) return last_id;

        const char *create_sql = "CREATE TABLE IF NOT EXISTS BOARD(" \
                                 "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                                 "COLS INT NOT NULL," \
                                 "ROWS INT NOT NULL," \
                                 "BOARD TEXT NOT NULL);";
        execute_sql(db, create_sql, nullptr, nullptr);

        std::ostringstream oss;
        oss << "INSERT INTO BOARD (COLS, ROWS, BOARD) VALUES ("
            << cols << ", " << rows << ", '" << board << "');";
        execute_sql(db, oss.str(), nullptr, nullptr);

        last_id = static_cast<int>(sqlite3_last_insert_rowid(db));
        sqlite3_close(db);
        return last_id;
    }

    std::string get_board_by_id(int id) {
        sqlite3* db = open_database();
        if (!db) return "";

        std::ostringstream oss;
        oss << "SELECT BOARD FROM BOARD WHERE ID = " << id << ";";
        std::string result;
        execute_sql(db, oss.str(), callback, &result);

        sqlite3_close(db);
        return result;
    }

}