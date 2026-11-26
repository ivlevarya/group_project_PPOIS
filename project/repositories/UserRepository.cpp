#include "UserRepositiry.h"
#include "../models/User.h"
#include "../database/Database.h"
#include "../external/sqlite/sqlite3.h"

#include <stdexcept>
#include <string>
#include <optional>
#include <cstdint>

UserRepository::UserRepository(Database& database) : database(database) {}

std::int64_t UserRepository::addUser(const User& user){
    sqlite3* db = database.getConnection();

    const char* sql = "INSERT INTO users (first_name, last_name, email, password_hash) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);

    if (result != SQLITE_OK) {
        sqlite3_finalize(statement);
        throw std::runtime_error(sqlite3_errmsg(db));
    }


    result = sqlite3_bind_text(statement, 1, user.getFirstName().c_str(), -1, SQLITE_TRANSIENT);

    if (result == SQLITE_OK) {
        result = sqlite3_bind_text(statement, 2, user.getLastName().c_str(), -1, SQLITE_TRANSIENT);
    }

    if (result == SQLITE_OK) {
        result = sqlite3_bind_text(statement, 3, user.getEmail().c_str(), -1, SQLITE_TRANSIENT);
    }

    if (result == SQLITE_OK) {
        result = sqlite3_bind_text(statement, 4, user.getPasswordHash().c_str(), -1, SQLITE_TRANSIENT);
    }

    if (result != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    result = sqlite3_step(statement);

    if (result != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    std::int64_t userId = sqlite3_last_insert_rowid(db);

    sqlite3_finalize(statement);

    return userId;
}


std::optional<UserWithId> UserRepository::getUserByEmail(const std::string& email) {
    sqlite3* db = database.getConnection();

    const char* sql = "SELECT id, first_name, last_name, email, password_hash FROM users WHERE email = ?;";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);

    if (result != SQLITE_OK) {
        sqlite3_finalize(statement);
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    result = sqlite3_bind_text(statement, 1, email.c_str(), -1, SQLITE_TRANSIENT);

    if (result != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    result = sqlite3_step(statement);

    if (result == SQLITE_DONE) {
        sqlite3_finalize(statement);
        return std::nullopt;
    }

    if (result != SQLITE_ROW) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    try {
        std::int64_t id = sqlite3_column_int64(statement, 0);

        std::string firstName = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));

        std::string lastName = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));

        std::string userEmail = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));

        std::string passwordHash = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));

        UserWithId resultUser{id, User(firstName, lastName, userEmail, passwordHash)};

        sqlite3_finalize(statement);

        return resultUser;
    }
    catch (...) {
        sqlite3_finalize(statement);
        throw;
    }
}

