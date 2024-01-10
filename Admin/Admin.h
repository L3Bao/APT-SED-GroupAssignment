
#ifndef ADMIN_H
#define ADMIN_H

#include "../User/User.h"

#include <iostream>

class User;

class Admin: public User {
public:
    Admin(std::string username, std::string password);

    friend class System;

    friend class InputData;

    friend class OutputData;
};


#endif //ADMIN_H
