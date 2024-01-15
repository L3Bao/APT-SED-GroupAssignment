#ifndef VALIDATION_H
#define VALIDATION_H

#include <iostream>
#include <ctype.h>

class Skill;
class Rating;
class Request;
class MemberRent;
class DateTime;
class System;

class Validation {
public:
    bool password(std::string password);

    bool username(std::string username, System *sys);

    bool phoneNumber(std::string phoneNumber, System *sys);

    bool email(std::string email, System *sys);

    bool address(std::string address, System *sys);

    bool time(std::string time);

    bool timeLater(std::string startTime, std::string endTime);

    bool name(std::string name);

    bool rating(std::string ratingStr, double rating);

    friend class System;

};
#endif // VALIDATION_H