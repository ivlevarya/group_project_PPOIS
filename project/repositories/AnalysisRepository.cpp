
#include "AnalysisRepository.h"

#include "../models/Analysis.h"
#include "../database/Database.h"
#include "../external/sqlite/sqlite3.h"

#include <stdexcept>
#include <string>

AnalysisRepository::AnalysisRepository(Database& database) : database(database) {}

std::int64_t AnalysisRepository::addAnalysis(const Analysis& analysis,std::int64_t patientId){
    sqlite3* db = database.getConnection();

    const char* sql = "INSERT INTO analyses (patient_id, date, name, result) VALUES (?, ?, ?, ?);";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);

    if (result != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }

    std::string date = analysis.getDate();
    std::string name = analysis.getName();
    std::string analysisResult = analysis.getResult();

    int r1 = sqlite3_bind_int64(statement, 1, patientId);

    int r2 = sqlite3_bind_text(statement, 2, date.c_str(), -1, SQLITE_TRANSIENT);

    int r3 = sqlite3_bind_text(statement, 3, name.c_str(), -1, SQLITE_TRANSIENT);

    int r4 = sqlite3_bind_text(statement, 4, analysisResult.c_str(),-1, SQLITE_TRANSIENT);

    if (r1 != SQLITE_OK || r2 != SQLITE_OK || r3 != SQLITE_OK ||  r4 != SQLITE_OK) {

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


std::vector<Analysis> AnalysisRepository::getAnalysesByPatientId(std::int64_t patientId){
    sqlite3* db = database.getConnection();

    const char* sql = "SELECT date, name, result FROM analyses WHERE patient_id = ? ORDER " 
        "BY date DESC, id DESC;";

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

    std::vector<Analysis> analyses;

    while ((result = sqlite3_step(statement)) == SQLITE_ROW) {

        const unsigned char* dateText = sqlite3_column_text(statement, 0);

        const unsigned char* nameText = sqlite3_column_text(statement, 1);

        const unsigned char* resultText = sqlite3_column_text(statement, 2);

        if (dateText == nullptr || nameText == nullptr || resultText == nullptr) {

            sqlite3_finalize(statement);

            throw std::runtime_error("Invalid analysis data");
        }

        std::string date = reinterpret_cast<const char*>(dateText);

        std::string name = reinterpret_cast<const char*>(nameText);

        std::string value = reinterpret_cast<const char*>(resultText);

        analyses.emplace_back(date, name, value);
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

    return analyses;
}

