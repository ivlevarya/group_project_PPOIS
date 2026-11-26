
#include "Medication.h"
#include "../utils/DateUtils.h"
#include <stdexcept>
#include "../utils/ValidationUtils.h"

Medication::Medication(
    const std::string& name,
    const std::string& dosage,
    const std::string& frequency,
    const std::string& startDate,
    const std::string& endDate
)
    : name(name), dosage(dosage), frequency(frequency), startDate(startDate), endDate(endDate){

    ValidationUtils::validateRequiredText(this->name, "Medication name");

    ValidationUtils::validateRequiredText(this->dosage, "Medication dosage");

    ValidationUtils::validateRequiredText(this->frequency, "Medication frequency");

    if (!DateUtils::isValidDate(startDate) || !DateUtils::isValidDate(endDate)) {
        throw std::invalid_argument("Invalid medication date");
    }

    if (endDate < startDate) {
        throw std::invalid_argument("End date cannot be earlier than start date");
    }

}

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
