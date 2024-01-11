#include "Skill\Skill.h"
#include "Rating\Rating.h"
#include "SkillRent\SkillRent.h"
#include "DateTime\DateTime.h"
#include "Member\Member.h"

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

bool Skill::addReviewToSkill(Rating *skillReview) {
    skillRatingList.push_back(skillReview);
    return true;
}