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


InputData::InputData() {
    // Clear all data from two lists
    inputStorageMemberList.clear();
    inputStorageSKILLList.clear();
}

void InputData::inputStorageLoadSKILLListToSystem(System *system) {
    // Push the Skill object to the system's Skill list
    for (auto &Skill: inputStorageSKILLList) {
        system->systemSkillList.push_back(Skill.second);
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

        // ss1 for splitting comma, ss2 for storage data
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
        std::string id_type = wordList[6];
        std::string passport_number = wordList[7];
        std::string driver_license_number = wordList[8];
        std::string expiry_date = wordList[9];
        int creditPoints = convertStringToInt(wordList[10]);

        // // Declare the member account
        // auto *member = new Member(memberID, username, password, firstName, lastName, phoneNumber, id_type, passport_number, driver_license_number, expiry_date, creditPoints);
        // inputStorageMemberList[memberID] = member; <?????>
    }
} 

void InputData::inputSKILLListFromFile() {
    std::ifstream is {MEMBER_OWN_SKILL_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_OWN_SKILL_PATH << " for input\n";
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

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        // Declare all variable
        int SkillID = convertStringToInt(wordList[0]);
        std::string SkillModel = wordList[1];
        std::string SkillColor = wordList[2];
        std::string SkillEngineSize = wordList[3];
        std::string transmissionMode = wordList[4];
        std::string yearMade = wordList[5];
        int cityID = convertStringToInt(wordList[6]);
        std::string SkillDescription = wordList[7];

        // Declare the member account
        auto *Skill = new Skill(int skillID, std::vector<std::string> skillList);
        inputStorageSKILLList[SkillID] = Skill;
    }
}

void InputData::inputMemberOwnSKILLFromFile() {
    std::ifstream is {MEMBER_OWN_SKILL_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_OWN_SKILL_PATH<< " for input\n";
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

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        // Declare all variable
        int memberID = convertStringToInt(wordList[0]);
        int SkillID = convertStringToInt(wordList[1]);

        auto member = inputStorageMemberList[memberID];
        auto skill = inputStorageSKILLList[SkillID];

        member->setNewSKILL(skill);
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

void InputData::inputMemberListSKILLFromFile() {
    std::ifstream is {MEMBER_LIST_SKILL_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_OWN_SKILL_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        std::stringstream ss1 {line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        DateTime *availableFromDate = convertStringToDate(wordList[0]);
        DateTime *availableToDate = convertStringToDate(wordList[1]);
        int consumingPointsPerDay = convertStringToInt(wordList[2]);
        double minRenterRatingScore = convertStringToDouble(wordList[3]);
        int ownerMemberID = convertStringToInt(wordList[4]);


        // Get the owner and their Skill
        auto ownerMember = inputStorageMemberList[ownerMemberID];


        // List the Skill
        ownerMember->listSkill(availableFromDate, availableToDate, consumingPointsPerDay, minRenterRatingScore);
    }
}

void InputData::inputMemberRequestSKILLFromFile()
{
    std::ifstream is {MEMBER_REQUEST_SKILL_PATH};
    //check open file
    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_REQUEST_SKILL_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        std::stringstream ss1{line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        DateTime *requestFrom = convertStringToDate(wordList[0]);
        DateTime *requestTo = convertStringToDate(wordList[1]);
        int memberID = convertStringToInt(wordList[2]);
        int SkillID = convertStringToInt(wordList[3]);

        //get member
        auto member = inputStorageMemberList[memberID];
        auto Skill = inputStorageSKILLList[SkillID];

        //store data to request

        auto *request = new Request(requestFrom, requestTo, member);
        member->memberRequestList.push_back(request);
        Skill->skillRequestList.push_back(request);
    }
}

void InputData::inputMemberRentSKILLFromFile() {
    std::ifstream is{MEMBER_RENT_SKILL_PATH};
    //check open file
    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_RENT_SKILL_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        std::stringstream ss1{line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        DateTime *rentFrom = convertStringToDate(wordList[0]);
        DateTime *rentTo = convertStringToDate(wordList[1]);
        int memberID = convertStringToInt(wordList[2]);
        int SkillID = convertStringToInt(wordList[3]);

        //get member and Skill
        auto member = inputStorageMemberList[memberID];
        auto Skill = inputStorageSKILLList[SkillID];

        //store data to request
        auto *memberRent = new MemberRent(rentFrom, rentTo, Skill);
        member->memberRentList.push_back(memberRent);

        auto *SkillRent = new SkillRent(rentFrom, rentTo, member);
        Skill->skillRentList.push_back(SkillRent);
    }
}

void InputData::inputMemberReviewSKILLFromFile()
{
    std::ifstream is{MEMBER_REVIEW_SKILL_PATH};
    //check open file
    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_REVIEW_SKILL_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        std::stringstream ss1{line};

        if (line.empty()) {
            return;
        }

        std::string word;
        std::vector<std::string> wordList;

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        int scoreRating = convertStringToInt(wordList[0]);
        std::string comment = wordList[1];
        int memberID = convertStringToInt(wordList[2]);
        int SkillID = convertStringToInt(wordList[3]);

        //get member and Skill
        auto member = inputStorageMemberList[memberID];
        auto Skill = inputStorageSkillList[SkillID];

        auto *SkillReview = new Review(scoreRating, comment, member);
        Skill->SkillReviewList.push_back(SkillReview);

    }

}

void InputData::inputMemberReviewRenterFromFile() {
    std::ifstream is{MEMBER_REVIEW_RENTER_PATH};

    //check open file
    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_REVIEW_RENTER_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        std::stringstream ss1{line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        int scoreRating = convertStringToInt(wordList[0]);
        std::string comment = wordList[1];
        int memberID = convertStringToInt(wordList[2]);
        int renterID = convertStringToInt(wordList[3]);

        //get member and Skill
        auto renter = inputStorageMemberList[renterID];
        auto owner = inputStorageMemberList[memberID];

        auto* reviewRenter = new Review (scoreRating, comment, owner);
        renter->memberReviewList.push_back(reviewRenter);
    }
}

void InputData::inputSKILLUnratedRenterFromFile()
{
    std::ifstream is {SKILL_UNRATED_RENTER_PATH };
    //check open file
    if (!is.is_open()) {
        std::cerr << "Cannot open " << SKILL_UNRATED_RENTER_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        std::stringstream ss1{line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        DateTime *rentFrom= convertStringToDate(wordList[0]);
        DateTime*rentTo = convertStringToDate(wordList[1]);

        int SkillID = convertStringToInt(wordList[2]);
        int renterID = convertStringToInt(wordList[3]);


        auto member = inputStorageMemberList[renterID];
        auto Skill = inputStorageSKILLList[SkillID];

        auto *unratedRenter = new SkillRent(rentFromDate, rentToDate,member);
        Skill->unratedRenterList.push_back(unratedRenter);
    }
}

void InputData::inputStorageFromFileList() {
    inputMemberListFromFile();
    inputSKILLListFromFile();
    inputAdminListFromFile();
    inputMemberOwnSKILLFromFile();
    inputMemberListSKILLFromFile();
    inputMemberRequestSKILLFromFile();
    inputMemberRentSkillFromFile();
    inputMemberReviewSKILLFromFile();
    inputMemberReviewRenterFromFile();
    inputSkillUnratedRenterFromFile();
}

void InputData::inputStorageLoadDataToSystem(System *system) {
    inputStorageLoadMemberListToSystem(system);
    inputStorageLoadSKILLListToSystem(system);
    inputStorageLoadAdminListToSystem(system);
}


