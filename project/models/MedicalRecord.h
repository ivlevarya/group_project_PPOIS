#pragma once

#include <string>

class MedicalRecord {
private:
    std::string date;

public:
    explicit MedicalRecord(const std::string& date);

    virtual ~MedicalRecord() = default;

    std::string getDate() const;

    virtual std::string getDescription() const = 0;
};
