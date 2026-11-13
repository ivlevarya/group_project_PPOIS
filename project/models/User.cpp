#include "User.h"

User::User(std::string name)
    : name(name) {
}

std::string User::getName() const {
    return name;
}

void User::setName(const std::string& newName) {
    name = newName;
}
