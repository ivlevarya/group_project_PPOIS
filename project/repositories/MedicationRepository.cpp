
#include "MedicationRepository.h"

#include "../models/Medication.h"
#include "../database/Database.h"
#include "../external/sqlite/sqlite3.h"

#include <stdexcept>
#include <string>

MedicationRepository::MedicationRepository(Database& database) : database(database) {}

std::int64_t MedicationRepository::addMedication(const Medication& medication, std::int64_t patientId){
    sqlite3* db = database.getConnection();

    const char* sql = "INSERT INTO medications (patient_id, name, dosage, frequency, start_date, end_date) "
        "VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);

    if (result != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    std::string name = medication.getName();
    std::string dosage = medication.getDosage();
    std::string frequency = medication.getFrequency();
    std::string startDate = medication.getStartDate();
    std::string endDate = medication.getEndDate();

   
    int r1 = sqlite3_bind_int64(statement, 1, patientId);

    int r2 = sqlite3_bind_text(statement, 2, name.c_str(), -1, SQLITE_TRANSIENT);

    int r3 = sqlite3_bind_text(statement, 3, dosage.c_str(), -1, SQLITE_TRANSIENT);

    int r4 = sqlite3_bind_text(statement, 4, frequency.c_str(), -1, SQLITE_TRANSIENT);

    int r5 = sqlite3_bind_text(statement, 5, startDate.c_str(), -1, SQLITE_TRANSIENT);

    int r6 = sqlite3_bind_text(statement, 6, endDate.c_str(), -1, SQLITE_TRANSIENT);

    if (r1 != SQLITE_OK || r2 != SQLITE_OK || r3 != SQLITE_OK || r4 != SQLITE_OK || r5 != SQLITE_OK ||
        r6 != SQLITE_OK) {

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

    std::int64_t id = sqlite3_last_insert_rowid(db);

    result = sqlite3_finalize(statement);

    if (result != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    return id;
}


std::vector<Medication> MedicationRepository::getMedicationsByPatientId(std::int64_t patientId){

    sqlite3* db = database.getConnection();

    const char* sql = "SELECT name, dosage, frequency, start_date, end_date FROM medications "
        "WHERE patient_id = ? "
        "ORDER BY start_date DESC, id DESC;";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);

    if (result != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    result = sqlite3_bind_int64(statement, 1, patientId);

    if (result != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    std::vector<Medication> medications;

    while ((result = sqlite3_step(statement)) == SQLITE_ROW) {

        const unsigned char* nameText = sqlite3_column_text(statement, 0);

        const unsigned char* dosageText = sqlite3_column_text(statement, 1);

        const unsigned char* frequencyText = sqlite3_column_text(statement, 2);

        const unsigned char* startText = sqlite3_column_text(statement, 3);

        const unsigned char* endText = sqlite3_column_text(statement, 4);

        if (nameText == nullptr || dosageText == nullptr || frequencyText == nullptr || startText == nullptr ||
            endText == nullptr) {

            sqlite3_finalize(statement);

            throw std::runtime_error("Invalid medication data");
        }

        std::string name = reinterpret_cast<const char*>(nameText);

        std::string dosage = reinterpret_cast<const char*>(dosageText);

        std::string frequency = reinterpret_cast<const char*>(frequencyText);

        std::string startDate = reinterpret_cast<const char*>(startText);

        std::string endDate = reinterpret_cast<const char*>(endText);

        medications.emplace_back(name,dosage,frequency,startDate,endDate);
    }

    if (result != SQLITE_DONE) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    result = sqlite3_finalize(statement);

    if (result != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    return medications;
}
