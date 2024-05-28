#ifndef DOMINOREST_DB_HANDLER_H
#define DOMINOREST_DB_HANDLER_H

#include <string>
#include <sqlite3.h>

namespace db_handler {

    int save_into_db(int cols, int rows, const std::string& board);

    std::string get_board_by_id(int id);

}

#endif //DOMINOREST_DB_HANDLER_H
