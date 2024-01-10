#ifndef VALIDATION_H
#define VALIDATION_H

#include <iostream>
#include <ctype.h>

class Motorbike;
class Review;
class Request;
class MemberRent;
class Date;
class System;

class Validation {
public:
    bool password(std::string password);

    bool username(std::string username, System *sys);

    bool phoneNumber(std::string phoneNumber, System *sys);

    bool passport_number(std::string passportNumber, System *sys);

    bool driver_license_number(std::string driverLicenseNumber, System *sys);

    bool date(std::string date);

    bool listDate (std::string startDate, std::string endDate);

    bool name(std::string name);

    friend class System;

};
#endif // VALIDATION_H