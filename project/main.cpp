
#include <iostream>
#include <exception>
#include <memory>
#include <cstdint>

#include "models/Analysis.h"
#include "models/MedicalCard.h"

#include "database/Database.h"
#include "repositories/AnalysisRepository.h"

int main() {
    try {
        Database db("database/medspace.db");

        AnalysisRepository analysisRepository(db);

        std::int64_t patientId = 9;

        // Load analyses from SQLite
        auto analyses =
            analysisRepository.getAnalysesByPatientId(
                patientId
            );

        // Create medical card
        MedicalCard card(patientId);

        // Add loaded analyses to medical card
        for (const auto& analysis : analyses) {
            card.addRecord(
                std::make_unique<Analysis>(analysis)
            );
        }

        // Display medical records
        std::cout << "Medical Card: "
            << card.getPatientId() << '\n';

        auto descriptions = card.getDescriptions();

        for (const auto& description : descriptions) {
            std::cout << description << '\n';
        }
    }
    catch (const std::exception& error) {
        std::cerr << "Error: "
            << error.what() << '\n';

        return 1;
    }

    return 0;
}
