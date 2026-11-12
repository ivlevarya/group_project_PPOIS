#include <iostream>
#include "models/Patient.h"

int main() {
    Patient patient("Ivan Ivanov", 2004);

    std::cout << patient.getName() << std::endl;
    std::cout << patient.getBirthYear() << std::endl;

    return 0;
}