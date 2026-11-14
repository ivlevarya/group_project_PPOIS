#include "Analysis.h"

Analysis::Analysis(const std::string& date, const std::string& name, const std::string& result): 
    MedicalRecord(date), name(name), result(result) {
}

std::string Analysis::getDescription() const {
    return "Analysis: " + name +
        ", Result: " + result +
        ", Date: " + getDate();
}