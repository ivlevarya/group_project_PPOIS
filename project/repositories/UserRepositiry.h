#pragma once

#include <cstdint>

class Database;
class User;

class UserRepository {
private:
    Database& database;

public:
    explicit UserRepository(Database& database);

    std::int64_t addUser(const User& user);
};