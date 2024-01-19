#include "Admin.h"

#include <utility>

Admin::Admin(std::string username, std::string password)
    : User(std::move(username), std::move(password)) {
}
