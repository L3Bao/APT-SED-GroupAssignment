
#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include <iostream>
#include "DateTime\DateTime.h"

int convertStringToInt(std::string &str);
double convertStringToDouble(std::string &str);
DateTime* convertStringToDateTime(std::string &str);
bool isNumber(std::string &str);
DateTime* getCurrentDate();
#endif //MIDDLEWARE_H
