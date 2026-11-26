#include "Analysis.h"
#include "../utils/ValidationUtils.h"

Analysis::Analysis(const std::string& date, const std::string& name, const std::string& result): 
    MedicalRecord(date), name(name), result(result) {
    ValidationUtils::validateRequiredText(this->name, "Analysis name");

    ValidationUtils::validateRequiredText(this->result, "Analysis result");
}

std::string Analysis::getDescription() const {
    return "Analysis: " + name +
        ", Result: " + result +
        ", Date: " + getDate();
}

std::string Analysis::getName() const {
    return name;
}

std::string Analysis::getResult() const {
    return result;
}