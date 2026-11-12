#pragma once

#include <string>

class Patient {
private:
    std::string name;
    int birthYear;

public:
    Patient(std::string name, int birthYear);

    std::string getName() const;
    int getBirthYear() const;

    void setName(std::string newName);
    void setBirthYear(int newBirthYear);
};