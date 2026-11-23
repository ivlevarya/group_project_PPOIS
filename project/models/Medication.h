#pragma once

#include <string>

class Medication {
private:
    std::string name;
    std::string dosage;
    std::string frequency;
    std::string startDate;
    std::string endDate;

public:
    Medication(
        const std::string& name,
        const std::string& dosage,
        const std::string& frequency,
        const std::string& startDate,
        const std::string& endDate
    );

    std::string getName() const;
    std::string getDosage() const;
    std::string getFrequency() const;
    std::string getStartDate() const;
    std::string getEndDate() const;
};