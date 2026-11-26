#include "DateUtils.h"
#include <ctime>
#include <stdexcept>

namespace DateUtils {

    bool isValidDate(const std::string& date) {

        // Check YYYY-MM-DD format
        if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
            return false;
        }

        // Check that all other characters are digits
        for (int i = 0; i < 10; ++i) {
            if (i == 4 || i == 7) {
                continue;
            }

            if (date[i] < '0' || date[i] > '9') {
                return false;
            }
        }

        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));

        if (year < 1 || month < 1 || month > 12) {
            return false;
        }

        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        // Leap year calculation
        bool isLeapYear = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);

        if (isLeapYear) {
            daysInMonth[1] = 29;
        }

        return day >= 1 && day <= daysInMonth[month - 1];
    }

    int getCurrentYear() {
        std::time_t now = std::time(nullptr);
        std::tm currentDate{};

#ifdef _WIN32
        if (localtime_s(&currentDate, &now) != 0) {
            throw std::runtime_error("Cannot get current date");
        }
#else
        if (localtime_r(&now, &currentDate) == nullptr) {
            throw std::runtime_error("Cannot get current date");
        }
#endif

        return currentDate.tm_year + 1900;
    }

}