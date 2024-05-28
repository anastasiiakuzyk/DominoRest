#ifndef DOMINOREST_AUTH_HANDLER_H
#define DOMINOREST_AUTH_HANDLER_H

#include "crow.h"
#include <unordered_map>

namespace auth {

    struct User {
        std::string salt;
        std::string hashed_password;
    };

    bool authenticate(const std::string &token);

    crow::response register_route(const crow::request &req);

    crow::response login_route(const crow::request &req);

    crow::response create_dev_key_route(const crow::request &req);

    bool verifyDevKey(const std::string &dev_key);

}

#endif //DOMINOREST_AUTH_HANDLER_H
