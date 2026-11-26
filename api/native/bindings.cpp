
#include <pybind11/pybind11.h>

#include "database/Database.h"
#include "models/User.h"
#include "repositories/UserRepositiry.h"

#include <cstdint>
#include <string>

namespace py = pybind11;

// Create a user in SQLite
std::int64_t createUser(
    const std::string& dbPath,
    const std::string& firstName,
    const std::string& lastName,
    const std::string& email,
    const std::string& passwordHash
) {
    Database database(dbPath);
    UserRepository repository(database);

    User user(
        firstName,
        lastName,
        email,
        passwordHash
    );

    return repository.addUser(user);
}

// Find a user by email
py::object getUserByEmail(
    const std::string& dbPath,
    const std::string& email
) {
    Database database(dbPath);
    UserRepository repository(database);

    auto result = repository.getUserByEmail(email);

    if (!result.has_value()) {
        return py::none();
    }

    py::dict user;

    user["id"] = result->id;
    user["first_name"] = result->user.getFirstName();
    user["last_name"] = result->user.getLastName();
    user["email"] = result->user.getEmail();
    user["password_hash"] =
        result->user.getPasswordHash();

    return user;
}

PYBIND11_MODULE(medspase_core, m) {
    m.doc() = "MedSpase C++ core";

    m.def("create_user", &createUser);

    m.def("get_user_by_email", &getUserByEmail);
}
