#include "MedicalRecord.h"

MedicalRecord::MedicalRecord(const std::string& date)
    : date(date) {
}

std::string MedicalRecord::getDate() const {
    return date;
}
