#include "./Validation.h"
#include "Middleware\Middleware.h"
#include "DateTime\DateTime.h"
#include "Skill/Skill.h"
#include "Member/Member.h"
#include "SkillRent/Skillrent.h"
#include "MemberRent\MemberRent.h"
#include "Request\Request.h"
#include "Rating\Rating.h"
#include "System\System.h"
#include <ctype.h>
#include <sstream>


bool Validation::password(std::string password) {
    if (password.length() < 6) {
        std::cout << "Password is too short. Please enter a longer password.\n";
        return false;
    }
    for (int i = 0; i < password.size(); i++){
        if (password[i] == ' '){
            std::cout << "Password cannot contain space. Try again.\n";
            return false;
        }
    }
    return true;
}

bool Validation::username(std::string username, System *sys) {
    if (username.length() < 6) {
        std::cout << "Username is must be more than 6 characters. Try another.\n";
        return false;
    }
    for (int i = 0; i < username.size(); i++){
        if (username[i] == ' '){
            std::cout << "Username cannot contain a space\n";
            return false;
        }
    }
    for (Member *mem: sys->systemMemberList) {
        if (mem->username == username) {
            std::cout << "Username is taken. Try another.\n";
            return false;
        }
    }
    return true;
}


bool Validation::phoneNumber(std::string phoneNum, System *sys) {
    if (!isNumber(phoneNum)) {
        std::cout << "Please enter a valid phone number. Try again.\n";
        return false;
    } else if (phoneNum.length() != 10) {
        std::cout << "Please enter a valid phone number. Try again.\n";
        return false;
    }

    for (Member *mem: sys->systemMemberList) {
        if (mem->phoneNumber == phoneNum) {
            std::cout << "Phone number is taken. Please try another\n";
            return false;
        }
    }
    return true;
}

bool Validation::name(std::string name){
    for (int i = 0; i < name.length(); i++){
        if (!std::isalpha(name[i]) && name[i] != ' '){
            return false;
        }
    }
    return true;
}



