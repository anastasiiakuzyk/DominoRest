#ifndef DOMINOREST_AUTH_HANDLER_H
#define DOMINOREST_AUTH_HANDLER_H

#include "crow.h"
#include <unordered_map>

struct User {
    std::string salt;
    std::string hashed_password;
};

std::string generateNonce(size_t length);

std::string pbkdf2Sha256(const std::string &password, const std::string &salt, int iterations, int keyLen);

bool authenticate(const std::string &token);

crow::response register_route(const crow::request &req);

crow::response login_route(const crow::request &req);

crow::response create_dev_key_route(const crow::request &req);

bool verifyDevKey(const std::string &dev_key);

std::string generateToken();

std::string generateDevKey();

#endif //DOMINOREST_AUTH_HANDLER_H
