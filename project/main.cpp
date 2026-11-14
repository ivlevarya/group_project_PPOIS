
#include <iostream>
#include <memory>

#include "models/MedicalCard.h"
#include "models/Analysis.h"
#include "models/DoctorVisit.h"

int main() {
    // Create medical card for patient ID = 1
    MedicalCard card(1);

    // Add analysis
    card.addRecord(std::make_unique<Analysis>("2026-11-10","Blood test", "Hemoglobin: 135"));

    // Add doctor visit
    card.addRecord(std::make_unique<DoctorVisit>("2026-11-12", "Dr. Ivanov", "City Clinic", "Regular checkup"));

    // Get all descriptions
    auto descriptions = card.getDescriptions();

    std::cout << "Medical card of patient ID: "
        << card.getPatientId() << '\n';

    for (const auto& description : descriptions) {
        std::cout << description << '\n';
    }

    return 0;
}
