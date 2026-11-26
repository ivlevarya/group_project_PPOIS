#include "ValidationUtils.h"

#include <stdexcept>
#include <string>

namespace ValidationUtils {

    void validateRequiredText(const std::string& value, const std::string& fieldName) {
        if (value.find_first_not_of(" \t\r\n") == std::string::npos) {

            throw std::invalid_argument(fieldName + " cannot be empty");
        }
    }

}