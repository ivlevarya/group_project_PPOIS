#include "DoctorVisit.h"

DoctorVisit::DoctorVisit(const std::string& date, const std::string& doctorName, const std::string& clinicName,
    const std::string& reason)
    : MedicalRecord(date), doctorName(doctorName), clinicName(clinicName), reason(reason) {}

std::string DoctorVisit::getDescription() const {
    return "Doctor: " + doctorName +
        ", Clinic: " + clinicName +
        ", Reason: " + reason +
        ", Date: " + getDate();
}

std::string DoctorVisit::getDoctorName() const {
    return doctorName;
}

std::string DoctorVisit::getClinicName() const {
    return clinicName;
}

std::string DoctorVisit::getReason() const {
    return reason;
}
