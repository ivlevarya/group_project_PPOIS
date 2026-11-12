#pragma once

#include <cstdint>

class Database;
class Patient;

class Database;

class PatientRepository {
private:
    Database& database;

public:
    explicit PatientRepository(Database& database);

    std::int64_t addPatient(const Patient& patient);
};