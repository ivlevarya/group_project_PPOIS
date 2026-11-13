#pragma once

#include <string>

class User {
private:
    std::string name;

public:
    explicit User(std::string name);

    std::string getName() const;
    void setName(const std::string& newName);
};
