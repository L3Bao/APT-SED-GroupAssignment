#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <optional>

#include "Member.h"
#include "../Rating/Rating.h"
#include "../SkillRent/SkillRent.h"
#include "../MemberRent/MemberRent.h"
#include "../Skill/Skill.h"
#include "../Middleware/Middleware.h"
#include "../Request/Request.h"
#include "../Member/BlockedMember.h"

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


//Feature 4: A member can login with the registered username and password, and can view all of his/her information.
bool Member::showMemInfo() {
    std::cout << "\n+ Username: " << username << '\n';
    std::cout << "+ Full name: " << get_name() << '\n';
    std::cout << "+ Phone number: " << phoneNumber << '\n';
    std::cout << "+ Email: " << email << '\n';
    std::cout << "+ Address: " << address << '\n';
    std::cout << "+ Credit points: " << creditPoints << '\n';

    if (ownedSkill != nullptr) {
        std::cout << "+ Owned skill: " << ownedSkill->getSkillInfo() << '\n';
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
bool Member::listSkill(DateTime *startTime, DateTime *endTime, int creditPointsPerHour, std::optional<double> minimumHostRating) {
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

        // Store the result of getRatingScore in a tuple
        auto ratingScores = request->requestedByMember->getRatingScore();

        // Extract the Host Rating Score using std::get
        double hostRatingScore = std::get<2>(ratingScores);

        std::cout << "+ " << i+1 << ". " << request->requestFrom->toString() << " - " << request->requestFrom->toString() << ", " << "Member: " << request->requestedByMember->get_name() << ", Host Rating: " << hostRatingScore << '\n';
    }
    return true;
}

bool Member::denyRequest(int acceptedRequestID) {
    // Check if the acceptedRequestID is valid
    if (acceptedRequestID < 0 || acceptedRequestID >= ownedSkill->skillRequestList.size()) {
        std::cerr << "Invalid request ID\n";
        return false;
    }


    auto acceptedRequest = ownedSkill->skillRequestList[acceptedRequestID];

    // Create a vector to store the requests to remove
    std::vector<Request*> requestsToRemove;

    for (auto &request : ownedSkill->skillRequestList) {
        // Check if the request is the accepted request

        if (request == acceptedRequest) {
            continue;
        }

        // Check time overlap
        if (!(*request->requestTo < *acceptedRequest->requestFrom || *acceptedRequest->requestTo < *request->requestFrom)) {
            // Add to removal list if there's an overlap
            requestsToRemove.push_back(request);
        }
    }

    // Remove overlapping requests
    for (auto &request : requestsToRemove) {
        // Remove the request from the member's request list
        request->requestedByMember->RemoveFromRequestList(request);
        // Remove the request from the skill's request list
        ownedSkill->skillRequestList.erase(std::remove(ownedSkill->skillRequestList.begin(), ownedSkill->skillRequestList.end(), request), ownedSkill->skillRequestList.end());
    }

    return true;
}

//Remove the request from member request list.
bool Member::RemoveFromRequestList(Request *removedRequest) {
    memberRequestList.erase(std::find(memberRequestList.begin(), memberRequestList.end(), removedRequest));
    return true;
}

bool Member::acceptRequest(int acceptedRequestID) {
    // Check if the acceptedRequestID is valid
    if (acceptedRequestID < 0 || acceptedRequestID >= ownedSkill->skillRequestList.size()) {
        std::cerr << "Invalid request ID\n";
        return false;
    }

    auto acceptedRequest = ownedSkill->skillRequestList[acceptedRequestID];

    // Parameter for SkillRent
    auto rentStartTime = acceptedRequest->requestFrom;
    auto rentEndTime = acceptedRequest->requestTo;
    auto rentMember = acceptedRequest->requestedByMember;

    int durationInHours = (*rentEndTime - *rentStartTime) / 60; // Convert to hour

    // Calculate the required credit points
    int requiredCreditPoints = durationInHours * ownedSkill->creditCostPerHour;
    if (rentMember->creditPoints < requiredCreditPoints) {
        // Remove the request from the member's request list
        rentMember->RemoveFromRequestList(acceptedRequest);

        // Remove the accepted request from the skill's request list
        ownedSkill->skillRequestList.erase(std::remove(ownedSkill->skillRequestList.begin(), ownedSkill->skillRequestList.end(), acceptedRequest), ownedSkill->skillRequestList.end());

        return false;
    }

    // Create the according objects
    auto *skillRent = new SkillRent(rentStartTime, rentEndTime, rentMember);
    auto *memberRent = new MemberRent(rentStartTime, rentEndTime, ownedSkill);

    // Add to skillRentList
    ownedSkill->skillRentList.push_back(skillRent);

    // Add host to the list
    rentMember->addHost(memberRent);

    // Reject other requests if time is overlapped
    denyRequest(acceptedRequestID);

    // Remove the request from request list
    rentMember->RemoveFromRequestList(acceptedRequest);

    // Remove the accepted request from the skill's request list
    ownedSkill->skillRequestList.erase(std::remove(ownedSkill->skillRequestList.begin(), ownedSkill->skillRequestList.end(), acceptedRequest), ownedSkill->skillRequestList.end());

    // Deduct the host's credit points and add to the supporter's credit points
    rentMember->minusCreditPoints(requiredCreditPoints);
    this->addCreditPoints(requiredCreditPoints);

    return true;
}

bool Member::addHost(MemberRent *addedRent) {
    memberRentList.push_back(addedRent);
    return true;
}

bool Member::addCreditPoints(int points) {
    creditPoints += points;
    return true;
}

bool Member::minusCreditPoints(int points) {
    if (creditPoints < points) {
        return false;
    }

    creditPoints -= points;
    return true;
}

bool Member::completeRequest(int completedSkillID, bool isSupporter) {
    // Check completedSkillID is valid
    if (completedSkillID < 0 || completedSkillID >= memberRentList.size()) {
        std::cerr << "Invalid skill ID\n";
        return false;
    }

    auto completedSkill = memberRentList[completedSkillID];
    auto completedSkillFrom = completedSkill->rentFrom;
    auto completedSkillTo = completedSkill->rentTo;
    auto completedRentSkill = completedSkill->rentSkill;


    SkillRent *completedSession;
    if (isSupporter) {
        // Use the constructor for supportedByMember
        completedSession = new SkillRent(completedSkillFrom, completedSkillTo, this, true);
    } else {
        // Use the constructor for rentedByMember
        completedSession = new SkillRent(completedSkillFrom, completedSkillTo, this);
    }

    completedRentSkill->addCompletedSession(completedSession);
    removeHost(completedSkill);

    return true;
}

bool Member::removeHost(MemberRent *completedRequest) {
    memberRentList.erase(std::find(memberRentList.begin(), memberRentList.end(), completedRequest));
    return true;
}

bool Member::rateSupporterAndSkill(int reviewingHostID, int skillRating, int supporterRating, std::string comment) {
    // Check the boundary
    if (reviewingHostID >= ownedSkill->completedSkillList.size()) {
        return false;
    }

    auto ratedSkill = ownedSkill->completedSkillList[reviewingHostID];

    auto ratedHost = ratedSkill->rentedByMember;

    if (skillRating < 1 || skillRating > 5 || supporterRating < 1 || supporterRating > 5) {
        return false; //Invalid rating score
    }

    RatingScores scores(skillRating, supporterRating, 0);

    auto newRating = new Rating(scores, std::move(comment), this);
    ratedHost->addRatingToMemberRentList(newRating);

    return true;
}

bool Member::addToRequestList(Request *addedRequest) {
    memberRequestList.push_back(addedRequest);
    return true;
}

bool Member::showCurrentSkillRent() {
    if (memberRentList.empty()) {
        std::cout << "You do not have any supported skills right now\n\n";
        return false;
    }

    std::cout << "Your current supported skills list:\n\n";
    for (int i = 0; i < memberRentList.size(); i++) {
        auto rent = memberRentList[i];
        std::cout << "+ " << i+1 << ". " << rent->rentSkill->getSkillInfo() << ", " << rent->rentFrom->toString() << " - " << rent->rentTo->toString() << '\n';
    }

    return true;
}

bool Member::addRatingToMemberRentList(Rating *memberRating) {
    memberRatingList.push_back(memberRating);
    return true;
}

bool Member::rateHost(Member* host, int hostRating, std::string comment) {
    if (host == nullptr) {
        return false;
    }

    if (hostRating < 1 || hostRating > 5) {
        return false; //Invalid rating score
    }

    RatingScores scores(0, 0, hostRating);

    auto newRating = new Rating(scores, std::move(comment), this);
    return true;
}

std::string Member::viewSupporterRateHost() {
    std::stringstream ss;
    if (memberRatingList.empty()) {
        return "This host has no reviews.\n\n";
    }

    ss << "The reviews for this host:\n\n";
    for (int i = 0; i < memberRatingList.size(); i++) {
        auto rating = memberRatingList[i];
        auto comment = rating->comment;
        auto hostScore = rating->scores.getHostRating();
        auto reviewedByMember = rating->reviewedByMember;
        ss << "+ " << i+1 << ". " << reviewedByMember->get_name() << " - " << comment << ", " << "Host Rating: " << hostScore << '\n';
    }
    return ss.str();
}

std::string Member::viewHostRateSupporter() {
    std::stringstream ss;
    if (memberRatingList.empty()) {
        return "This supporter has no reviews.\n\n";
    }

    ss << "The reviews for this supporter:\n\n";
    for (int i = 0; i < memberRatingList.size(); i++) {
        auto rating = memberRatingList[i];
        auto comment = rating->comment;
        auto skillScore = rating->scores.getSkillRating();
        auto supporterScore = rating->scores.getSupporterRating();
        auto reviewedByMember = rating->reviewedByMember;
        ss << "+ " << i+1 << ". " << reviewedByMember-> get_name() << " - " << comment << ", " << "Skill Rating: " << skillScore << ", Supporter Rating: " << supporterScore << '\n';
    }
    return ss.str();
}

std::string Member::guestViewSupporterInfo() {
    std::stringstream ss;
    ss << "Username: " << username;
    return ss.str();
}

std::string Member::viewSupporterInfoInDetail() {
    std::stringstream ss;
    ss << "+ Username: " << username << "\n" << "+ Full name: " << get_name() << "\n" << "+ Phone number: " << phoneNumber << "\n" << "+ Email: " << email << "\n" << "+ Address: " << address << "\n" << "\n" << "+ Skill: "; 
    if (ownedSkill != nullptr) {
        ss << ownedSkill->getSkillInfo();
    } else {
        ss << "None";
    }
    ss << "\n";
    return ss.str();
}

bool Member::blockMember(Member* memberToBlock, bool blockView, bool blockRequestSupport) {
    if (memberToBlock == nullptr || memberToBlock == this) {
        return false;
    }

    int blockedID = memberToBlock->memberID;
    auto it = blockedMembers.find(blockedID);
    if (it != blockedMembers.end()) {
        it->second->updateBlockSettings(blockView, blockRequestSupport);
    } else {
        blockedMembers[blockedID] = new BlockedMember(this->memberID, blockedID, blockView, blockRequestSupport);
    }

    return true;
}

void Member::setPassword(const std::string& newPassword) {
    password = newPassword;
}

// View the completed session
bool Member::showCompletedSession() {
    if (ownedSkill->completedSkillList.empty()) {
        std::cout << "There aren't any completed sessions\n\n";
        return false;
    }

    std::cout << "The list of completed sessions:\n\n";
    int i = 1;
    for (auto &completedSession : ownedSkill->completedSkillList) {
        auto from = completedSession->rentFrom;
        auto to = completedSession->rentTo;
        auto host = completedSession->rentedByMember;
        auto supporter = completedSession->supportedByMember;

        std::cout << "-->\t" << i << ". " << from->toString() << " - " << to->toString() << ", Rented by: " << host->get_name() << ", Supported by: " << supporter->get_name() << "\n";
        i++;
    }

    return true;
}

void Member::addBlockedMember(Member* blockedMember, bool blockView, bool blockRequestSupport) {
    if (blockedMember == nullptr) {
        // Handle the error appropriately
        return;
    }

    int blockedID = blockedMember->memberID; // Assuming Member has a method getId()

    // Check if the member is already blocked
    auto it = blockedMembers.find(blockedID);
    if (it != blockedMembers.end()) {
        // Update existing block settings
        it->second->updateBlockSettings(blockView, blockRequestSupport);
    } else {
        // Create a new BlockedMember and add it to the map
        blockedMembers[blockedID] = new BlockedMember(this->memberID, blockedID, blockView, blockRequestSupport);
    }
}

bool Member::isMemberBlocked(Member* member) {
    if (member == nullptr) {
        return false;
    }

    return blockedMembers.find(member->memberID) != blockedMembers.end();
}

bool Member::isBlockedForViewing(Member* member) {
    if (member == nullptr) {
        return false;
    }

    auto it = blockedMembers.find(member->memberID);
    if (it != blockedMembers.end()) {
        return it->second->isBlockView();
    }
    return false;
}

bool Member::isBlockedForRequesting(Member* member) {
    if (member == nullptr) {
        return false;
    }

    auto it = blockedMembers.find(member->memberID);
    if (it != blockedMembers.end()) {
        return it->second->isBlockRequestSupport();
    }
    return false;
}

// Destructor
Member::~Member() {
    delete ownedSkill;
    for (auto &rent : memberRentList) {
        delete rent;
    }
    for (auto &rating : memberRatingList) {
        delete rating;
    }
    for (auto &request : memberRequestList) {
        delete request;
    }
    for (auto &block : blockedMembers) {
        delete block.second;
    }
}