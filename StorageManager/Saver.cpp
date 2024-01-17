#include <fstream>
#include <sstream>
#include <vector>

#include "Path.h"
#include "System\System.h"
#include "Admin\Admin.h"
#include "Member\Member.h"
#include "Skill\Skill.h"
#include "MemberRent\MemberRent.h"
#include "DateTime\DateTime.h"
#include "Request\Request.cpp"
#include "Rating\Rating.h"
#include "SkillRent\Skillrent.h"
#include "Middleware\Middleware.h"
#include "Saver.h"

OutputData::OutputData() {
    this->outputStorageSkillList.clear();
    this->outputStorageMemberList.clear();
}

void OutputData::outputStorageLoadMemberListFromSystem(System *system) {
    for (auto &member: system->systemMemberList) {
        outputStorageMemberList.push_back(member);
    }
}

void OutputData::outputStorageLoadSkillListFromSystem(System *system) {
    for (auto &skill: system->systemSkillList) {
        outputStorageSkillList.push_back(skill);
    }
}



// Output data to Storage/Member.csv
void OutputData::outputMemberListToFile() {
    std::ofstream os{MEMBER_PATH};  // Ensure MEMBER_PATH matches your file path

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_PATH << " for output\n";
        return;
    }

    // Print to file following the structure in StorageStructure folder
    for (auto &member: outputStorageMemberList) {
        os << member->memberID << ","
           << member->username << ","
           << member->password << ","
           << member->firstName << ","
           << member->lastName << ","
           << member->phoneNumber << ","
           << member->email << ","
           << member->address << ","
           << member->creditPoints << "\n";
    }

    // Close the file
    os.close();
}


void OutputData::outputSkillListToFile() {
    std::ofstream os {SKILL_PATH};  

    if (!os) {
        std::cerr << "Cannot open " << SKILL_PATH << " for output\n";
        return;
    }

    for (auto &skill: outputStorageSkillList) {
        std::string skillListStr;
        // Use the getter method to access skillList
        for (const auto& skillName : skill->getSkillList()) {
            skillListStr += skillName + ";";
        }
        if (!skillListStr.empty()) {
            skillListStr.pop_back();  // Remove the last semicolon
        }

        // Use the getter method to access skillID
        os << skill->getSkillID() << ","
           << skillListStr << ","
           << skill->getCityID() << "\n";  // Assuming getCityName() method provides city name
    }

    os.close();
}





// Output member own skill to Storage/MemberOwnSkill.csv
void OutputData::outputMemberOwnSkillToFile() {
    std::ofstream os {MEMBER_OWN_SKILL_PATH};

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_OWN_SKILL_PATH << " for output\n";
        return;
    }

    for (auto &member: outputStorageMemberList) {
        // Check if member owns a skill
        if (member->ownedSkill) {
            os << member->memberID << ","
               << member->ownedSkill->getSkillID() << "\n";
        }
    }

    os.close();
}


// Output data to Storage/MemberListSkill.csv
void OutputData::outputMemberListSkillToFile() {
    std::ofstream os {MEMBER_LIST_SKILL_PATH};

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_LIST_SKILL_PATH << " for output\n";
        return;
    }

    // Loop through the list of members
    for (auto &member: outputStorageMemberList) {
        // Check if member owns a skill and if it is listed
        if (member->ownedSkill && member->ownedSkill->isListed) {
            Skill* skill = member->ownedSkill;

            // Convert availableFrom and availableTo dates to string
            std::string availableFromStr = skill->getAvailableFrom() ? skill->getAvailableFrom()->toString() : "";
            std::string availableToStr = skill->getAvailableTo() ? skill->getAvailableTo()->toString() : "";

            // Output the info for listing skill
            os << availableFromStr << ","
               << availableToStr << ","
               << skill->getCreditCostPerHour() << ","
               << (skill->getMinHostRating().has_value() ? std::to_string(skill->getMinHostRating().value()) : "N/A") << ","
               << member->memberID << ","
               << skill->getSkillID() << "\n";
        }
    }

    os.close();
}


// Output member's skill request information to Storage/MemberRequestSkill.csv
void OutputData::outputMemberRequestSkillToFile() {
    std::ofstream os {MEMBER_REQUEST_SKILL_PATH};

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_REQUEST_SKILL_PATH << " for output\n";
        return;
    }

    // Loop through each member's skill request list
    for (auto &member: outputStorageMemberList) {
        for (auto &skillRequest: member->memberRequestList) {  // Assuming memberRequestList contains Request objects
            if (skillRequest->requestFrom && skillRequest->requestTo) {
                os << skillRequest->requestFrom->toString() << ","
                   << skillRequest->requestTo->toString() << ","
                   << member->memberID << "\n";  // Member ID who made the request
            }
        }
    }

    os.close();
}
// Output data for member renting skill to Storage/MemberRentSkill.csv
void OutputData::outputMemberRentSkillToFile() {
    std::ofstream os {MEMBER_RENT_SKILL_PATH};

    if (!os.is_open()) {
        std::cerr << "Cannot open " << MEMBER_RENT_SKILL_PATH << " for output\n";
        return;
    }

    for (auto &member: outputStorageMemberList) {
        for (auto &memberRent: member->memberRentList) {
            if (memberRent->rentFrom && memberRent->rentTo && memberRent->rentSkill) {
                os << memberRent->rentFrom->toString() << ","
                   << memberRent->rentTo->toString() << ","
                   << member->memberID << ","
                   << memberRent->rentSkill->getSkillID() << "\n";  // Assuming Skill class has a getSkillID method
            }
        }
    }

    os.close();
}


// Output member skill ratings to Storage/MemberRatingSkill.csv
void OutputData::outputMemberRatingSkillToFile() {
    std::ofstream os {MEMBER_RATING_SKILL_PATH};  // Adjust the file path as needed

    if (!os.is_open()) {
        std::cerr << "Cannot open " << MEMBER_RATING_SKILL_PATH << " for output\n";
        return;
    }

    for (auto &member: outputStorageMemberList) {
        for (auto &rating: member->memberRatingList) {
            // Assuming that getSkillRating() returns the skill rating and reviewedByMember points to the member who made the rating
            os << rating->scores.getSkillRating() << ","
               << rating->comment << ","
               << rating->reviewedByMember->getMemberID() << ","  // ID of the member who wrote the review
               << member->getMemberID() << "\n";  // ID of the member whose skill was rated
        }
    }

    os.close();
}


//Output MotorbikeOwner review Renter to data file /Storage/MemberReviewRenter.csv
void OutputData::outputMemberReviewRenterToFile()
{
    std::ofstream os {MEMBER_REVIEW_RENTER_PATH};

    if (!os.is_open()) {
        std::cerr << "Cannot open " << MEMBER_REVIEW_RENTER_PATH << " for output\n";
        return;
    }

    for (auto &member: outputStorageMemberList) {
        for (auto &memberReview: member->memberReviewList) {
            std::stringstream ss;
            ss << memberReview->ratingScore << "," //score rating
               << memberReview->comment << ","     //comment
               << memberReview->reviewedByMember->memberID << "," //memberID
               << member->memberID << "\n";          //renterID
            std::string line = ss.str();
            os << line;
        }
    }

    os.close();
}

//Output renter id and motorbike info that unrated the motorbike after rent. /Storage/MotorbikeUnratedRenter.csv
void OutputData::outputMotorbikeUnratedRenterToFile()
{
    std::ofstream os {MOTORBIKE_UNRATED_RENTER_PATH};

    for (auto &motorbike: outputStorageMotorbikeList) {
        for (auto &unratedRenter: motorbike->unratedRenterList) {
            os << unratedRenter->rentFromDate->toStr() << "," //scorerating
            << unratedRenter->rentToDate->toStr() << ","     //comment
            << motorbike->motorbikeID << ","           //motorbikeID
            << unratedRenter->rentedByMember->memberID <<"\n"; //renterID
        }
    }

    os.close();
}

void OutputData::outputStorageLoadDataFromSystem(System *system) {
    outputStorageLoadMemberListFromSystem(system);
    outputStorageLoadMotorbikeListFromSystem(system);
}


void OutputData::outputStorageToFileList() {
    outputMemberListToFile();
    outputMotorbikeListToFile();
    outputMemberOwnMotorbikeToFile();
    outputMemberListMotorbikeToFile();
    outputMemberRequestMotorbikeToFile();
    outputMemberRentMotorbikeToFile();
    outputMemberReviewMotorbikeToFile();
    outputMemberReviewRenterToFile();
    outputMotorbikeUnratedRenterToFile();
}


