#include "MedicalRecord.h"
#include "../utils/DateUtils.h"
#include <stdexcept>

MedicalRecord::MedicalRecord(const std::string& date)
    : date(date) {
    if (!DateUtils::isValidDate(date)) {
        throw std::invalid_argument("Invalid medical record date");
    }
}

std::string MedicalRecord::getDate() const {
    return date;
}
