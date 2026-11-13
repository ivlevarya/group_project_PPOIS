#pragma once

#include <cstdint>
#include <optional>

#include "../models/Patient.h"

class Database;


class Database;

class PatientRepository {
private:
    Database& database;

public:
    explicit PatientRepository(Database& database);

    std::int64_t addPatient(const Patient& patient);

    std::optional<Patient> getPatientById(std::int64_t id);

    bool updatePatient(std::int64_t id, const Patient& patient);

    bool deletePatient(std::int64_t id);
};