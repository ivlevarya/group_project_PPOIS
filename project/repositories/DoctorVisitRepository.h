#pragma once

#include <cstdint>
#include <vector>
#include "../models/DoctorVisit.h"

class Database;
class DoctorVisit;

class DoctorVisitRepository {
private:
    Database& database;

public:
    explicit DoctorVisitRepository(Database& database);

    std::int64_t addDoctorVisit(const DoctorVisit& visit, std::int64_t patientId);

    std::vector<DoctorVisit> getDoctorVisitsByPatientId(std::int64_t patientId);
};
