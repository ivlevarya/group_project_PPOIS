#pragma once

#include <cstdint>
#include <vector>

class Database;
class Analysis;

class AnalysisRepository {
private:
    Database& database;

public:
    explicit AnalysisRepository(Database& database);

    std::int64_t addAnalysis(const Analysis& analysis, std::int64_t patientId);

    std::vector<Analysis> getAnalysesByPatientId(std::int64_t patientId);
};
