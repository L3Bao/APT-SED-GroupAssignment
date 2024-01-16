#include "Skill.h"
#include "../Rating/Rating.h"
#include "../SkillRent/SkillRent.h"
#include "../DateTime/DateTime.h"
#include "../Member/Member.h"
#include "../Request/Request.h"

#include <sstream>
#include <algorithm>

class Member;

Skill::Skill(int skillID, std::vector<std::string> skillList, int cityID) {
    this->skillID = skillID;
    this->skillList = skillList;
    this->cityID = cityID;
    this->isListed = false;
    this->availableFrom = nullptr;
    this->availableTo = nullptr;
    this->creditCostPerHour = 0;
    this->minHostRating = std::nullopt;
}

std::string Skill::getSkillInfo() {
    std::stringstream ss;
    ss << "Skill List: ";
    for (size_t i = 0; i < skillList.size(); ++i) {
        ss << skillList[i];
        if (i != skillList.size() - 1) {
            ss << ", ";
        }
    }
    ss << ", City: " << getCityName();
    return ss.str();
}

std::string Skill::getCityName() {
    return CITY_NAME_LIST[cityID];
}

bool Skill::addRequestToSkillRequestList(Request *request) {
    skillRequestList.push_back(request);
    return true;
}

bool Skill::removeRequestFromSkillRequestList(Request *request) {
    skillRequestList.erase(std::find(skillRequestList.begin(), skillRequestList.end(), request));
    return true;
}

bool Skill::addCompletedSession(SkillRent* host) {
    if (host == nullptr) {
        // Guard against null pointers to ensure robustness
        std::cerr << "Cannot add a null session to completed sessions.\n";
        return false;
    }

    // Add the session to the completed sessions list
    completedSkillList.push_back(host);
    return true;
}



Skill::~Skill() {
    for (auto request : skillRequestList) {
        delete request;
    }
    for (auto rent : skillRentList) {
        delete rent;
    }

    delete availableFrom;
    delete availableTo;
}