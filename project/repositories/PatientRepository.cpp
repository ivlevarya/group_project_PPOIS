#include "PatientRepository.h"
#include "../database/Database.h"
#include "../models/Patient.h"
#include "../external/sqlite/sqlite3.h"

#include <stdexcept>
#include <string>

PatientRepository::PatientRepository(Database& database) : database(database) {}

std::int64_t PatientRepository::addPatient(const Patient& patient) {
    sqlite3* db = database.getConnection();

    const char* sql = "INSERT INTO patients (name, birth_year) VALUES (?, ?);";

    sqlite3_stmt* statement = nullptr;

    // Подготавливаем SQL-запрос
    int result = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);

    if (result != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    // Передаем значения в запрос
    std::string name = patient.getName();

    int nameResult = sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    int yearResult = sqlite3_bind_int(statement, 2, patient.getBirthYear());

    if (nameResult != SQLITE_OK || yearResult != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    // Выполняем запрос
    result = sqlite3_step(statement);

    if (result != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    // Получаем номер новой записи
    std::int64_t id = sqlite3_last_insert_rowid(db);

    // Освобождаем ресурсы запроса
    result = sqlite3_finalize(statement);

    if (result != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    return id;
}