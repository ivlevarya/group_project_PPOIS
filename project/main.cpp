
#include <iostream>
#include <exception>
#include <memory>
#include <cstdint>

#include "database/Database.h"

#include "models/Analysis.h"
#include "models/DoctorVisit.h"
#include "models/MedicalCard.h"

#include "repositories/AnalysisRepository.h"
#include "repositories/DoctorVisitRepository.h"

int main() {
    try {
        Database db("database/medspace.db");

        AnalysisRepository analysisRepository(db);
        DoctorVisitRepository visitRepository(db);

        // Use an existing patient ID
        std::int64_t patientId = 9;

        // Create medical card
        MedicalCard card(patientId);

        // Load analyses from database
        auto analyses =
            analysisRepository.getAnalysesByPatientId(patientId);

        for (const auto& analysis : analyses) {
            card.addRecord(
                std::make_unique<Analysis>(analysis)
            );
        }

        // Load doctor visits from database
        auto visits =
            visitRepository.getDoctorVisitsByPatientId(patientId);

        for (const auto& visit : visits) {
            card.addRecord(
                std::make_unique<DoctorVisit>(visit)
            );
        }

        // Display medical card
        std::cout << "Medical Card: "
            << card.getPatientId() << '\n';

        auto descriptions = card.getDescriptions();

        for (const auto& description : descriptions) {
            std::cout << description << '\n';
        }

        std::cout << "Total records: "
            << descriptions.size() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: "
            << error.what() << '\n';

        return 1;
    }

    return 0;
}
