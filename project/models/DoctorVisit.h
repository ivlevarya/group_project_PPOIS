#pragma once

#include "MedicalRecord.h"
#include <string>

class DoctorVisit : public MedicalRecord {
private:
    std::string doctorName;
    std::string clinicName;
    std::string reason;

public:
    DoctorVisit(const std::string& date, const std::string& doctorName, const std::string& clinicName, 
        const std::string& reason);

    std::string getDescription() const override;

    std::string getDoctorName() const;
    std::string getClinicName() const;
    std::string getReason() const;
};
