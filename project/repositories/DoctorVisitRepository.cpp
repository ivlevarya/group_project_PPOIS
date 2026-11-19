
#include "DoctorVisitRepository.h"

#include "../models/DoctorVisit.h"
#include "../database/Database.h"
#include "../external/sqlite/sqlite3.h"

#include <stdexcept>
#include <string>

DoctorVisitRepository::DoctorVisitRepository(Database& database): database(database) {}

std::int64_t DoctorVisitRepository::addDoctorVisit(const DoctorVisit& visit, std::int64_t patientId){
    sqlite3* db = database.getConnection();

    const char* sql = "INSERT INTO doctor_visits (patient_id, date, doctor_name, clinic_name, reason)"
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);

    if (result != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    std::string date = visit.getDate();
    std::string doctorName = visit.getDoctorName();
    std::string clinicName = visit.getClinicName();
    std::string reason = visit.getReason();

    int r1 = sqlite3_bind_int64(statement, 1, patientId);

    int r2 = sqlite3_bind_text(statement, 2, date.c_str(), -1, SQLITE_TRANSIENT);

    int r3 = sqlite3_bind_text(statement, 3, doctorName.c_str(), -1, SQLITE_TRANSIENT);

    int r4 = sqlite3_bind_text(statement, 4, clinicName.c_str(), -1, SQLITE_TRANSIENT);

    int r5 = sqlite3_bind_text(statement, 5, reason.c_str(), -1, SQLITE_TRANSIENT);

    if (r1 != SQLITE_OK || r2 != SQLITE_OK || r3 != SQLITE_OK || r4 != SQLITE_OK || r5 != SQLITE_OK) {
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


std::vector<DoctorVisit> DoctorVisitRepository::getDoctorVisitsByPatientId(std::int64_t patientId){
    sqlite3* db = database.getConnection();

    const char* sql = "SELECT date, doctor_name, clinic_name, reason FROM doctor_visits "
        "WHERE patient_id = ? "
        "ORDER BY date DESC, id DESC;";

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

    std::vector<DoctorVisit> visits;

    while ((result = sqlite3_step(statement)) == SQLITE_ROW) {

        const unsigned char* dateText = sqlite3_column_text(statement, 0);

        const unsigned char* doctorText = sqlite3_column_text(statement, 1);

        const unsigned char* clinicText = sqlite3_column_text(statement, 2);

        const unsigned char* reasonText = sqlite3_column_text(statement, 3);

        if (dateText == nullptr || doctorText == nullptr || clinicText == nullptr || reasonText == nullptr) {

            sqlite3_finalize(statement);

            throw std::runtime_error(
                "Invalid doctor visit data"
            );
        }

        std::string date = reinterpret_cast<const char*>(dateText);

        std::string doctorName = reinterpret_cast<const char*>(doctorText);

        std::string clinicName = reinterpret_cast<const char*>(clinicText);

        std::string reason = reinterpret_cast<const char*>(reasonText);

        visits.emplace_back(date, doctorName,clinicName,reason);
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

    return visits;
}

