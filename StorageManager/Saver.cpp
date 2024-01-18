#include <fstream>
#include <sstream>
#include <vector>

#include "Path.h"
#include "../System/System.h"
#include "../Admin/Admin.h"
#include "../Member/Member.h"
#include "../Skill/Skill.h"
#include "../MemberRent/MemberRent.h"
#include "../DateTime/DateTime.h"
#include "../Request/Request.h"
#include "../Rating/Rating.h"
#include "../SkillRent/Skillrent.h"
#include "../Middleware/Middleware.h"
#include "../Member/BlockedMember.h"
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

void OutputData::outputStorageBlockedMemberListFromSystem(System *system) {
    for (auto &blockedMember: system->systemBlockedMemberList) {
        outputStorageBlockedMemberList.push_back(blockedMember);
    }
}

// Output data to Storage/Member.csv
void OutputData::outputMemberListToFile() {
    std::ofstream os{MEMBER_PATH};  // Ensure MEMBER_PATH matches your file path

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_PATH << " for output\n";
        return;
    }


    bool isFirstMember = true;
    // Print to file following the structure in StorageStructure folder
    for (auto &member: outputStorageMemberList) {
        if (!isFirstMember) {
            os << "\n";
        }
        os << member->memberID << ","
           << member->username << ","
           << member->password << ","
           << member->firstName << ","
           << member->lastName << ","
           << member->phoneNumber << ","
           << member->email << ","
           << member->address << ","
           << member->creditPoints;

        isFirstMember = false;
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
void OutputData::outputMemberRatingSkillAndSupporterToFile() {
    std::ofstream os {MEMBER_RATING_SKILL_PATH};  // Adjust the file path as needed

    if (!os.is_open()) {
        std::cerr << "Cannot open " << MEMBER_RATING_SKILL_PATH << " for output\n";
        return;
    }

    for (auto &member: outputStorageMemberList) {
        for (auto &rating: member->memberRatingList) {
            // Assuming that getSkillRating() returns the skill rating and reviewedByMember points to the member who made the rating
            os << rating->scores.getSkillRating() << ","
               << rating->scores.getSupporterRating() << ","
               << rating->comment << ","
               << rating->reviewedByMember->memberID << ","  // ID of the member who wrote the review
               << member->memberID << "\n";  // ID of the member whose skill was rated
        }
    }

    os.close();
}


//Output skillOwner review Renter to data file /Storage/MemberReviewRenter.csv
void OutputData::outputMemberRatingHostToFile()
{
    std::ofstream os {MEMBER_RATING_RENTER_PATH};

    if (!os.is_open()) {
        std::cerr << "Cannot open " << MEMBER_RATING_RENTER_PATH << " for output\n";
        return;
    }

    for (auto &member: outputStorageMemberList) {
        for (auto &memberReview: member->memberRatingList) {
            std::stringstream ss;
            ss << memberReview->scores.getHostRating() << "," //score rating
               << memberReview->comment << ","     //comment
               << memberReview->reviewedByMember->memberID << "," //memberID
               << member->memberID << "\n";          //renterID
            std::string line = ss.str();
            os << line;
        }
    }

    os.close();
}

//Output renter id and skill info that unrated the skill after rent. /Storage/CompletedSessionList.csv
void OutputData::outputCompletedSessionListToFile()
{
    std::ofstream os {COMPLETED_SESSION_LIST_PATH};

    for (auto &skill: outputStorageSkillList) {
        for (auto &completedSession: skill->completedSkillList) {
            os << completedSession->rentFrom->toString() << "," //scorerating
            << completedSession->rentTo->toString() << ","     //comment
            << skill->skillID << ","           //skillID
            << completedSession->rentedByMember->memberID <<"\n"; //renterID
        }
    }

    os.close();
}

void OutputData::updatePasswordtoFile(std::vector<Member*> memberList) {
    std::ofstream os(MEMBER_PATH, std::ios::trunc);

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_PATH << " for output\n";
        return;
    }

    for (auto it = memberList.begin(); it != memberList.end(); ++it) {
        Member* member = *it;
        if (it != memberList.begin()) {
            os << "\n";
        }
        os << member->memberID << ","
           << member->username << ","
           << member->password << "," 
           << member->firstName << ","
           << member->lastName << ","
           << member->phoneNumber << ","
           << member->email << ","
           << member->address << ","
           << member->creditPoints;
    }

    os.close();
}

void OutputData::outputBlockedMemberToFile() {  
    std::ofstream os(MEMBER_BLOCK_LIST_PATH, std::ios::trunc);

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_BLOCK_LIST_PATH << " for output\n";
        return;
    }

    // Check if the file is empty
    os.seekp(0, std::ios::end); // Move to the end of the file
    bool isEmpty = os.tellp() == 0; // Check if the file position is at the start

    // If the file is not empty and there are blocked members to write, add a newline first
    if (!isEmpty && !outputStorageBlockedMemberList.empty()) {
        os << "\n";
    }

    for (auto &blockedMember : outputStorageBlockedMemberList) {
        os << blockedMember->getBlockerID() << ","
           << blockedMember->getBlockedID() << ","
           << blockedMember->isBlockView() << ","
           << blockedMember->isBlockRequestSupport() << "\n";
    }

    os.close();
}

void OutputData::outputStorageLoadDataFromSystem(System *system) {
    outputStorageLoadMemberListFromSystem(system);
    outputStorageLoadSkillListFromSystem(system);
    outputStorageBlockedMemberListFromSystem(system);
}


void OutputData::outputStorageToFileList() {
    outputMemberListToFile();
    outputSkillListToFile();
    outputMemberOwnSkillToFile();
    outputMemberListSkillToFile();
    outputMemberRequestSkillToFile();
    outputMemberRentSkillToFile();
    outputMemberRatingSkillAndSupporterToFile();
    outputMemberRatingHostToFile();
    outputCompletedSessionListToFile();
    outputBlockedMemberToFile();
}


