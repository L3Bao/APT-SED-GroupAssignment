#include "Validation.h"
#include "../Middleware/Middleware.h"
#include "../DateTime/DateTime.h"
#include "../Skill/Skill.h"
#include "../Member/Member.h"
#include "../SkillRent/Skillrent.h"
#include "../MemberRent/MemberRent.h"
#include "../Request/Request.h"
#include "../Rating/Rating.h"
#include "../System/System.h"
#include <ctype.h>
#include <sstream>
#include <regex>


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

bool Validation::email(std::string emailAddress, System *sys) {
    // Email format validation
    std::regex emailPattern(R"(^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$)");
    if (!std::regex_match(emailAddress, emailPattern)) {
        std::cout << "Please enter a valid email address. Try again.\n";
        return false;
    }

    // Check for email uniqueness
    for (const auto& member : sys->systemMemberList) {
        if (member->email == emailAddress) {
            std::cout << "Email address is already in use. Please try another.\n";
            return false;
        }
    }

    return true; // Email is valid and unique
}

bool Validation::address(std::string homeAddress, System *sys) {
    // Basic validation to check if the address is not empty
    if (homeAddress.empty()) {
        std::cout << "Address cannot be empty. Please enter a valid address.\n";
        return false;
    }

    // Regex to match format "a number followed by a name"
    std::regex addressPattern(R"(^\d+\s+[A-Za-z]+(?:\s+[A-Za-z]+)*)");
    if (!std::regex_match(homeAddress, addressPattern)) {
        std::cout << "Invalid address format. The format should be 'a number followed by a name'.\n";
        return false;
    }

    // Check for address uniqueness
    for (const auto& member : sys->systemMemberList) {
        if (member->address == homeAddress) {
            std::cout << "Address is already in use. Please try another.\n";
            return false;
        }
    }

    return true; // Address is valid
}

bool Validation::time(std::string time) {
    // Check length (should be 5 for "hh:mm")
    if (time.length() != 5) {
        std::cout << "Please enter a valid time. Try again.\n";
        return false;
    }

    // Check format (positions for digits and colon)
    if (!std::isdigit(time[0]) || !std::isdigit(time[1]) || 
        time[2] != ':' || 
        !std::isdigit(time[3]) || !std::isdigit(time[4])) {
        std::cout << "Please enter a valid time. Try again.\n";
        return false;
    }

    // Convert string to DateTime and validate hours and minutes
    try {
        DateTime* temp = convertStringToDateTime(time);

        // Validate hours and minutes
        if (temp->hour < 0 || temp->hour > 23 || temp->minute < 0 || temp->minute > 59) {
            std::cout << "Please enter a valid time. Try again.\n";
            delete temp;
            return false;
        }

        delete temp;
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << "\n";
        return false;
    }

    return true;
}

bool Validation::timeLater(std::string startTime, std::string endTime) {
    // Convert strings to DateTime objects
    DateTime* start = convertStringToDateTime(startTime);
    DateTime* end = convertStringToDateTime(endTime);

    // Compare the two DateTime objects
    if (*start < *end) {
        delete start;
        delete end;
        return true;
    } else {
        delete start;
        delete end;
        std::cout << "End time must be later than start time. Try again.\n";
        return false;
    }
}

bool Validation::rating(std::string ratingStr, double rating) {
     try {
            rating = std::stod(ratingStr);  // Convert string to double
            if (rating >= 1.0 && rating <= 5.0) {
                return true;  // Valid rating
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid format: Rating must be a number.\n";
        }
        return false;  // Invalid rating
}