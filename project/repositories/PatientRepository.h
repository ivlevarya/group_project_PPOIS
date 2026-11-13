#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../models/Patient.h"

class Database;

struct PatientWithId {
    std::int64_t id;
    Patient patient;
};

class Database;

class PatientRepository {
private:
    Database& database;

public:
    explicit PatientRepository(Database& database);

    std::int64_t addPatient(const Patient& patient, std::int64_t userId);

    std::optional<Patient> getPatientById(std::int64_t id);

    bool updatePatient(std::int64_t id, const Patient& patient);

    bool deletePatient(std::int64_t id);

    std::vector<PatientWithId> getPatientsByUserId(std::int64_t userId);
};