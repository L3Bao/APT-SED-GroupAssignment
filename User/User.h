#ifndef USER_H
#define USER_H

#include <iostream>
// #include "./../../System/System.h"

class User {
protected:
    std::string username;
    std::string password;

public:
    User(std::string username, std::string password);

    friend class System;

    friend class OutputData;

    friend class Validation;
};


#endif //TEST_USER_H
