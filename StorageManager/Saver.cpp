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

    bool isFirstMember = true;
    for (auto &skill: outputStorageSkillList) {
        if (!isFirstMember) {
            os << "\n";
        }
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
           << skill->getCityID();  // getCityName() method provides city name
        isFirstMember = false;
    }

    os.close();
}





// Output member own skill to Storage/MemberOwnSkill.csv
void OutputData::outputMemberOwnSkillToFile() {
    std::ofstream os{MEMBER_OWN_SKILL_PATH};

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_OWN_SKILL_PATH << " for output\n";
        return;
    }

    // Count the total number of entries to write
    int totalEntries = 0;
    for (auto &member : outputStorageMemberList) {
        if (member->ownedSkill) {
            totalEntries++;
        }
    }

    // Track the number of entries written
    int count = 0;

    // Loop through the list of members
    for (auto &member : outputStorageMemberList) {
        // Check if member owns a skill
        if (member->ownedSkill) {
            count++;
            os << member->memberID << ","
               << member->ownedSkill->getSkillID();

            // Add a newline if it's not the last entry
            if (count < totalEntries) {
                os << "\n";
            }
        }
    }

    os.close();
}



// Output data to Storage/MemberListSkill.csv
void OutputData::outputMemberListSkillToFile() {
    std::ofstream os{MEMBER_LIST_SKILL_PATH};

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_LIST_SKILL_PATH << " for output\n";
        return;
    }

    // Track the number of entries written
    int count = 0;
    int totalEntries = 0;

    // Count the total number of entries to write
    for (auto &member : outputStorageMemberList) {
        if (member->ownedSkill && member->ownedSkill->isListed) {
            totalEntries++;
        }
    }

    // Loop through the list of members
    for (auto &member : outputStorageMemberList) {
        if (member->ownedSkill && member->ownedSkill->isListed) {
            count++;
            Skill *skill = member->ownedSkill;

            std::string availableFromStr = skill->getAvailableFrom() ? skill->getAvailableFrom()->toString() : "";
            std::string availableToStr = skill->getAvailableTo() ? skill->getAvailableTo()->toString() : "";
            std::stringstream minHostRatingStream;
            if (skill->getMinHostRating().has_value()) {
                double minHostRating = skill->getMinHostRating().value();
                minHostRatingStream << std::fixed << std::setprecision(1) << minHostRating;
            } else {
                minHostRatingStream << "N/A";
            }

            os << availableFromStr << ","
               << availableToStr << ","
               << skill->getCreditCostPerHour() << ","
               << minHostRatingStream.str() << ","
               << member->memberID << ","
               << skill->getSkillID();

            // Add a newline if it's not the last entry
            if (count < totalEntries) {
                os << "\n";
            }
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

    // Count the total number of entries
    int totalEntries = 0;
    for (auto &skill : outputStorageSkillList) {
        totalEntries += skill->skillRequestList.size();
    }

    // Keep track of the number of entries written
    int count = 0;

    // Loop through each member's skill request list
    for (auto &skill: outputStorageSkillList) {
        for (auto &skillRequest: skill->skillRequestList) {  // memberRequestList contains Request objects
            if (skillRequest->requestFrom && skillRequest->requestTo) {
                count++;
                os << skillRequest->requestFrom->toString() << ","
                   << skillRequest->requestTo->toString() << ","
                   << skillRequest->requestedByMember->memberID << ","  // Member ID who made the request
                   << skill->skillID;
                if (count < totalEntries) {
                    os << "\n";
                }
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

    // Count the total number of entries
    int totalEntries = 0;
    for (auto &member : outputStorageMemberList) {
        totalEntries += member->memberRentList.size();
    }

    // Keep track of the number of entries written
    int count = 0;

    for (auto &member : outputStorageMemberList) {
        for (auto &memberRent : member->memberRentList) {
            if (memberRent->rentFrom && memberRent->rentTo && memberRent->rentSkill) {
                count++;
                os << memberRent->rentFrom->toString() << ","
                   << memberRent->rentTo->toString() << ","
                   << member->memberID << ","
                   << memberRent->rentSkill->getSkillID();

                // Add newline if not the last entry
                if (count < totalEntries) {
                    os << "\n";
                }
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

    // Count the total number of entries
    int totalEntries = 0;
    for (auto &member : outputStorageMemberList) {
        totalEntries += member->memberRatingSupporterAndSkillList.size();
    }

    // Keep track of the number of entries written
    int count = 0;

    for (auto &member: outputStorageMemberList) {
        for (auto &rating: member->memberRatingSupporterAndSkillList) {
            count++;
            //  that getSkillRating() returns the skill rating and reviewedByMember points to the member who made the rating
            os << rating->scores.getSkillRating() << ","
               << rating->scores.getSupporterRating() << ","
               << rating->comment << ","
               << rating->reviewedByMember->memberID << ","  // ID of the member who wrote the review
               << rating->receivedByMember->memberID;  // ID of the member whose skill was rated
            if (count < totalEntries) {
                os << "\n";
            }
        }
    }

    os.close();
}


//Output Supporter review Host to data file /Storage/MemberRatingHost.csv
void OutputData::outputMemberRatingHostToFile()
{
    std::ofstream os {MEMBER_RATING_HOST_PATH};

    if (!os.is_open()) {
        std::cerr << "Cannot open " << MEMBER_RATING_HOST_PATH << " for output\n";
        return;
    }

    // Count the total number of entries
    int totalEntries = 0;
    for (auto &member : outputStorageMemberList) {
        totalEntries += member->memberRatingHostList.size();
    }

    // Keep track of the number of entries written
    int count = 0;

    for (auto &member: outputStorageMemberList) {
        for (auto &memberReview: member->memberRatingHostList) {
            count++;
            os << memberReview->scores.getHostRating() << "," // Host rating score
               << memberReview->comment << ","                // Comment
               << memberReview->reviewedByMember->memberID << "," // Member ID (the one who gave the rating)
               << memberReview->receivedByMember->memberID;                             // Host ID (the one who received the rating)

            // Add newline if not the last entry
            if (count < totalEntries) {
                os << "\n";
            }
        }
    }

    os.close();
}



//Output renter id and skill info that unrated the skill after rent. /Storage/CompletedSessionList.csv
void OutputData::outputCompletedSessionListToFile()
{
    std::ofstream os {COMPLETED_SESSION_LIST_PATH};

    if (!os) {
        std::cerr << "Cannot open " << COMPLETED_SESSION_LIST_PATH << " for output\n";
        return;
    }

    // Count the total number of entries
    int totalEntries = 0;
    for (auto &skill : outputStorageMemberList) {
        totalEntries += skill->completedSessionList.size();
    }

    // Keep track of the number of entries written
    int count = 0;

    for (auto &skill: outputStorageMemberList) {
        for (auto &completedSession: skill->completedSessionList) {
            count++;
            os << completedSession->rentFrom->toString() << "," // Rent from
            << completedSession->rentTo->toString() << ","     // Rent to
            << completedSession->rentedByMember->memberID << ","  // hostID
            << completedSession->supportedByMember->memberID; // supporterID
            // Add newline if not the last entry
            if (count < totalEntries) {
                os << "\n";
            }
        }
        
    }

    os.close();
}

void OutputData::updateMemInfotoFile(std::vector<Member*> memberList) {
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
    std::ofstream os(MEMBER_BLOCK_LIST_PATH, std::ios::trunc); // Use trunc mode to overwrite the file

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_BLOCK_LIST_PATH << " for output\n";
        return;
    }

    // Count the total number of entries
    int totalEntries = 0;
    totalEntries += outputStorageBlockedMemberList.size();

    // Keep track of the number of entries written
    int count = 0;

    for (auto &blockedMember : outputStorageBlockedMemberList) {
        count++;
        os << blockedMember->getBlockerID() << ","
           << blockedMember->getBlockedID() << ","
           << blockedMember->isBlockView() << ","
           << blockedMember->isBlockRequestSupport();
        if (count < totalEntries) {
            os << "\n";
        }
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


