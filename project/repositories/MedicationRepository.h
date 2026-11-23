
#pragma once

#include <cstdint>
#include <vector>
#include "../models/Medication.h"

class Database;
class Medication;

class MedicationRepository {
private:
    Database& database;

public:
    explicit MedicationRepository(Database& database);

    std::int64_t addMedication(const Medication& medication, std::int64_t patientId);

    std::vector<Medication> getMedicationsByPatientId(std::int64_t patientId);
};
