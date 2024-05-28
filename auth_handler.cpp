#include "auth_handler.h"
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

namespace auth {

    std::unordered_map<std::string, User> users;
    std::unordered_map<std::string, std::string> active_tokens;
    std::unordered_map<std::string, std::string> dev_keys;

    std::string generateNonce(size_t length) {
        unsigned char buffer[length];
        if (RAND_bytes(buffer, length) != 1) {
            CROW_LOG_ERROR << "Failed to generate random bytes";
        }

        return crow::utility::base64encode(buffer, length);
    }

    std::string pbkdf2Sha256(const std::string &password, const std::string &salt, int iterations, int keyLen) {
        unsigned char key[keyLen];
        PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                          reinterpret_cast<const unsigned char *>(salt.c_str()), salt.length(),
                          iterations, EVP_sha256(), keyLen, key);
        return std::string(reinterpret_cast<char *>(key), keyLen);
    }

    std::string generateToken() {
        return generateNonce(32);
    }

    crow::response register_route(const crow::request &req) {
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400, "Bad Request: Unable to parse JSON.");

        std::string username = x["username"].s();
        std::string password = x["password"].s();

        if (users.find(username) != users.end()) {
            CROW_LOG_WARNING << "User " + username + " already exists.";
            return crow::response(400, "User " + username + "already exists.");
        }

        std::string salt = generateNonce(16);
        std::string hashed_password = pbkdf2Sha256(password, salt, 4096, 32);

        users[username] = {salt, hashed_password};

        CROW_LOG_INFO << "User " + username + " registered successfully.";
        return crow::response(200, "User " + username + " registered successfully.");
    }

    crow::response login_route(const crow::request &req) {
        auto x = crow::json::load(req.body);
        if (!x) {
            CROW_LOG_ERROR << "Bad Request: Unable to parse JSON.";
            return crow::response(400, "Bad Request: Unable to parse JSON.");
        }

        std::string username = x["username"].s();
        std::string password = x["password"].s();

        auto it = users.find(username);
        if (it == users.end()) {
            CROW_LOG_WARNING << "Invalid username or password.";
            return crow::response(400, "Invalid username or password.");
        }

        std::string salt = it->second.salt;
        std::string hashed_password = pbkdf2Sha256(password, salt, 4096, 32);

        if (hashed_password != it->second.hashed_password) {
            CROW_LOG_WARNING << "Invalid username or password.";
            return crow::response(400, "Invalid username or password.");
        }

        std::string token = generateToken();
        active_tokens[token] = username;

        crow::json::wvalue result;
        result["token"] = token;

        CROW_LOG_INFO << "User " + username + " logged in successfully.";
        return crow::response(200, result);
    }

    bool authenticate(const std::string &token) {
        return active_tokens.find(token) != active_tokens.end();
    }

    std::string generateDevKey() {
        return generateNonce(32);
    }

    crow::response create_dev_key_route(const crow::request &req) {
        std::string token = req.get_header_value("Authorization");

        if (!authenticate(token)) {
            CROW_LOG_ERROR << "Unauthorized: Invalid or missing token.";
            return crow::response(401, "Unauthorized: Invalid or missing token.");
        }

        std::string username = active_tokens[token];
        std::string dev_key = generateDevKey();
        dev_keys[dev_key] = username;

        crow::json::wvalue result;
        result["dev_key"] = dev_key;

        CROW_LOG_INFO << "Development key created for user " + username;
        return crow::response(200, result);
    }

    bool verifyDevKey(const std::string &dev_key) {
        return dev_keys.find(dev_key) != dev_keys.end();
    }
}