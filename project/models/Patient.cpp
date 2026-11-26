#include"Patient.h"
#include "../utils/DateUtils.h"
#include <stdexcept>
#include "../utils/ValidationUtils.h"

namespace {

	void validateBirthYear(int year) {
		if (year <= 0 || year > DateUtils::getCurrentYear()) {

			throw std::invalid_argument("Invalid patient birth year");
		}
	}

}

Patient::Patient(std::string name, int birthYear) : name(name), birthYear(birthYear){

	ValidationUtils::validateRequiredText(this->name, "Patient name");

	validateBirthYear(this->birthYear);
};

std::string Patient::getName() const {
	return name;
}

int Patient::getBirthYear() const {
	return birthYear;
}

void Patient::setName(std::string newName) {
	ValidationUtils::validateRequiredText(newName, "Patient name");
	name = newName;
}

void Patient::setBirthYear(int newBirthYear) {
	validateBirthYear(newBirthYear);
	birthYear = newBirthYear;
}