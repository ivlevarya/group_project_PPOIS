#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include "../models/User.h"

class Database;


struct UserWithId {
    std::int64_t id;
    User user;
};

class UserRepository {
private:
    Database& database;

public:
    explicit UserRepository(Database& database);

    std::int64_t addUser(const User& user);

    std::optional<UserWithId> getUserByEmail(const std::string& email);
};