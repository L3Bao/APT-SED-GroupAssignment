#include <sstream>
#include <vector>
#include "Middleware.h"
#include "../DateTime/DateTime.h"
#include <ctime>
#include <iomanip>
#include <optional>

int convertStringToInt(std::string &str) {
    int result;
    std::stringstream ss;
    ss << str;
    ss >> result;
    return result;
}

double convertStringToDouble(std::string &str) {
    double result;
    std::stringstream ss;
    ss << str;
    ss >> result;
    return result;
}

std::optional<double> convertStringToOptionalDouble(const std::string& str) {
    if (str.empty()) {
        return std::nullopt; // Return no value for empty strings
    }

    std::stringstream ss(str);
    double result;

    if (ss >> result) {
        return result; // Successfully converted to double
    } else {
        return std::nullopt; // Conversion failed
    }
}


DateTime* convertStringToDateTime(std::string str) {
    std::stringstream ss {str};
    std::string segment;
    std::vector<int> timeSegments;

    // Split string by ':'
    while (std::getline(ss, segment, ':')) {
        timeSegments.push_back(std::stoi(segment));
    }

    if (timeSegments.size() != 2) {
        throw std::invalid_argument("Invalid time format. Time should be in hh:mm format.");
    }

    // Create a DateTime object
    DateTime* result = new DateTime(timeSegments[0], timeSegments[1]);

    return result;
}

bool convertStringToBool(std::string& str) {
    return str == "1";
}

bool isNumber(std::string &str) {
    for (int i = 0; i < str.length(); i++) {
        if (i == 0){
            if (str[i] == '-'){
                continue;
            }
        }
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}