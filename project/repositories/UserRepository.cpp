#include "UserRepositiry.h"
#include "../models/User.h"
#include "../database/Database.h"
#include "../external/sqlite/sqlite3.h"

#include <memory>
#include <stdexcept>
#include <string>

UserRepository::UserRepository(Database& database) : database(database) {}

std::int64_t UserRepository::addUser(const User& user){
    sqlite3* db = database.getConnection();

    const char* sql = "INSERT INTO users (name) VALUES (?);";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);

    if (result != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    std::string name = user.getName();

    result = sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    if (result != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    result = sqlite3_step(statement);

    if (result != SQLITE_DONE) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    return sqlite3_last_insert_rowid(db);
}
