#include "Admin.h"

#include <utility>

Admin::Admin(std::string username = "admin123", std::string password = "1234567")
    : User(std::move(username), std::move(password)) {
}
