#include "User.h"
#include "../utils/ValidationUtils.h"

User::User(std::string name)
    : name(name) {

    ValidationUtils::validateRequiredText(this->name, "User name");
}

std::string User::getName() const {
    return name;
}

void User::setName(const std::string& newName) {
    ValidationUtils::validateRequiredText(newName, "User name");

    name = newName;
}
