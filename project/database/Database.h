#pragma once

#include <string>

struct sqlite3;

class Database {
private:
	sqlite3* connection;
public:
	Database(const std::string& path);
	~Database();
	// Запрещаем копирование соединения
	Database(const Database&) = delete;
	Database& operator=(const Database&) = delete;
	sqlite3* getConnection() const;
};