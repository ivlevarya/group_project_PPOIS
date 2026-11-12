#include <iostream>
#include "models/Patient.h"
#include "database/Database.h"
#include "repositories/PatientRepository.h"
#include"external/sqlite/sqlite3.h"

int main() {
    try {
        Patient patient("Ivan Ivanov", 2004);
        Database db("database/medspace.db");
        PatientRepository repository(db);
        std::int64_t id = repository.addPatient(patient);
        std::cout << "Add patient with ID " << id << std::endl;
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }
    return 0;
}