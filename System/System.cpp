#include "System.h"
#include "../DateTime/DateTime.h"
#include "../Skill/Skill.h"
#include "../Member/Member.h"
#include "../Member/BlockedMember.h"
#include "../Admin/Admin.h"
#include "../SkillRent/SkillRent.h"
#include "../MemberRent/MemberRent.h"
#include "../Request/Request.h"
#include "../Rating/Rating.h"
#include "../Validation/Validation.h"
#include "../Middleware/Middleware.h"
#include "../StorageManager/Loader.h"
#include "../StorageManager/Saver.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
using std::cout;

// Clear system info when initialized
System::System()
{
    this->currentMember = nullptr;
    this->systemSkillList.clear();
    this->systemMemberList.clear();
    this->memberSuitableSkillList.clear();
}

// Clear screen
void System::clearScreen()
{
    std::cout << "Loading..."
              << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(750));
    for (int n = 0; n < 10; n++)
    {
        cout << "\n\n\n\n\n\n\n\n\n\n";
    }
}

// Set current member when logging in
void System::setCurrentMember(Member *member)
{
    currentMember = member;
    memberSuitableSkillList.clear();
}

// Add member when registering
void System::addMember(Member *member)
{
    systemMemberList.push_back(member);
}

// Add skill after entering skill info
void System::addSkill(Skill *skill)
{
    systemSkillList.push_back(skill);
}

// Add new admin
void System::addAdmin(Admin *admin)
{
    systemAdminList.push_back(admin);
}

// Add new entry in blocked member list
void System::addBlockedMember(BlockedMember *blockedMember)
{
    systemBlockedMemberList.push_back(blockedMember);
}

// Function to show suitable supporter list
bool System::memberSearchSuitableSkillList(DateTime *startTime, DateTime *endTime, int cityID)
{
    memberSuitableSkillList.clear();
    suitableSkillsList.clear();

    for (auto &request : currentMember->memberRequestList)
    {
        if (*request->requestTo < *startTime || *endTime < *request->requestFrom)
        {
            continue; // No overlap
        }
        return false; // Overlap detected
    }

    for (auto &skill : systemSkillList)
    {
        if (skill->skillOwner == currentMember)
        {
            continue; // Skip current member's skills
        }

        if (isSuitableSkill(startTime, endTime, cityID - 1, skill))
        {
            suitableSkillsList.push_back(skill);                  // Add skill
            memberSuitableSkillList.push_back(skill->skillOwner); // Add owner
        }
    }

    if (suitableSkillsList.empty())
    {
        std::cout << "\nNo suitable supporters found.\n";
        return false;
    }

    std::cout << "\nThe list of suitable supporters:\n";
    for (size_t i = 0; i < suitableSkillsList.size(); ++i)
    {
        Skill *skill = suitableSkillsList[i];
        Member *supporter = memberSuitableSkillList[i];

        std::cout << i + 1 << ". Supporter's Name: " << supporter->get_name()
                  << ", Skill(s): " << skill->getListOfSkill() << "\n";
    }
    return true;
}

// Function to check if the supporter is suitable to the member
bool System::isSuitableSkill(DateTime *startTime, DateTime *endTime, int cityID, Skill *skill) {
    // Check if the skill belongs to the current member
    if (skill->skillOwner == currentMember) {
        std::cout << "Cannot request your own skill.\n";
        return false;
    }

    // Check if skill availability times are defined
    if (skill->availableFrom == nullptr || skill->availableTo == nullptr) {
        return false;
    }

    // Check for minimum required host rating
    auto [avgSkillRating, avgSupporterRating, avgHostRating] = skill->skillOwner->getRatingScore();
    if (avgHostRating < skill->minHostRating) {
        std::cout << "Your host rating is below the minimum required for this skill.\n";
        return false;
    }

    // Check if the requested time is within the skill's available time
    if (*startTime < *skill->availableFrom || *endTime > *skill->availableTo) {
        std::cout << "Requested time is outside the skill's available time.\n";
        return false;
    }

    // Check for sufficient credit points
    double timeDifferenceInHours = static_cast<double>(*endTime - *startTime) / 60.0;
    if (timeDifferenceInHours * skill->creditCostPerHour > currentMember->creditPoints) {
        std::cout << "Insufficient credit points for the requested time period.\n";
        return false;
    }

    // Check if the skill is available in the selected city
    if (cityID != skill->cityID) {
        std::cout << "Skill is not available in the selected city.\n";
        return false;
    }

    // Check for overlapping rented time slots
    for (auto &rent : skill->skillRentList) {
        if (!(*endTime <= *rent->rentFrom || *startTime >= *rent->rentTo)) {
            std::cout << "Requested time slot overlaps with an existing rent: "
                      << rent->rentFrom->toString() << " to " << rent->rentTo->toString() << "\n";
            return false;
        }
    }

    // Skill is suitable for the request
    return true;
}

bool System::memberViewSkillReviewList(int skillID, DateTime *sD, DateTime *eD)
{
    if (skillID >= suitableSkillsList.size())
    {
        return false;
    }

    auto skill = suitableSkillsList[skillID];
    std::cout << skill->skillOwner->viewHostRateSupporter();
    return true;
}

// Function to send request to the supporter
bool System::memberSendRequest(DateTime *startTime, DateTime *endTime, int SkillID)
{
    if (SkillID >= suitableSkillsList.size())
    {
        return false;
    }

    for (auto &memberRequest : currentMember->memberRequestList)
    {
        if (*(memberRequest->requestTo) < *startTime || *endTime < *(memberRequest->requestFrom))
        {
            continue;
        }

        return false;
    }

    // Create an object for the request
    auto *request = new Request(startTime, endTime, currentMember);

    // Add the request to the request list of the skill
    suitableSkillsList[SkillID]->addRequestToSkillRequestList(request);

    // Add the request to the request list of the member
    currentMember->addToRequestList(request);

    return true;
}

// Function to accept host request
bool System::memberAcceptRequest(int requestID)
{

    // Current member accept the request
    currentMember->acceptRequest(requestID);

    return true;
}

// Function to view rent list
bool System::memberViewRentList()
{

    currentMember->showCurrentSkillRent();
    return true;
}

// Function for current member to turnback the skill
bool System::completeRequest(int completedSkillID)
{
    if (currentMember == nullptr)
    {
        std::cerr << "No current member set\n";
        return false;
    }

    return currentMember->completeRequest(completedSkillID);
}

// Function to rate host
bool System::memberRateHost(Member *host)
{
    if (host == nullptr)
    {
        std::cerr << "Invalid host. Cannot rate.\n";
        return false;
    }

    std::cout << "Rating host: " << host->get_name() << "\n";

    int hostRating;
    std::string comment;
    std::cout << "\nRate this member from 1 to 5\n";
    hostRating = choiceFunc(1, 5); // Ensure choiceFunc limits choice between 1 and 5

    std::cout << "\nEnter a comment about this member: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, comment);

    return currentMember->rateHost(host, hostRating, comment);
}

// Function to rate skill
bool System::memberRateSupporterAndSkill(Member *supporter)
{
    if (supporter == nullptr)
    {
        std::cerr << "Invalid supporter. Cannot rate.\n";
        return false;
    }

    int skillRating, supporterRating;
    std::string comment;

    // Prompt for rating the skill
    std::cout << "\n--- Rate the Skill from 1 to 5 ---\n";
    skillRating = choiceFunc(1, 5);

    // Prompt for rating the supporter
    std::cout << "\n--- Rate the Supporter from 1 to 5 ---\n";
    supporterRating = choiceFunc(1, 5);

    // Prompt for a comment
    std::cout << "\nEnter a comment about this skill/supporter: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, comment);

    // Pass the ratings and comment to the Member's rating method
    return currentMember->rateSupporterAndSkill(supporter, skillRating, supporterRating, comment);
}

// Function for entering choice
int System::choiceFunc(int a, int b)
{
    std::string temp;
    int tempInt;
    int err;

    do
    {
        err = 0;
        std::cout << "\nEnter your choice (Number only): ";
        std::cin >> temp;
        if (!isNumber(temp))
        {
            std::cout << "Error. Please try again!\n";
            err++;
            continue;
        }
        tempInt = convertStringToInt(temp);
        if (tempInt < a || tempInt > b)
        {
            std::cout << "Error. Please try again!\n";
            err++;
            continue;
        }
    } while (err);

    return tempInt;
}

bool System::getUserYesNoResponse()
{
    std::string response;
    while (true)
    {
        std::getline(std::cin, response);

        // Convert response to lowercase for easier comparison
        std::transform(response.begin(), response.end(), response.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });

        if (response == "yes" || response == "y")
        {
            return true;
        }
        else if (response == "no" || response == "n")
        {
            return false;
        }
        else
        {
            std::cout << "Invalid input. Please enter 'yes' or 'no': ";
        }
    }
}

// Feature 10: A member can block another member from viewing his/her information or requesting support.
bool System::blockMemberInteraction(Member *requestingMember)
{
    if (requestingMember == nullptr)
    {
        std::cerr << "Invalid requesting member.\n";
        return false;
    }

    std::cout << "Select a member to block:\n";
    Member *targetMember = chooseMember();

    if (targetMember == nullptr)
    {
        std::cout << "Returning to the previous menu...\n";
        return false;
    }

    if (requestingMember == targetMember)
    {
        std::cerr << "Cannot block oneself.\n";
        return false;
    }

    bool blockView = false, blockRequestSupport = false;

    // Check if already blocked from viewing and ask if not
    if (!requestingMember->isBlockedForViewing(targetMember))
    {
        std::cout << "Do you want to block this member from viewing your information? (yes/no): ";
        blockView = getUserYesNoResponse();
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Check if already blocked from requesting support and ask if not
    if (!requestingMember->isBlockedForRequesting(targetMember))
    {
        std::cout << "Do you want to block this member from requesting your support? (yes/no): ";
        blockRequestSupport = getUserYesNoResponse();
    }

    // Find if there's already a block between the two members
    BlockedMember *existingBlock = nullptr;
    for (auto &block : systemBlockedMemberList)
    {
        if (block->getBlockerID() == requestingMember->memberID && block->getBlockedID() == targetMember->memberID)
        {
            existingBlock = block;
            break;
        }
    }

    if (existingBlock)
    {
        // Update existing block
        existingBlock->updateBlockSettings(blockView, blockRequestSupport);
        std::cout << "Block settings updated successfully.\n";
    }
    else
    {
        // Attempt to create a new block
        bool success = requestingMember->blockMember(targetMember, blockView, blockRequestSupport);
        if (success)
        {
            auto *newBlock = new BlockedMember(requestingMember->memberID, targetMember->memberID, blockView, blockRequestSupport);
            addBlockedMember(newBlock);
            std::cout << "New block settings added successfully.\n";
        }
        else
        {
            std::cerr << "Failed to update block settings.\n";
            return false;
        }
    }

    std::cout << "\n1. Return to member Menu\n2. Block another member\n";
    int choice = choiceFunc(1, 2);

    if (choice == 1)
    {
        memberMenu();
    }
    else
    {
        return blockMemberInteraction(currentMember); // Recursively call the function to block another member
    }

    return true;
}

// main menu
void System::mainMenu()
{
    std ::cout << "\n\n\n    *    \"TIME BANK\" APP    *\n\n\n";
    cout << "\t--> MAIN MENU <--\n\n"
         << "\t--> 1. Guest\n"
         << "\t--> 2. Member\n"
         << "\t--> 3. Admin\n"
         << "\t--> 4. Exit\n\n";
    switch (choiceFunc(1, 4))
    {
    case 1:
        guestMenu();
        break;

    case 2:
        memberLoginMenu();
        break;

    case 3:
        if (adminLogin())
        {
            std::cout << "Login successful\n";
            adminMenu();
        }
        else
        {
            std::cout << "Login failed\n";
            clearScreen();

            mainMenu();
        }
        break;

    case 4:
        systemExit();
        break;
    }
}
// end main

// Guest's Feature
void System::guestMenu()
{
    clearScreen();
    std ::cout << "\n\n\n    *    \"TIME BANK\" APP    *\n\n\n";
    std::cout << "\t--> GUEST MENU <--\n\n"
              << "--> 1.\tView Supporter\n"
              << "--> 2.\tRegister for an account\n"
              << "--> 3.\tBack to main menu\n";
    switch (choiceFunc(1, 3))
    {
    case 1:
        // Print supporter list
        guestViewSupporterList();
        break;
    case 2:
        clearScreen();
        // Register new member
        if (memberRegister())
        {
            std::cout << "Registration successful\n";
            memberLoginMenu();
        }
        break;
    case 3:
        // Back to main menu
        clearScreen();
        mainMenu();
        break;
    }
}

// Feature 2: A non-member can view all supporters’ details (but not their reviews)
void System::guestViewSupporterList()
{
    int choice;
    while (true)
    {
        clearScreen();
        std ::cout << "\n\n\n    *    \"TIME BANK\" APP    *\n\n\n";
        std::cout << "\n\t\t--> SUPPORTER LIST <-- \n"
                  << "ID    Information \n\n";
        // Print supporter list
        for (Member *mem : systemMemberList)
        {
            std::cout << std::left << std::to_string(mem->memberID) + ". ";
            std::cout << mem->guestViewSupporterInfo() << "\n";
        }
        std::cout << "\n"
                  << systemMemberList.size() + 1 << ". Back to Guest Menu\n";
        choice = choiceFunc(1, systemMemberList.size() + 1);
        if (choice == systemMemberList.size() + 1)
        {
            // Go back to guest menu
            guestMenu(); // Exit the loop to return to the guest menu
        }
        else
        {
            // View supporter info
            cout << "\nYou have chosen supporter number " << choice;
            std::cout << "\n"
                      << systemMemberList[choice - 1]->viewSupporterInfoInDetail();
            std::cout << "\n1. Back to supporter list\n";
            choiceFunc(1, 1); // Wait for user input
            // The loop will then start again, showing the supporter list
        }
    }
}

// end guest

// admin (feature 5)
bool System::adminLogin()
{
    clearScreen();
    std ::cout << "\n\n\n    *    \"TIME BANK\" APP    *\n\n\n";
    std::string username, password;
    // Enter admin username and password
    std::cout << "\t--> ADMIN LOGIN <--\n\n"
              << "Username: ";
    std::cin.ignore();
    std::getline(std::cin, username);
    std::cout << "\nPassword: ";
    std::getline(std::cin, password);

    for (auto &admin : systemAdminList)
    {

        // Check whether the password and username is valid
        if (username != admin->username || password != admin->password)
        {
            continue;
        }

        return true;
    }

    return false;
}

void System::adminMenu()
{
    clearScreen();
    std ::cout << "\n\n\n    *    \"TIME BANK\" APP    *\n\n\n";
    std::cout << "\t--> ADMIN MENU <--\n\n"
              << "--> 1.\tChange user's password \n"
              << "--> 2.\tSign out";
    switch (choiceFunc(1, 2))
    {
    case 1:
        // View member list
        clearScreen();
        adminChangePassword();
        break;

    case 2:
        // Back to main menu
        clearScreen();
        mainMenu();
        break;
    }
}

void System::displayMemberList()
{
    int index = 1;
    for (auto &member : systemMemberList)
    {
        if (member != currentMember)
        { // Skip the current member
            std::cout << index << ". " << member->username << "\n";
            index++;
        }
    }
    std::cout << index << ". Back to main menu\n";
}

Member *System::chooseMember()
{
    displayMemberList();
    int choice = choiceFunc(1, systemMemberList.size() + 1); // Adjusted the size

    if (choice == systemMemberList.size() + 1)
    {
        return nullptr; // Indicates the user chose to go back
    }

    // Correctly map the choice to the member in the list, accounting for the skipped current member
    int actualIndex = 0;
    for (auto &member : systemMemberList)
    {
        if (member != currentMember)
        {
            actualIndex++;
            if (actualIndex == choice)
            {
                return member;
            }
        }
    }
    return nullptr;
}

// Feature 3: An admin can login with predefined username and password, and can reset password for any member.
bool System::adminChangePassword()
{
    Member *mem = chooseMember();
    if (mem == nullptr)
    {
        std::cout << "Returning to admin menu...\n";
        adminMenu();
        return false;
    }

    std::string newPassword, confirmPassword;
    do
    {
        std::cout << "Enter new password for " << mem->username << ": ";
        std::cin >> newPassword;
        std::cout << "Confirm new password: ";
        std::cin >> confirmPassword;
    } while (newPassword != confirmPassword || !validate.password(newPassword));

    mem->setPassword(newPassword);
    std::cout << "Password for member " << mem->username << " has been reset.\n";

    OutputData outputData;
    outputData.updateMemInfotoFile(systemMemberList);

    std::cout << "\n1. Return to Admin Menu\n2. Change another password\n";
    int choice = choiceFunc(1, 2);

    if (choice == 1)
    {
        adminMenu();
    }
    else
    {
        return adminChangePassword(); // Recursively call the function to change another password
    }

    return true;
}

// member
// member login (feature 4)
void System::memberLoginMenu()
{
    clearScreen();
    std ::cout << "\n\n\n    *    \"TIME BANK\" APP    *\n\n\n";
    std::cout << "\t--> MEMBER LOGIN MENU <--\n\n"
              << "--> 1.\tLogin\n"
              << "--> 2.\tBack to main menu\n";
    switch (choiceFunc(1, 2))
    {
    case 1:
        // Login to a member account
        if (memberLogin())
        {
            std::cout << "Login successful\n";
            memberMenu();
        }
        else
        {
            std::cout << "Wrong username or password\n";
            mainMenu();
        }
        break;
    case 2:
        clearScreen();
        mainMenu();
        break;
    }
}

bool System::memberLogin()
{
    std::string username, password;
    // Enter member account info
    std::cout << "\n\t--> MEMBER LOGIN <--\n\n"
              << "Username: ";
    std::cin >> username;
    std::cout << "\nPassword: ";
    std::cin >> password;
    // Check if username and password is correct
    for (Member *mem : systemMemberList)
    {
        if (username == mem->username && password == mem->password)
        {
            setCurrentMember(mem);
            return true;
        }
    }
    return false;
}
// end login

// Feature 1: A non-member can register to become a member (information is recorded)
bool System::memberRegister()
{
    std::string username, password, firstName, lastName, phoneNumber, email, address;

    std ::cout << "\n\n\n    *    \"TIME BANK\" APP    *\n\n\n";

    // Enter member information
    std::cout << "\t--> MEMBER REGISTRATION <--\n\n";
    do
    {
        std::cout << "Enter a username: ";
        std::cin.ignore();
        std::getline(std::cin, username);
    } while (!validate.username(username, this));

    do
    {
        std::cout << "\nEnter a password: ";
        std::getline(std::cin, password);
    } while (!validate.password(password));

    do
    {
        std::cout << "\nEnter your first name: ";
        std::getline(std::cin, firstName);
    } while (!validate.name(firstName));

    do
    {
        std::cout << "\nEnter your last name: ";
        std::getline(std::cin, lastName);
    } while (!validate.name(lastName));

    do
    {
        std::cout << "\nEnter your phone number: ";
        std::getline(std::cin, phoneNumber);
    } while (!validate.phoneNumber(phoneNumber, this));

    do
    {
        std::cout << "\nEnter your email: ";
        std::getline(std::cin, email);
    } while (!validate.email(email, this));

    do
    {
        std::cout << "\nEnter your address: ";
        std::getline(std::cin, address);
    } while (!validate.address(address, this));

    auto *newMember = new Member(systemMemberList.size() + 1, username, password, firstName, lastName, phoneNumber, email, address);
    addMember(newMember);
    return true;
}
// End register

// Menu for member
void System::memberMenu()
{
    clearScreen();
    std ::cout << "\n\n\n    *    \"TIME BANK\" APP    *\n\n\n";
    std::cout << "\t--> MEMBER MENU <--\n\n"
              << "--> 1.\tView your information\n"
              << "--> 2.\tView other members' information\n"
              << "--> 3.\tSkill supporting menu\n"
              << "--> 4.\tSearch for available supporters\n"
              << "--> 5.\tView skill request list (supporter only)\n"
              << "--> 6.\tView currently supported skill (supporter only)\n"
              << "--> 7.\tView completed session list\n"
              << "--> 8.\tBlock member\n"
              << "--> 9.\tTop up credit points\n"
              << "--> 10.\tSign out\n";
    switch (choiceFunc(1, 10))
    {
    case 1:
        // View member info
        currentMember->showMemInfo();
        std::cout << "1. Back to member menu";
        choiceFunc(1, 1);
        memberMenu();
        break;

    case 2:
        viewMemberInformation();
        break;

    case 3:
        // Go to member list
        memberListMenu();
        break;

    case 4:
        // Go to search suitable supporter
        memberSearchSuitableSkillMenu();
        break;

    case 5:
        // View skill requests
        memberViewSkillRequestListMenu();
        break;

    case 6:
        // View currently supported skill
        memberViewRentedSkill();
        break;

    case 7:
        completedSessionListMenu();
        break;
    case 8:
        blockMemberInteraction(currentMember);
        break;
    case 9:
        topUpCreditPoints();
        break;
    case 10:
        // Sign out and go back to main menu
        setCurrentMember(nullptr);
        clearScreen();
        mainMenu();
        break;
    }
}
// end menu

void System::topUpCreditPoints()
{
    int amount;
    std::string password;

    std::cout << "Enter the amount to top up (in credit points): ";
    std::cin >> amount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

    // Validate the amount
    if (amount <= 0)
    {
        std::cerr << "Invalid amount. Please enter a positive number.\n";
        return;
    }

    std::cout << "Enter your password for authorization: ";
    std::getline(std::cin, password);

    // Call the top-up method on the current member
    if (currentMember->topUpCreditPoints(amount, password))
    {
        OutputData od;
        od.updateMemInfotoFile(systemMemberList);
        std::cout << "Credit points successfully topped up.\n";
    }
    else
    {
        std::cerr << "Failed to top up credit points. Please check the amount and your password.\n";
    }

    std::cout << "\n1. Return to member Menu\n";
    int choice = choiceFunc(1, 1);

    if (choice == 1)
    {
        memberMenu();
    }
}

void System::viewMemberInformation()
{
    while (true)
    {
        std::cout << "Select a member to view their information:\n";
        Member *selectedMember = chooseMember();

        if (selectedMember == nullptr)
        {
            std::cout << "Returning to the previous menu...\n";
            memberMenu();
            return;
        }

        if (selectedMember->isBlockedForViewing(currentMember))
        {
            std::cout << "You are blocked from viewing this member's information.\n";
        }
        else
        {
            selectedMember->showMemInfo(selectedMember);
        }

        std::cout << "\nChoose an option:\n";
        std::cout << "1. Back to Member Menu\n";
        std::cout << "2. View another Supporter's Information\n";
        int choice = choiceFunc(1, 2);

        if (choice == 1)
        {
            memberMenu(); // go back to the member Menu
            break;
        }
        // If choice is 2, the loop continues for viewing another supporter's information
    }
}

// list menu (feature 5)
void System::memberListMenu()
{
    std ::cout << "\n\n\n    *    \"TIME BANK\" APP    *\n\n\n";
    cout << "\t--> LIST MENU <--\n\n";
    // Check if member has an owned skill
    if (currentMember->ownedSkill != nullptr)
    {
        // Check if member's skill is listed
        if (currentMember->ownedSkill->isListed)
        {
            // Print skill info
            std::cout << "Currently listed skill:\n"
                      << currentMember->ownedSkill->getSkillInfo() << "\n";
            if (currentMember->ownedSkill->skillRequestList.empty())
            {
                std::cout << "\n--> 1.\tUnlist skill\n"
                          << "--> 2.\tBack to member menu\n";
                switch (choiceFunc(1, 2))
                {
                case 1:
                    // Unlist skill
                    memberUnlistSkill();
                    std::cout << "1. Back to list menu\n";
                    choiceFunc(1, 1);
                    memberListMenu();
                    break;
                case 2:
                    // Back to member menu
                    memberMenu();
                    break;
                }
            }
            else
            {
                std::cout << "1. Back to member menu\n";
                int choice = choiceFunc(1, 1);
                memberMenu();
            }
        }
        else
        {
            // List skill if skill isn't listed
            std::cout << "Your skill isn't listed\n\n"
                      << "--> 1. List your skill\n"
                      << "--> 2. Back to member menu\n";
            switch (choiceFunc(1, 2))
            {
            case 1:
                memberListSkill();
                std::cout << "\nSuccessfully listed skill\n\n";
                memberListMenu();
                break;
            case 2:
                memberMenu();
                break;
            }
        }
    }
    else
    {
        // Enter skill info if member has no owned skill
        std ::cout << "You haven't had any skills on our system yet!!\n";
        cout << "1. Enter your skill info\n";
        cout << "2. Back to member menu\n";
        switch (choiceFunc(1, 2))
        {
        case 1:
            std::cout << "Please enter your SKILL information \n\n";
            memberEnterSkillInfo();
            memberListMenu();
            break;
        case 2:
            memberMenu();
            break;
        }
    }
}

bool System::memberEnterSkillInfo()
{
    std::vector<std::string> skillList;
    std::string skillName;
    int cityID;

    std::cout << "Enter the skill information: \n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true)
    {
        std::cout << " - Enter a skill (type 'done' to finish): ";

        std::getline(std::cin, skillName);

        if (skillName.empty())
        {
            std::cout << "Skill name cannot be empty. Please enter a skill.\n";
        }
        else if (skillName == "done")
        {
            if (skillList.empty())
            {
                std::cout << "Please enter at least one skill.\n";
            }
            else
            {
                break;
            }
        }
        else
        {
            skillList.push_back(skillName);
        }
    }

    std::cout << "\nChoose the city your skill is in: \n\n"
              << "--> 1.\tHanoi\n"
              << "--> 2.\tSaigon\n";
    cityID = choiceFunc(1, 2) - 1;
    auto *newSkill = new Skill(systemSkillList.size() + 1, skillList, cityID);
    // Add the skill into the skill list
    addSkill(newSkill);
    // Set owned skill to the newly added skill
    currentMember->setNewSkill(newSkill);
    return true;
}

// Feature 5: A member can list himself/herself available to be booked (including skills can be performed, consuming points per hour, with/without minimum required host-rating score), and unlist if wanted.
bool System::memberListSkill()
{
    std::string startTime, endTime, conPoints;
    // Enter listing info (with validation)
    std::cout << "\nEnter listing information: \n";
    int conPointsNum;
    std::optional<double> minHostRatingOpt = std::nullopt;

    do
    {
        do
        {
            std::cout << " - Start time(hh:mm): ";
            std::cin >> startTime;
        } while (!validate.time(startTime));
        do
        {
            std::cout << "- End date(hh:mm): ";
            std::cin >> endTime;
        } while (!validate.time(endTime));
    } while (!validate.timeLater(startTime, endTime));

    DateTime *sT = convertStringToDateTime(startTime);
    DateTime *eT = convertStringToDateTime(endTime);

    do
    {
        std::cout << "- Consuming points per hour: ";
        std::cin >> conPoints;
        if (!isNumber(conPoints))
        {
            std::cout << "Your option is not a number. Please try again.\n";
        }
    } while (!isNumber(conPoints));
    conPointsNum = convertStringToInt(conPoints);

    // Ask if the member wants to set a minimum host rating
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Do you want to set a minimum required host-rating score? (yes/no): ";
    if (getUserYesNoResponse())
    {
        std::string minHostRating;
        double minHostRatingNum;
        do
        {
            std::cout << "- Minimum Host Rating (1.0-5.0): ";
            std::cin >> minHostRating;
        } while (!validate.rating(minHostRating, minHostRatingNum));
        minHostRatingOpt = minHostRatingNum;
    }

    // Add listing info to data
    currentMember->listSkill(sT, eT, conPointsNum, minHostRatingOpt);

    // Clean up dynamically allocated DateTime objects
    delete sT;
    delete eT;

    return true;
}

// Feature 5: A member can list himself/herself available to be booked (including skills can be performed, consuming points per hour, with/without minimum required host-rating score), and unlist if wanted.
bool System::memberUnlistSkill()
{
    if (currentMember->unlistSkill())
    {
        std::cout << "\nSuccessfully unlist skill \n\n";
        return true;
    }
    else
    {
        std::cout << "Failed to unlist skill. Please accept member request before unlisting the skill.\n";
        return false;
    }
}

// Feature 6: A member can search for all available suitable supporters for a specified city (suitable with his current credit points and rating score).
void System::memberSearchSuitableSkillMenu()
{
    std::string start, end;
    int cityID;
    // Enter search information
    std::cout << "\nSearch for a suitable skill: \n\n";

    std::cout << "\nChoose the city you want to search for: \n\n"
              << "--> 1. Hanoi\n"
              << "--> 2. Saigon\n";
    cityID = choiceFunc(1, 2);
    do
    {
        do
        {
            std::cout << "Enter start time (hh:mm): ";
            std::cin >> start;
        } while (!validate.time(start));

        do
        {
            std::cout << "\n\nEnter end time (hh:mm): ";
            std::cin >> end;
        } while (!validate.time(end));
    } while (!validate.timeLater(start, end));
    DateTime *startTime = convertStringToDateTime(start), *endTime = convertStringToDateTime(end);
    // Go to suitable skill list
    memberSuitableSkillMenu(startTime, endTime, cityID);
}

void System::memberSuitableSkillMenu(DateTime *sD, DateTime *eD, int cityID)
{
    memberSuitableSkillList.clear();

    if (!memberSearchSuitableSkillList(sD, eD, cityID))
    {
        std::cout << "No suitable skills found. Please try different criteria.\n";
        std::cout << "1. Retry with different criteria\n";
        std::cout << "2. Return to member menu\n";
        int choice = choiceFunc(1, 2);
        if (choice == 1)
        {
            memberSearchSuitableSkillMenu(); // Retry
        }
        else
        {
            memberMenu(); // Return to member menu
        }
        return;
    }

    std::cout << memberSuitableSkillList.size() + 1 << ". Back to member menu\n";
    int choice = choiceFunc(1, memberSuitableSkillList.size() + 1);
    if (choice == (memberSuitableSkillList.size() + 1))
    {
        memberMenu();
    }
    else
    {
        Member *potentialSupporter = memberSuitableSkillList[choice - 1];
        if (potentialSupporter->isBlockedForRequesting(currentMember))
        {
            std::cout << "\nYou are blocked from requesting support from this member.\n";
            std::cout << "\n--> 1.\tBack to skill list\n";
            choiceFunc(1, 1);
            memberSuitableSkillMenu(sD, eD, cityID);
            return;
        }

        std::cout << "\n\n--> 1.\tRequest supporter\n\n"
                  << "--> 2.\tView supporter reviews\n\n"
                  << "--> 3.\tBack to skill list\n";
        switch (choiceFunc(1, 3))
        {
        case 1:
            memberSendRequest(sD, eD, choice - 1);
            std::cout << "\n\tRequest sent\n";
            memberMenu();
            break;
        case 2:
            memberViewSkillReviewList(choice - 1, sD, eD);
            std::cout << "\n\n--> 1.\tRequest supporter\n\n"
                      << "--> 2.\tBack to skill list\n";
            switch (choiceFunc(1, 2))
            {
            case 1:
                memberSendRequest(sD, eD, choice - 1);
                std::cout << "\tRequest sent\n";
                memberMenu();
                break;
            case 2:
                memberSuitableSkillMenu(sD, eD, cityID);
                break;
            }
            break;
        case 3:
            memberSuitableSkillMenu(sD, eD, cityID);
            break;
        }
    }
}

void System::suitableSkillMenu(int choice, DateTime *sD, DateTime *eD, int cityID)
{
    std::cout << memberSuitableSkillList[choice - 1]->showMemInfo() << "\n";
    std::cout << "1. View reviews\n"
              << "2. Request supporter\n"
              << "3. Back to skill list\n";
    switch (choiceFunc(1, 3))
    {
    case 1:
        std::cout << memberSuitableSkillList[choice - 1]->viewHostRateSupporter();
        std::cout << "1. Back to skill menu\n";
        choiceFunc(1, 1);
        suitableSkillMenu(choice, sD, eD, cityID);
    case 2:
        if (memberSendRequest(sD, eD, suitableSkillsList[choice - 1]->skillID))
        {
            std::cout << "Request sent\n";
        }
        else
        {
            std::cout << "Failed to send request\n";
        }
        memberMenu();
        break;
    case 3:
        memberSuitableSkillMenu(sD, eD, cityID);
        break;
    }
}
// end search suitable skill

// Feature 8: A member can view all requests to his listed skills.
void System::memberViewSkillRequestListMenu()
{
    // Check if member has an owned skill
    if (currentMember->ownedSkill == nullptr)
    {
        std::cout << "You haven't entered your skill info\n"
                  << "1. Back to member menu\n";
        choiceFunc(1, 1);
        memberMenu();
        // Check if owned skill is listed
    }
    else if (!currentMember->ownedSkill->isListed)
    {
        std::cout << "You haven't listed your skill yet!\n"
                  << "1. Back to member menu\n";
        choiceFunc(1, 1);
        memberMenu();
    }

    std::cout << "\nChoose a request to view in detail: \n\n";

    // View skill request list
    if (currentMember->showListOfRequest())
    {
        std::cout << "\n"
                  << currentMember->ownedSkill->skillRequestList.size() + 1 << ". Back to member menu\n\n";
        int choice = choiceFunc(1, currentMember->ownedSkill->skillRequestList.size() + 1);
        if (choice == currentMember->ownedSkill->skillRequestList.size() + 1)
        {
            // Back to member menu
            memberMenu();
        }
        else
        {
            std::cout << "\n\n-->\t1. Accept the request\n"
                      << "-->\t2. View the reviews of this member\n"
                      << "-->\t3. Back to menu\n";
            switch (choiceFunc(1, 3))
            {
            case 1:
            {
                // Accept skill request
                if (currentMember->acceptRequest(choice - 1))
                {
                    std::cout << "Request accepted successfully\n";
                }
                else
                {
                    std::cout << "Failed to accept request\n";
                }
                memberMenu();
                break;
            }
            case 2:
            {
                // View member reviews
                memberViewRenterReviewList(choice);
                break;
            }
            case 3:
            {
                // Back to member menu
                memberMenu();
                break;
            }
            }
        }
    }
    else
    {
        // Back to member menu
        std::cout << "1. Back to member menu\n";
        choiceFunc(1, 1);
        memberMenu();
    }
}

// end view skill request

// view currently supported skill (complete the request)
void System::memberViewRentedSkill()
{
    // Check if current member doesn't support a host
    if (currentMember->memberRentList.empty())
    {
        std::cout << "\nYou haven't supported anyone yet\n\n"
                  << "1. Back to member menu\n";
        choiceFunc(1, 1);
        memberMenu();
    }
    else
    {
        // View rent list
        currentMember->showCurrentSkillRent();
        std::cout << currentMember->memberRentList.size() + 1 << ". Back to member menu\n";
        int choice = choiceFunc(1, currentMember->memberRentList.size() + 1);
        if (choice == currentMember->memberRentList.size() + 1)
        {
            // Back to member menu
            memberMenu();
        }
        else
        {
            // Check for renting information
            auto rentSkill = currentMember->memberRentList[choice - 1]->rentSkill;
            std::cout << "\n"
                      << rentSkill->getSkillInfo() << "\n"
                      << "\n-->\t1. Complete the request\n"
                      << "\n-->\t2. Back to member menu\n";
            switch (choiceFunc(1, 2))
            {
            case 1:
            {
                // Complete the request
                if (currentMember->completeRequest(choice - 1))
                {
                    std::cout << "\nYou have completed the request\n"
                              << "Thank you for your contribution\n"
                              << "SEE YOU AGAIN!!!\n\n";

                    // Rate the host
                    Member *host = rentSkill->skillRentList[choice - 1]->rentedByMember; // Retrieve the host
                    memberRateHost(host);
                    std::cout << "Host reviewed.\n";
                }
                else
                {
                    std::cout << "\nProcess unsuccessful\n";
                }
                std::cout << "-->\t1. Back to member menu\n";
                choiceFunc(1, 1);
                memberMenu();
                break;
            }
            case 2:
            {
                // Back to member menu
                memberMenu();
                break;
            }
            }
        }
    }
} // end view currently rented skill

void System::completedSessionListMenu()
{
    std::cout << "Choose 1 for supporter, choose 2 for host\n";
    std::cout << "1. Supporter\n";
    std::cout << "2. Host\n";
    std::cout << "3. Back to member menu\n";
    int ch = choiceFunc(1, 3);
    if (ch == 1)
    {
        for (auto &skill : systemMemberList)
        {
            for (auto &session : skill->completedSessionList)
            {
                // Check if the current member is the supporter in the session
                if (session->supportedByMember->memberID == currentMember->memberID)
                {
                    completedSessionList.push_back(session);
                }
            }
        }

        if (completedSessionList.empty())
        {
            std::cout << "\nYou have no completed sessions as supporter.\n\n";
            std::cout << "1. Back to member menu\n";
            choiceFunc(1, 1);
            memberMenu();
            return;
        }

        // Display the relevant completed sessions
        std::cout << "\nCompleted Session List: \n\n";
        for (size_t i = 0; i < completedSessionList.size(); i++)
        {
            auto session = completedSessionList[i];
            std::cout << "--> " << i + 1 << ". "
                      << "From: " << session->rentFrom->toString()
                      << ", To: " << session->rentTo->toString()
                      << ", Supported By: " << session->supportedByMember->get_name()
                      << ", Hosted By: " << session->rentedByMember->get_name() << "\n";
        }
        std::cout << completedSessionList.size() + 1 << ". Back to member menu (You can only choose this one!!!)\n";
        choiceFunc(completedSessionList.size() + 1, completedSessionList.size() + 1);
        memberMenu();
        return;
    }
    else if (ch == 2)
    {
        // Search through all skills for completed sessions where the current member is the host
        for (auto &skill : systemMemberList)
        {
            for (auto &session : skill->completedSessionList)
            {
                // Check if the current member is the host in the session
                if (session->rentedByMember->memberID == currentMember->memberID)
                {
                    completedSessionList.push_back(session);
                }
            }
        }

        // Check if there are relevant sessions
        if (completedSessionList.empty())
        {
            std::cout << "\nYou have no completed sessions as host.\n\n";
            std::cout << "1. Back to member menu\n";
            choiceFunc(1, 1);
            memberMenu();
            return;
        }

        // Display the relevant completed sessions
        std::cout << "\nCompleted Session List: \n\n";
        for (size_t i = 0; i < completedSessionList.size(); i++)
        {
            auto session = completedSessionList[i];
            std::cout << "--> " << i + 1 << ". "
                      << "From: " << session->rentFrom->toString()
                      << ", To: " << session->rentTo->toString()
                      << ", Supported By: " << session->supportedByMember->get_name()
                      << ", Hosted By: " << session->rentedByMember->get_name() << "\n";
        }
        std::cout << completedSessionList.size() + 1 << ". Back to member menu\n";

        // Choose a session for further action
        int choice = choiceFunc(1, completedSessionList.size() + 1);
        if (choice == completedSessionList.size() + 1)
        {
            memberMenu();
        }
        else
        {
            hostRateSupporterMenu(choice - 1);
        }
    }
    else
    {
        memberMenu();
    }
}

void System::hostRateSupporterMenu(int sessionIndex)
{
    if (sessionIndex < 0 || sessionIndex >= completedSessionList.size())
    {
        std::cerr << "Invalid session index.\n";
        completedSessionListMenu();
        return;
    }

    auto session = completedSessionList[sessionIndex];
    auto supporter = session->supportedByMember;

    // Display supporter's information
    supporter->showMemInfo();

    // Prompt for rating
    std::cout << "-->\t1. Rate this member\n"
              << "-->\t2. Back to completed session list\n";
    int userChoice = choiceFunc(1, 2);
    if (userChoice == 1)
    {
        memberRateSupporterAndSkill(supporter);
        std::cout << "You have successfully rated the supporter and their skill(s)!";
        memberMenu();
    }
    else
    {
        completedSessionListMenu();
    }
}

// Save data to files when exiting the program;
void System::systemExit()
{
    auto *outputStorageManager = new OutputData();
    outputStorageManager->outputStorageLoadDataFromSystem(this);
    outputStorageManager->outputStorageToFileList();
    std::exit(0); // Ensure proper function call
}

void System::memberViewRenterReviewList(int choice)
{
    auto skillRequestList = currentMember->ownedSkill->skillRequestList;
    auto host = skillRequestList[choice - 1]->requestedByMember;
    // Renter reviews
    std::cout << host->viewSupporterRateHost();

    std::cout << "\n\n--> 1.\tAccept the request\n\n"
              << "--> 2.\tBack to menu list\n";

    switch (choiceFunc(1, 2))
    {
    case 1:
    {
        // Accept the request
        if (currentMember->acceptRequest(choice - 1))
        {
            std::cout << "Request accepted successfully\n";
        }
        else
        {
            std::cout << "Failed to accept request\n";
        }
        memberMenu();
        break;
    }
    case 2:
    {
        // Back to member menu
        memberMenu();
        break;
    }
    }
}