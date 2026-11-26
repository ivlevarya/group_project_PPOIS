#include "User.h"
#include "../utils/ValidationUtils.h"

User::User(const std::string& firstName, const std::string& lastName, const std::string& email, const std::string& passwordHash)
    : firstName(firstName), lastName(lastName), email(email), passwordHash(passwordHash)
{
    ValidationUtils::validateRequiredText(this->firstName, "First name");

    ValidationUtils::validateRequiredText(this->lastName, "Last name");

    ValidationUtils::validateRequiredText(this->email, "Email");

    ValidationUtils::validateRequiredText(this->passwordHash, "Password hash");
}

const std::string& User::getFirstName() const {
    return firstName;
}

const std::string& User::getLastName() const {
    return lastName;
}

const std::string& User::getEmail() const {
    return email;
}

const std::string& User::getPasswordHash() const {
    return passwordHash;
}

