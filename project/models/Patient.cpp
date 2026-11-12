#include"Patient.h"

Patient::Patient(std::string name, int birthYear) : name(name), birthYear(birthYear){};

std::string Patient::getName() const {
	return name;
}

int Patient::getBirthYear() const {
	return birthYear;
}

void Patient::setName(std::string newName) {
	name = newName;
}

void Patient::setBirthYear(int newBirthYear) {
	birthYear = newBirthYear;
}