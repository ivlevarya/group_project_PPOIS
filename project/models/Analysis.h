#pragma once

#include "MedicalRecord.h"
#include <string>

class Analysis : public MedicalRecord {
private:
    std::string name;
    std::string result;

public:
    Analysis(const std::string& date, const std::string& name, const std::string& result);

    std::string getDescription() const override;
};