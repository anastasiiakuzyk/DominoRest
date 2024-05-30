#ifndef DOMINOREST_DB_HANDLER_H
#define DOMINOREST_DB_HANDLER_H

#include <string>
#include <sqlite3.h>

static const char *DB_PATH = "test.db";

int save_into_db(int cols, int rows, const std::string &board);

std::string get_board_by_id(int id);

sqlite3 *open_database();

void
execute_sql(sqlite3 *db, const std::string &sql, int (*callback)(void *, int, char **, char **), void *callback_arg);

int callback(void *data, int argc, char **argv, char **azColName);

#endif //DOMINOREST_DB_HANDLER_H
