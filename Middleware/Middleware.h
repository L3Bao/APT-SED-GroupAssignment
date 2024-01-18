
#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include <iostream>
#include <optional>
#include "../DateTime/DateTime.h"

int convertStringToInt(std::string &str);
double convertStringToDouble(std::string &str);
DateTime* convertStringToDateTime(std::string str);
bool isNumber(std::string &str);
bool convertStringToBool(std::string &str);
std::optional<double> convertStringToOptionalDouble(const std::string& str);
#endif //MIDDLEWARE_H
