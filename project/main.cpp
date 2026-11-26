
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <exception>
#include <cstdint>
#include <memory>

#include "database/Database.h"

#include "models/User.h"
#include "models/Patient.h"
#include "models/Analysis.h"
#include "models/DoctorVisit.h"
#include "models/MedicalCard.h"

#include "repositories/UserRepositiry.h"
#include "repositories/PatientRepository.h"
#include "repositories/AnalysisRepository.h"
#include "repositories/DoctorVisitRepository.h"
#include "models/Medication.h"
#include "repositories/MedicationRepository.h"

#include "external/sqlite/sqlite3.h"

// Read text from console
std::string readText(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string value;

        if (!std::getline(std::cin, value)) {
            throw std::runtime_error("Input closed");
        }

        if (value.find_first_not_of(" \t\r\n") !=
            std::string::npos) {
            return value;
        }

        std::cout << "Value cannot be empty.\n";
    }
}

// Read an integer from console
std::int64_t readNumber(const std::string& prompt) {
    while (true) {
        std::string text = readText(prompt);
        std::istringstream stream(text);

        std::int64_t number;
        std::string extra;

        if ((stream >> number) && !(stream >> extra)) {
            return number;
        }

        std::cout << "Please enter a valid number.\n";
    }
}

// Read a positive ID
std::int64_t readId(const std::string& prompt) {
    while (true) {
        std::int64_t id = readNumber(prompt);

        if (id > 0) {
            return id;
        }

        std::cout << "ID must be positive.\n";
    }
}

// Display main menu
void showMenu() {
    std::cout << "\n========== MEDSPASE ==========\n";
    std::cout << "1. Create user\n";
    std::cout << "2. Add patient\n";
    std::cout << "3. Show user's patients\n";
    std::cout << "4. Add analysis\n";
    std::cout << "5. Add doctor visit\n";
    std::cout << "6. Show medical card\n";
    std::cout << "7. Add medication\n";
    std::cout << "8. Show medications\n";
    std::cout << "9. Delete patient\n";
    std::cout << "0. Exit\n";
    std::cout << "==============================\n";
}

int main() {
    try {
        // Connect to SQLite
        Database db("database/medspace.db");
        // Create repositories
        UserRepository userRepository(db);
        PatientRepository patientRepository(db);
        AnalysisRepository analysisRepository(db);
        DoctorVisitRepository visitRepository(db);
        MedicationRepository medicationRepository(db);


        try {
            Medication medication(
                "Paracetamol",
                "500 mg",
                "Twice daily",
                "2026-11-24",
                "2026-12-01"
            );

            medicationRepository.addMedication(
                medication, 999999999
            );

            std::cout << "Repository call finished\n";
        }
        catch (const std::exception& error) {
            std::cout << "Database error: "
                << error.what() << '\n';
        }




        while (true) {
            showMenu();

            std::int64_t choice = readNumber(
                "Choose an option: "
            );

            try {
                switch (choice) {

                    // Create user
                case 1: {
                    std::string name = readText(
                        "Enter user name: "
                    );

                    User user(name);

                    std::int64_t userId =
                        userRepository.addUser(user);

                    std::cout << "User created!\n";
                    std::cout << "User ID: "
                        << userId << '\n';

                    break;
                }

                      // Add patient to user
                case 2: {
                    std::int64_t userId = readId(
                        "Enter user ID: "
                    );

                    std::string name = readText(
                        "Enter patient name: "
                    );

                    int birthYear = static_cast<int>(
                        readNumber("Enter birth year: ")
                        );

                    Patient patient(name, birthYear);

                    std::int64_t patientId =
                        patientRepository.addPatient(
                            patient,
                            userId
                        );

                    std::cout << "Patient created!\n";
                    std::cout << "Patient ID: "
                        << patientId << '\n';

                    break;
                }

                      // Show patients belonging to user
                case 3: {
                    std::int64_t userId = readId(
                        "Enter user ID: "
                    );

                    auto patients =
                        patientRepository.getPatientsByUserId(
                            userId
                        );

                    std::cout << "\nPatients:\n";

                    for (const auto& item : patients) {
                        std::cout
                            << "ID: " << item.id
                            << " | Name: "
                            << item.patient.getName()
                            << " | Birth year: "
                            << item.patient.getBirthYear()
                            << '\n';
                    }

                    std::cout << "Total patients: "
                        << patients.size() << '\n';

                    break;
                }

                      // Add analysis
                case 4: {
                    std::int64_t patientId = readId(
                        "Enter patient ID: "
                    );

                    if (!patientRepository
                        .getPatientById(patientId)
                        .has_value()) {
                        std::cout << "Patient not found.\n";
                        break;
                    }

                    std::string date = readText(
                        "Enter date (YYYY-MM-DD): "
                    );

                    std::string name = readText(
                        "Enter analysis name: "
                    );

                    std::string result = readText(
                        "Enter result: "
                    );

                    Analysis analysis(date, name, result);

                    std::int64_t analysisId =
                        analysisRepository.addAnalysis(
                            analysis,
                            patientId
                        );

                    std::cout << "Analysis saved!\n";
                    std::cout << "Analysis ID: "
                        << analysisId << '\n';

                    break;
                }

                      // Add doctor visit
                case 5: {
                    std::int64_t patientId = readId(
                        "Enter patient ID: "
                    );

                    if (!patientRepository
                        .getPatientById(patientId)
                        .has_value()) {
                        std::cout << "Patient not found.\n";
                        break;
                    }

                    std::string date = readText(
                        "Enter date (YYYY-MM-DD): "
                    );

                    std::string doctor = readText(
                        "Enter doctor name: "
                    );

                    std::string clinic = readText(
                        "Enter clinic name: "
                    );

                    std::string reason = readText(
                        "Enter visit reason: "
                    );

                    DoctorVisit visit(
                        date,
                        doctor,
                        clinic,
                        reason
                    );

                    std::int64_t visitId =
                        visitRepository.addDoctorVisit(
                            visit,
                            patientId
                        );

                    std::cout << "Doctor visit saved!\n";
                    std::cout << "Visit ID: "
                        << visitId << '\n';

                    break;
                }

                      // Load and display medical card
                case 6: {
                    std::int64_t patientId = readId(
                        "Enter patient ID: "
                    );

                    auto patient =
                        patientRepository.getPatientById(
                            patientId
                        );

                    if (!patient.has_value()) {
                        std::cout << "Patient not found.\n";
                        break;
                    }

                    MedicalCard card(patientId);

                    // Load analyses
                    auto analyses =
                        analysisRepository.getAnalysesByPatientId(
                            patientId
                        );

                    for (const auto& analysis : analyses) {
                        card.addRecord(
                            std::make_unique<Analysis>(
                                analysis
                            )
                        );
                    }

                    // Load doctor visits
                    auto visits =
                        visitRepository.getDoctorVisitsByPatientId(
                            patientId
                        );

                    for (const auto& visit : visits) {
                        card.addRecord(
                            std::make_unique<DoctorVisit>(
                                visit
                            )
                        );
                    }

                    // Display medical card
                    std::cout << "\n====== MEDICAL CARD ======\n";
                    std::cout << "Patient ID: "
                        << card.getPatientId() << '\n';

                    std::cout << "Patient: "
                        << patient->getName() << '\n';

                    auto descriptions =
                        card.getDescriptions();

                    if (descriptions.empty()) {
                        std::cout
                            << "No medical records.\n";
                    }

                    for (const auto& description : descriptions) {
                        std::cout << "- "
                            << description << '\n';
                    }

                    std::cout << "Total records: "
                        << descriptions.size() << '\n';

                    std::cout << "==========================\n";

                    break;
                }
                case 7: {
                    std::int64_t patientId = readId("Enter patient ID: ");

                    if (!patientRepository.getPatientById(patientId).has_value()) {
                        std::cout << "Patient not found.\n";
                        break;
                    }

                    std::string name = readText("Enter medication name: ");

                    std::string dosage = readText("Enter dosage: ");

                    std::string frequency = readText("Enter frequency: ");

                    std::string startDate = readText("Enter start date (YYYY-MM-DD): ");

                    std::string endDate = readText("Enter end date (YYYY-MM-DD): ");

                    Medication medication(
                        name,
                        dosage,
                        frequency,
                        startDate,
                        endDate
                    );

                    std::int64_t medicationId =
                        medicationRepository.addMedication(
                            medication,
                            patientId
                        );

                    std::cout << "Medication saved!\n";
                    std::cout << "Medication ID: "
                        << medicationId << '\n';

                    break;
                }

                case 8: {
                    std::int64_t patientId = readId(
                        "Enter patient ID: "
                    );

                    auto patient =
                        patientRepository.getPatientById(patientId);

                    if (!patient.has_value()) {
                        std::cout << "Patient not found.\n";
                        break;
                    }

                    auto medications =
                        medicationRepository.getMedicationsByPatientId(
                            patientId
                        );

                    std::cout << "\nMedications for "
                        << patient->getName() << ":\n";

                    if (medications.empty()) {
                        std::cout << "No medications found.\n";
                    }

                    for (const auto& medication : medications) {
                        std::cout << "\nName: "
                            << medication.getName() << '\n';

                        std::cout << "Dosage: "
                            << medication.getDosage() << '\n';

                        std::cout << "Frequency: "
                            << medication.getFrequency() << '\n';

                        std::cout << "Period: "
                            << medication.getStartDate()
                            << " - "
                            << medication.getEndDate() << '\n';
                    }

                    std::cout << "\nTotal medications: "
                        << medications.size() << '\n';

                    break;
                }
                case 9: {
                    // Get patient ID
                    std::int64_t patientId = readId("Enter patient ID: ");

                    // Check if patient exists
                    auto patient = patientRepository.getPatientById(patientId);

                    if (!patient.has_value()) {
                        std::cout << "Patient not found\n";
                        break;
                    }

                    // Show patient information
                    std::cout << "Patient: "
                        << patient->getName() << '\n';

                    std::cout << "All patient medical records will be deleted.\n";

                    // Ask for confirmation
                    std::string confirmation = readText(
                        "Type DELETE to confirm: "
                    );

                    if (confirmation != "DELETE") {
                        std::cout << "Deletion cancelled\n";
                        break;
                    }

                    // Delete patient
                    patientRepository.deletePatient(patientId);

                    std::cout << "Patient deleted successfully\n";

                    break;
                }

                      // Exit
                case 0:
                    std::cout << "Goodbye!\n";
                    return 0;
                    break;

                default:
                    std::cout << "Unknown option.\n";
                    break;
                }

            }
            catch (const std::exception& error) {
                std::cerr << "Operation error: "
                    << error.what() << '\n';
            }
        }
    }
    catch (const std::exception& error) {
        std::cerr << "Program error: "
            << error.what() << '\n';

        return 1;
    }
}
