
#include "Medication.h"

Medication::Medication(
    const std::string& name,
    const std::string& dosage,
    const std::string& frequency,
    const std::string& startDate,
    const std::string& endDate
)
    : name(name), dosage(dosage), frequency(frequency), startDate(startDate), endDate(endDate){}

std::string Medication::getName() const {
    return name;
}

std::string Medication::getDosage() const {
    return dosage;
}

std::string Medication::getFrequency() const {
    return frequency;
}

std::string Medication::getStartDate() const {
    return startDate;
}

std::string Medication::getEndDate() const {
    return endDate;
}
