#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <iomanip>

#include "Member.h"
#include "Rating\Rating.h"
#include "SkillRent\SkillRent.h"
#include "MemberRent\MemberRent.h"
#include "Skill\Skill.h"
#include "Middleware\Middleware.h"
#include "Request\Request.h"

//Constructor
Member::Member(int memberID, std::string username, std::string password, std::string firstName, std::string lastName, std::string phoneNumber, std::string email, std::string address, int creditPoints) : User(std::move(username), std::move(password)) {
    this->memberID = memberID;
    this->firstName = std::move(firstName);
    this->lastName = std::move(lastName);
    this->phoneNumber = std::move(phoneNumber);
    this->email = std::move(email);
    this->address = std::move(address);
    this->creditPoints = creditPoints;

    this->ownedSkill = nullptr;

    memberRatingList.clear();
}

std::string Member::get_name() {
    return firstName + " " + lastName;
}

std::tuple<double, double, double> Member::getRatingScore() {
    if (memberRatingList.empty()) {
        return std::make_tuple(0.0, 0.0, 0.0);
    }

    double totalSkillRating = 0.0, totalSupporterRating = 0.0, totalHostRating = 0.0;

    for (auto &review : memberRatingList) {
        totalSkillRating += review->scores.getSkillRating();
        totalSupporterRating += review->scores.getSupporterRating();
        totalHostRating += review->scores.getHostRating();
    }

    double avgSkillRating = totalSkillRating / memberRatingList.size();
    double avgSupporterRating = totalSupporterRating / memberRatingList.size();
    double avgHostRating = totalHostRating / memberRatingList.size();

    // Round the averages to one decimal place
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << avgSkillRating << ' ' << avgSupporterRating << ' ' << avgHostRating;
    ss >> avgSkillRating >> avgSupporterRating >> avgHostRating;

    return std::make_tuple(avgSkillRating, avgSupporterRating, avgHostRating);
}


// Show member's information
bool Member::showMemInfo() {
    std::cout << "\n+ Username: " << username << '\n';
    std::cout << "+ Full name: " << get_name() << '\n';
    std::cout << "+ Phone number: " << phoneNumber << '\n';
    std::cout << "+ Email: " << email << '\n';
    std::cout << "+ Address: " << address << '\n';
    std::cout << "+ Credit points: " << creditPoints << '\n';

    if (ownedSkill != nullptr) {
        std::cout << "+ Owned skill: " << ownedSkill->getSkillList() << '\n';
    }

    // Get the rating scores
    auto [avgSkillRating, avgSupporterRating, avgHostRating] = getRatingScore();
    std::cout << "+ Average Skill Rating: " << avgSkillRating << '\n';
    std::cout << "+ Average Supporter Rating: " << avgSupporterRating << '\n';
    std::cout << "+ Average Host Rating: " << avgHostRating << '\n';

    std::cout << '\n';
    return true;
}

// List the skill
bool Member::listSkill(DateTime *startTime, DateTime *endTime, int creditPointsPerHour, double minimumHostRating) {
    if (ownedSkill->isListed) {
        return false;
    }

    ownedSkill->isListed = true;
    ownedSkill->availableFrom = startTime;
    ownedSkill->availableTo = endTime;
    ownedSkill->creditCostPerHour = creditPointsPerHour;
    ownedSkill->minHostRating = minimumHostRating;

    return true;
}

// Unlist the skill
bool Member::unlistSkill() {
    if (!ownedSkill->isListed || ownedSkill->skillRequestList.empty()) {
        return false;
    }

    ownedSkill->isListed = false;
    ownedSkill->availableFrom = nullptr;
    ownedSkill->availableTo = nullptr;
    ownedSkill->creditCostPerHour = 0;
    ownedSkill->minHostRating = 0;

    ownedSkill->skillRequestList.clear();

    return true;
}

bool Member::setNewSkill(Skill *skill) {
    if (ownedSkill != nullptr) {
        return false;
    }

    ownedSkill = skill;
    skill->skillOwner = this;
    return true;
}

bool Member::showListOfRequest() {
    if (ownedSkill->skillRequestList.empty()) {
        std::cout << "You do not have any requests right now\n\n";
        return false;
    }

    std::cout << "The request list for your support:\n\n";
    for (int i = 0; i < ownedSkill->skillRequestList.size(); i++) {
        auto request = ownedSkill->skillRequestList[i];
        std::cout << "+ " << i+1 << ". " << request->requestFrom->toString() << '\n';
    }

}