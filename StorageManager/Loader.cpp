#include <fstream>
#include <sstream>

#include "./Path.h"
#include "../System/System.h"
#include "../Admin/Admin.h"
#include "../Member/Member.h"
#include "../Member/BlockedMember.h"
#include "../Skill/Skill.h"
#include "../MemberRent/MemberRent.h"
#include "../DateTime/DateTime.h"
#include "../Request/Request.h"
#include "../Rating/Rating.h"
#include "../SkillRent/SkillRent.h"
#include "../Middleware/Middleware.h"
#include "Loader.h"


InputData::InputData() {
    // Clear all data from three lists
    inputStorageMemberList.clear();
    inputStorageSkillList.clear();
}

void InputData::inputStorageLoadSkillListToSystem(System *system) {
    // Push the Skill object to the system's Skill list
    for (auto &skill: inputStorageSkillList) {
        system->systemSkillList.push_back(skill.second);
    }
}

void InputData::inputStorageLoadMemberListToSystem(System *system) {
    system->systemMemberList.clear();

    // Add member to the system's member list
    for (auto &member: inputStorageMemberList) {
        system->addMember(member.second);
    }
}

void InputData::inputStorageLoadAdminListToSystem(System *system) {
    system->systemAdminList.clear();

    // Add admin to the system's admin list
    for (auto &admin: inputStorageAdminList) {
        system->addAdmin(admin);
    }
}

void InputData::inputMemberListFromFile() {
    std::ifstream is {MEMBER_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {

        // ss1 for splitting comma
        std::stringstream ss1 {line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        // Declare all variable
        int memberID = convertStringToInt(wordList[0]);
        std::string username = wordList[1];
        std::string password = wordList[2];
        std::string firstName = wordList[3];
        std::string lastName = wordList[4];
        std::string phoneNumber = wordList[5];
        std::string email = wordList[6];
        std::string address = wordList[7];
        int creditPoints = convertStringToInt(wordList[8]);

        // Declare the member account
        auto *member = new Member(memberID, username, password, firstName, lastName, phoneNumber, email, address, creditPoints);
        inputStorageMemberList[memberID] = member;
    }
}

void InputData::inputSkillListFromFile() {
    std::ifstream is{SKILL_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << SKILL_PATH << " for input\n";
        return;
    }

    std::string line;
    while (std::getline(is, line)) {
        if (line.empty()) {
            continue;  // Skip empty lines
        }

        std::stringstream ss(line);
        std::string word;
        std::vector<std::string> tokens;

        while (std::getline(ss, word, ',')) {
            tokens.push_back(word);
        }

        int skillID = convertStringToInt(tokens[0]);
        std::vector<std::string> skillList;
        
        
        std::stringstream ssSkills(tokens[1]);
        while (std::getline(ssSkills, word, ';')) {  // Skills are separated by semi-colons
            skillList.push_back(word);
        }

        int cityID = convertStringToInt(tokens[2]);

        // Create a Skill object and store it in the inputStorageSkillList
        Skill *skill = new Skill(skillID, skillList, cityID);
        inputStorageSkillList[skillID] = skill;
    }
}


void InputData::inputMemberOwnSkillFromFile() {
    std::ifstream is{MEMBER_OWN_SKILL_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_OWN_SKILL_PATH << " for input\n";
        return;
    }

    std::string line;
    while (std::getline(is, line)) {
        if (line.empty()) {
            continue;  // Skip empty lines
        }

        std::stringstream ss(line);
        std::string word;
        std::vector<std::string> wordList;

        while (std::getline(ss, word, ',')) {
            wordList.push_back(word);
        }

        int memberID = convertStringToInt(wordList[0]);
        int skillID = convertStringToInt(wordList[1]);

        // Retrieve the member and skill objects from their respective storage maps
        auto memberIt = inputStorageMemberList.find(memberID);
        auto skillIt = inputStorageSkillList.find(skillID);

        // Check if both member and skill exist before adding
        if (memberIt != inputStorageMemberList.end() && skillIt != inputStorageSkillList.end()) {
            Member* member = memberIt->second;
            Skill* skill = skillIt->second;

            // Set the member's owned skill
            if(!member->setNewSkill(skill)) {
                std::cerr << "Failed to set skill for member ID " << memberID << "\n";
            }
        } else {
            std::cerr << "Invalid member ID or skill ID in data file: " << line << "\n";
        }
    }
}


void InputData::inputAdminListFromFile() {
    std::ifstream is {ADMIN_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << ADMIN_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {

//        std::cout << line << "\n";
        // ss1 for splitting comma
        std::stringstream ss1 {line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        // Declare all variable
        std::string username = wordList[0];
        std::string password = wordList[1];

        auto *admin = new Admin(username, password);
        inputStorageAdminList.push_back(admin);
    }
}

void InputData::inputBlockedMemberListFromFile() {
    std::ifstream is{MEMBER_BLOCK_LIST_PATH};

    if(!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_BLOCK_LIST_PATH << " for input\n";
        return;
    }

    std::string line;

    while (std::getline(is, line)) {
        std::stringstream ss(line);
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            continue;
        }

        while(std::getline(ss, word, ',')) {
            wordList.push_back(word);
        }

        int blockerID = convertStringToInt(wordList[0]);
        int blockedID = convertStringToInt(wordList[1]);
        std::cout << "Parsed Block Relationship: Blocker ID " << blockerID << ", Blocked ID " << blockedID << "\n";
        bool blockView = convertStringToBool(wordList[2]);
        bool blockRequestSupport = convertStringToBool(wordList[3]);

        auto blockerIt = inputStorageMemberList.find(blockerID);
        auto blockedIt = inputStorageMemberList.find(blockedID);

        if (blockerIt != inputStorageMemberList.end() && blockedIt != inputStorageMemberList.end()) {
            // Update the blocker member with the block information
            blockerIt->second->addBlockedMember(blockedIt->second, blockView, blockRequestSupport);
        } else {
            // Handle edge cases
            if (blockerIt == inputStorageMemberList.end()) {
                std::cerr << "Blocker member with ID " << blockerID << " not found.\n";
            }
            if (blockedIt == inputStorageMemberList.end()) {
                std::cerr << "Blocked member with ID " << blockedID << " not found.\n";
            }
        }
    }
}

void InputData::inputMemberListSkillFromFile() {
    std::ifstream is{MEMBER_LIST_SKILL_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_LIST_SKILL_PATH << " for input\n";
        return;
    }

    std::string line;

    while (std::getline(is, line)) {
        std::stringstream ss(line);
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            continue;  // Skip empty lines
        }

        while (std::getline(ss, word, ',')) {
            wordList.push_back(word);
        }

        DateTime *availableFrom = convertStringToDateTime(wordList[0]);
        DateTime *availableTo = convertStringToDateTime(wordList[1]);
        int creditPointsPerHour = convertStringToInt(wordList[2]);
        std::optional<double> minHostRating = convertStringToOptionalDouble(wordList[3]);
        int ownerMemberID = convertStringToInt(wordList[4]);

        // Get the owner and their skill
        auto ownerMemberIt = inputStorageMemberList.find(ownerMemberID);
        if (ownerMemberIt != inputStorageMemberList.end()) {
            Member* ownerMember = ownerMemberIt->second;

            // Check if the member has a skill to list
            if (ownerMember->ownedSkill != nullptr) {
                // List the skill
                ownerMember->listSkill(availableFrom, availableTo, creditPointsPerHour, minHostRating);
            } else {
                std::cerr << "Member ID " << ownerMemberID << " does not own a skill.\n";
            }
        } else {
            std::cerr << "Invalid member ID: " << ownerMemberID << "\n";
        }
    }
}


void InputData::inputMemberRequestSkillFromFile() {
    std::ifstream is{MEMBER_REQUEST_SKILL_PATH}; // Adjust the file path accordingly

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_REQUEST_SKILL_PATH << " for input\n";
        return;
    }

    std::string line;
    while (std::getline(is, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }

        std::stringstream ss(line);
        std::string word;
        std::vector<std::string> wordList;

        while (std::getline(ss, word, ',')) {
            wordList.push_back(word);
        }

        DateTime *requestFromDate = convertStringToDateTime(wordList[0]);
        DateTime *requestToDate = convertStringToDateTime(wordList[1]);
        int memberID = convertStringToInt(wordList[2]);
        int skillID = convertStringToInt(wordList[3]);

        // Retrieve the member and skill objects
        auto memberIt = inputStorageMemberList.find(memberID);
        auto skillIt = inputStorageSkillList.find(skillID);

        if (memberIt != inputStorageMemberList.end() && skillIt != inputStorageSkillList.end()) {
            Member* member = memberIt->second;
            Skill* skill = skillIt->second;

            // Create and store the request
            auto *request = new Request(requestFromDate, requestToDate, member);
            member->addToRequestList(request); 
            skill->addRequestToSkillRequestList(request); 
        } else {
            std::cerr << "Invalid member ID or skill ID in data file: " << line << "\n";
        }
    }
}


void InputData::inputMemberRentSkillFromFile() {
    std::ifstream is{MEMBER_RENT_SKILL_PATH}; // Adjust the file path accordingly

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_RENT_SKILL_PATH << " for input\n";
        return;
    }

    std::string line;
    while (std::getline(is, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }

        std::stringstream ss(line);
        std::string word;
        std::vector<std::string> wordList;

        while (std::getline(ss, word, ',')) {
            wordList.push_back(word);
        }

        DateTime *rentFromDate = convertStringToDateTime(wordList[0]);
        DateTime *rentToDate = convertStringToDateTime(wordList[1]);
        int memberID = convertStringToInt(wordList[2]);
        int skillID = convertStringToInt(wordList[3]);

        // Retrieve the member and skill objects
        auto memberIt = inputStorageMemberList.find(memberID);
        auto skillIt = inputStorageSkillList.find(skillID);

        if (memberIt != inputStorageMemberList.end() && skillIt != inputStorageSkillList.end()) {
            Member* member = memberIt->second;
            Skill* skill = skillIt->second;

            // Create and store the skill rental
            auto *skillRent = new SkillRent(rentFromDate, rentToDate, member);
            skill->addSkillRent(skillRent); // Use the public method to add to skillRentList
        } else {
            std::cerr << "Invalid member ID or skill ID in data file: " << line << "\n";
        }
    }
}


void InputData::inputMemberRatingSkillAndSupporterFromFile() {
    std::ifstream is{MEMBER_RATING_SKILL_PATH}; // Adjust the file path accordingly

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_RATING_SKILL_PATH << " for input\n";
        return;
    }

    std::string line;
    while (std::getline(is, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }

        std::stringstream ss(line);
        std::string word;
        std::vector<std::string> wordList;

        while (std::getline(ss, word, ',')) {
            wordList.push_back(word);
        }

        int skillRating = convertStringToInt(wordList[0]);
        int supporterRating = convertStringToInt(wordList[1]);
        std::string comment = wordList[2];
        int memberID = convertStringToInt(wordList[3]);
        int skillID = convertStringToInt(wordList[4]);

        // Retrieve the member and skill objects
        auto memberIt = inputStorageMemberList.find(memberID);
        auto skillIt = inputStorageSkillList.find(skillID);

        if (memberIt != inputStorageMemberList.end() && skillIt != inputStorageSkillList.end()) {
            Member* member = memberIt->second;
            Skill* skill = skillIt->second;

            // Create and store the rating
            RatingScores scores(skillRating, supporterRating, 0); //
            auto *rating = new Rating(scores, comment, member);

            // Add rating to both Member and Skill if needed
            member->addRatingToMemberRentList(rating);
            // If Skill class also needs to store ratings, add a method in Skill class and call it here
            // skill->addRating(rating);
        } else {
            std::cerr << "Invalid member ID or skill ID in data file: " << line << "\n";
        }
    }
}



void InputData::inputMemberRatingHostFromFile() {
    std::ifstream is{MEMBER_RATING_RENTER_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_RATING_RENTER_PATH << " for input\n";
        return;
    }

    std::string line;
    while (std::getline(is, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }

        std::stringstream ss(line);
        std::string word;
        std::vector<std::string> wordList;

        while (std::getline(ss, word, ',')) {
            wordList.push_back(word);
        }


        int hostRating = convertStringToInt(wordList[0]);
        std::string comment = wordList[1];
        int memberID = convertStringToInt(wordList[2]);
        int reviewerID = convertStringToInt(wordList[3]); // ID of the member who is giving the review

        // Retrieve the member and the reviewer objects from storage
        auto memberIt = inputStorageMemberList.find(memberID);
        auto reviewerIt = inputStorageMemberList.find(reviewerID);

        if (memberIt != inputStorageMemberList.end() && reviewerIt != inputStorageMemberList.end()) {
            Member* member = memberIt->second;
            Member* reviewer = reviewerIt->second;

            // Create and store the rating
            RatingScores scores(0, 0, hostRating);
            auto *rating = new Rating(scores, comment, reviewer);

            // Add rating to the member's rating list
            member->addRatingToMemberRentList(rating);
        } else {
            std::cerr << "Invalid member ID or reviewer ID in data file: " << line << "\n";
        }
    }
}


void InputData::inputCompletedSessionFromFile() {
    std::ifstream is{COMPLETED_SESSION_LIST_PATH}; // Adjust this to your actual file path

    // Check if file is open
    if (!is.is_open()) {
        std::cerr << "Cannot open " << COMPLETED_SESSION_LIST_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }

        std::stringstream ss(line);
        std::string word;
        std::vector<std::string> wordList;

        while (std::getline(ss, word, ',')) {
            wordList.push_back(word);
        }

        // Parse data
        DateTime *rentFrom = convertStringToDateTime(wordList[0]);
        DateTime *rentTo = convertStringToDateTime(wordList[1]);
        int skillID = convertStringToInt(wordList[2]);
        int supporterID = convertStringToInt(wordList[3]);

        // Retrieve Member and Skill objects
        auto supporterIt = inputStorageMemberList.find(supporterID);
        auto skillIt = inputStorageSkillList.find(skillID);

        // Check if Member and Skill exist
        if (supporterIt != inputStorageMemberList.end() && skillIt != inputStorageSkillList.end()) {
            // Create a new SkillRent object for the unrated session
            auto *completedSession = new SkillRent(rentFrom, rentTo, supporterIt->second);
            // Add this unrated session to the Skill object
            skillIt->second->addSkillRent(completedSession); 
        } else {
            std::cerr << "Invalid Member ID or Skill ID in data file: " << line << "\n";
        }
    }
}

void InputData::inputStorageFromFileList() {
    inputMemberListFromFile();
    inputSkillListFromFile();
    inputAdminListFromFile();
    inputBlockedMemberListFromFile();
    inputMemberOwnSkillFromFile();
    inputMemberListSkillFromFile();
    inputMemberRequestSkillFromFile();
    inputMemberRentSkillFromFile();
    inputMemberRatingSkillAndSupporterFromFile();
    inputMemberRatingHostFromFile();
    inputCompletedSessionFromFile();
}

void InputData::inputStorageLoadDataToSystem(System *system) {
    inputStorageLoadMemberListToSystem(system);
    inputStorageLoadSkillListToSystem(system);
    inputStorageLoadAdminListToSystem(system);
}


