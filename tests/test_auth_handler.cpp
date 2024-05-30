#include <gtest/gtest.h>
#include <crow.h>
#include <unordered_map>
#include "auth_handler.h"

extern std::unordered_map<std::string, User> users;
extern std::unordered_map<std::string, std::string> active_tokens;
extern std::unordered_map<std::string, std::string> dev_keys;

// Test for generateNonce function
TEST(AuthHandlerTest, GenerateNonceLength) {
    std::string nonce = generateNonce(32);
    ASSERT_EQ(nonce.size(), 44);  // Base64 encoding increases size
}

TEST(AuthHandlerTest, GenerateNonceDifferentValues) {
    std::string nonce1 = generateNonce(32);
    std::string nonce2 = generateNonce(32);
    ASSERT_NE(nonce1, nonce2);
}

// Test for pbkdf2Sha256 function
TEST(AuthHandlerTest, PBKDF2Sha256) {
    std::string password = "password";
    std::string salt = "salt";
    std::string hashed = pbkdf2Sha256(password, salt, 4096, 32);
    ASSERT_EQ(hashed.size(), 32);
}

// Test for register_route function
TEST(AuthHandlerTest, RegisterRouteSuccess) {
    crow::request req;
    req.body = R"({"username":"testuser","password":"password"})";
    auto res = register_route(req);
    ASSERT_EQ(res.code, 200);
    ASSERT_EQ(users.size(), 1);
    ASSERT_EQ(users["testuser"].salt.size(), 24);  // 16 bytes base64 encoded
    ASSERT_EQ(users["testuser"].hashed_password.size(), 32);
}

TEST(AuthHandlerTest, RegisterRouteUserExists) {
    crow::request req1;
    req1.body = R"({"username":"testuser","password":"password"})";
    register_route(req1);

    crow::request req2;
    req2.body = R"({"username":"testuser","password":"password"})";
    auto res = register_route(req2);
    ASSERT_EQ(res.code, 400);
}

// Test for login_route function
TEST(AuthHandlerTest, LoginRouteSuccess) {
    // Register user first
    crow::request req1;
    req1.body = R"({"username":"testuser","password":"password"})";
    register_route(req1);

    // Login with registered user
    crow::request req2;
    req2.body = R"({"username":"testuser","password":"password"})";
    auto res = login_route(req2);
    ASSERT_EQ(res.code, 200);
    ASSERT_EQ(active_tokens.size(), 1);
}

TEST(AuthHandlerTest, LoginRouteInvalidUser) {
    crow::request req;
    req.body = R"({"username":"invaliduser","password":"password"})";
    auto res = login_route(req);
    ASSERT_EQ(res.code, 400);
}

// Test for authenticate function
TEST(AuthHandlerTest, AuthenticateSuccess) {
    std::string token = generateToken();
    active_tokens[token] = "testuser";
    ASSERT_TRUE(authenticate(token));
}

TEST(AuthHandlerTest, AuthenticateFailure) {
    std::string token = "invalidtoken";
    ASSERT_FALSE(authenticate(token));
}

// Test for generateDevKey function
TEST(AuthHandlerTest, GenerateDevKeyLength) {
    std::string dev_key = generateDevKey();
    ASSERT_EQ(dev_key.size(), 44);  // Base64 encoding increases size
}

// Test for create_dev_key_route function
TEST(AuthHandlerTest, CreateDevKeyRouteSuccess) {
    std::string token = generateToken();
    active_tokens[token] = "testuser";

    crow::request req;
    req.add_header("Authorization", token);

    auto res = create_dev_key_route(req);
    ASSERT_EQ(res.code, 200);
    ASSERT_EQ(dev_keys.size(), 1);
}

TEST(AuthHandlerTest, CreateDevKeyRouteUnauthorized) {
    crow::request req;
    req.add_header("Authorization", "invalidtoken");

    auto res = create_dev_key_route(req);
    ASSERT_EQ(res.code, 401);
}

// Test for verifyDevKey function
TEST(AuthHandlerTest, VerifyDevKeySuccess) {
    std::string dev_key = generateDevKey();
    dev_keys[dev_key] = "testuser";
    ASSERT_TRUE(verifyDevKey(dev_key));
}

TEST(AuthHandlerTest, VerifyDevKeyFailure) {
    std::string dev_key = "invalidkey";
    ASSERT_FALSE(verifyDevKey(dev_key));
}

TEST(AuthHandlerTest, LoginRouteInvalidPassword) {
    // Register user first
    crow::request req1;
    req1.body = R"({"username":"testuser","password":"password"})";
    register_route(req1);

    // Attempt login with incorrect password
    crow::request req2;
    req2.body = R"({"username":"testuser","password":"wrongpassword"})";
    auto res = login_route(req2);
    ASSERT_EQ(res.code, 400);
    ASSERT_EQ(res.body, "Invalid username or password.");
}