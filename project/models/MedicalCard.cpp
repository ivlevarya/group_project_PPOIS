#include "MedicalCard.h"

#include <stdexcept>
#include <utility>


MedicalCard::MedicalCard(std::int64_t patientId) : patientId(patientId) {}


std::int64_t MedicalCard::getPatientId() const {
    return patientId;
}

void MedicalCard::addRecord(std::unique_ptr<MedicalRecord> record) {
    if (record == nullptr) {
        throw std::invalid_argument("Medical record cannot be null");
    }

    records.push_back(std::move(record));
}


std::vector<std::string> MedicalCard::getDescriptions() const {
    std::vector<std::string> descriptions;

    for (const auto& record : records) {
        descriptions.push_back(record->getDescription());
    }

    return descriptions;
}
