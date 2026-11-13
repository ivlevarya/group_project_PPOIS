#include <iostream>
#include "models/Patient.h"
#include "database/Database.h"
#include "repositories/PatientRepository.h"
#include "external/sqlite/sqlite3.h"

int main() {
    try {
        Database db("database/medspace.db");
        PatientRepository repository(db);
        Patient p("Varvara", 2008);
        std::int64_t id = repository.addPatient(p);
        std::cout << "Created ID: " << id << std::endl;
        Patient p_1("Mark", 2016);
        bool update = repository.updatePatient(id, p_1);
        std::cout << "Updated: " << update << '\n';
        auto found = repository.getPatientById(id);

        if (found.has_value()) {
            std::cout << "Name: " << found->getName() << '\n';

            std::cout << "Birth year: " << found->getBirthYear() << '\n';
        }
        bool deleted = repository.deletePatient(id);

        std::cout << "Deleted: " << deleted << '\n';

        // 5. Проверяем, что пациент удален
        auto afterDelete = repository.getPatientById(id);

        if (!afterDelete.has_value()) {
            std::cout << "Patient not found!" << '\n';
        }
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }
    return 0;
}