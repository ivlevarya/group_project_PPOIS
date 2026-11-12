#include "Database.h"
#include "../external/sqlite/sqlite3.h"

#include <stdexcept>
#include <string>

Database::Database(const std::string& path) : connection(nullptr) {
    int result = sqlite3_open_v2(path.c_str(), &connection, SQLITE_OPEN_READWRITE, nullptr);

    if (result != SQLITE_OK) {
        std::string message = connection ? sqlite3_errmsg(connection) : "Unknown SQLite error";

        if (connection != nullptr) {
            sqlite3_close(connection);
            connection = nullptr;
        }

        throw std::runtime_error("Database connection error: " + message);
    }
    sqlite3_busy_timeout(connection, 5000);
}

Database::~Database()
{
    if (connection != nullptr) {
        sqlite3_close(connection);
    }
}

sqlite3* Database::getConnection() const {
    return connection;
}