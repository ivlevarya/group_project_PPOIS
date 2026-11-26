#pragma once

#include <string>

class User {
private:
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string passwordHash;

public:
    explicit User(const std::string& firstName, const std::string& lastName,const std::string& email,
        const std::string& passwordHash);

    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    const std::string& getEmail() const;
    const std::string& getPasswordHash() const;
};
