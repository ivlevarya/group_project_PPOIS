#pragma once

#include "MedicalRecord.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class MedicalCard {
private:
    std::int64_t patientId;

    std::vector<std::unique_ptr<MedicalRecord>> records;

public:
    explicit MedicalCard(std::int64_t patientId);

    std::int64_t getPatientId() const;

    void addRecord(std::unique_ptr<MedicalRecord> record);

    std::vector<std::string> getDescriptions() const;
};
