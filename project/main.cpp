
#include <iostream>
#include <exception>

#include "models/User.h"
#include "models/Patient.h"

#include "database/Database.h"

#include "repositories/UserRepositiry.h"
#include "repositories/PatientRepository.h"

int main() {
    try {
        Database db("database/medspace.db");

        PatientRepository patientRepository(db);

        std::int64_t userId = 5;

        auto p = patientRepository.getPatientsByUserId(userId);

        if (p.empty()) {
            std::cout << "No profiles found!" << '\n';
        }

        for (int i = 0; i < p.size(); i++) {
            std::cout << "Patient ID: " << p[i].id << std::endl;
            std::cout << "Name: "
                << p[i].patient.getName() << '\n';

            std::cout << "Birth year: "
                << p[i].patient.getBirthYear() << '\n';

            std::cout << "--------------------" << '\n';
        }
    }
    catch (const std::exception& error) {
        std::cerr << "Error: "
            << error.what() << '\n';
        return 1;
    }

    return 0;
}
