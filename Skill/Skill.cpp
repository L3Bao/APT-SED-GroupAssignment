#include "Skill.h"
#include "../Rating/Rating.h"
#include "../SkillRent/SkillRent.h"
#include "../DateTime/DateTime.h"
#include "../Member/Member.h"
#include "../Request/Request.h"

#include <sstream>
#include <algorithm>
#include <vector>

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

std::vector<std::string> Skill::getSkillList() const{
    return skillList;
}

std::string Skill::getListOfSkill() const {
    std::stringstream ss;
    for (size_t i = 0; i < skillList.size(); ++i) {
        ss << skillList[i];
        if (i != skillList.size() - 1) {
            ss << ", ";
        }
    }
    return ss.str();
}

int Skill::getSkillID() const{
    return skillID;
}

int Skill::getCityID() const{
    return cityID;
}

DateTime* Skill::getAvailableFrom() const {
    return availableFrom;
}

DateTime* Skill::getAvailableTo() const {
    return availableTo;
}

int Skill::getCreditCostPerHour() const {
    return creditCostPerHour;
}

std::optional<double> Skill::getMinHostRating() const {
    return minHostRating;
}

void Skill::addSkillRent(SkillRent* skillRent) {
    if (skillRent != nullptr) {
        skillRentList.push_back(skillRent);
    }
}

std::string Skill::getSkillInfo() const {
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

std::string Skill::getCityName() const {
    return CITY_NAME_LIST[cityID];
}

bool Skill::addRequestToSkillRequestList(Request* request) {
    skillRequestList.push_back(request);
    return true;
}

bool Skill::removeRequestFromSkillRequestList(Request* request) {
    skillRequestList.erase(std::find(skillRequestList.begin(), skillRequestList.end(), request));
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
