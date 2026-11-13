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

std::optional<Patient> PatientRepository::getPatientById(std::int64_t id) {

    sqlite3* db = database.getConnection();

    const char* sql = "SELECT name, birth_year FROM patients WHERE id = ?;";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);

    if (result != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    // Передаем идентификатор пациента
    result = sqlite3_bind_int64(statement, 1, id);

    if (result != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    // Выполняем запрос
    result = sqlite3_step(statement);

    if (result == SQLITE_ROW) {
        // Получаем значения столбцов
        const unsigned char* nameText = sqlite3_column_text(statement, 0);

        if (nameText == nullptr) {
            sqlite3_finalize(statement);
            throw std::runtime_error("Invalid patient name");
        }

        std::string name = reinterpret_cast<const char*>(nameText);

        int birthYear = sqlite3_column_int(statement, 1);

        // Создаем объект пациента
        Patient patient(name, birthYear);

        sqlite3_finalize(statement);

        return patient;
    }

    if (result == SQLITE_DONE) {
        sqlite3_finalize(statement);
        return std::nullopt;
    }

    std::string error = sqlite3_errmsg(db);
    sqlite3_finalize(statement);
    throw std::runtime_error(error);
}

bool PatientRepository::updatePatient(std::int64_t id, const Patient& patient) {
    sqlite3* db = database.getConnection();

    const char* sql = "UPDATE patients SET name = ?, birth_year = ? WHERE id = ?;";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);

    if (result != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    // Подставляем новые значения
    std::string name = patient.getName();

    int r1 = sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    int r2 = sqlite3_bind_int(statement, 2, patient.getBirthYear());

    int r3 = sqlite3_bind_int64(statement, 3, id);

    if (r1 != SQLITE_OK || r2 != SQLITE_OK || r3 != SQLITE_OK) {

        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    // Выполняем обновление
    result = sqlite3_step(statement);

    if (result != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    // Проверяем, затронута ли запись
    bool updated = sqlite3_changes(db) > 0;

    result = sqlite3_finalize(statement);

    if (result != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    return updated;
}


bool PatientRepository::deletePatient(std::int64_t id) {
    sqlite3* db = database.getConnection();

    const char* sql = "DELETE FROM patients WHERE id = ?;";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);

    if (result != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    // Указываем ID удаляемого пациента
    result = sqlite3_bind_int64(statement, 1, id);

    if (result != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    // Выполняем удаление
    result = sqlite3_step(statement);

    if (result != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    bool deleted = sqlite3_changes(db) > 0;

    result = sqlite3_finalize(statement);

    if (result != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    return deleted;
}
