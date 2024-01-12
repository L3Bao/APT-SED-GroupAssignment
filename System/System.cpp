#include "System.h"
#include "DateTime\DateTime.h"
#include "Skill\Skill.h"
#include "Member\Member.h"
#include "Admin\Admin.h"
#include "SkillRent\SkillRent.h"
#include "MemberRent\MemberRent.h"
#include "Request\Request.h"
#include "Rating\Rating.h"
#include "Validation\Validation.h"
#include "Middleware\Middleware.h"
/* #include "../StorageManager/InputData/InputData.h"
#include "../StorageManager/OutputData/OutputData.h" */
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
using std::cout;

//Clear system info when initialized
System::System() {
    this->currentMember = nullptr;
    this->systemSkillList.clear();
    this->systemMemberList.clear();
    this->memberSuitableSkillList.clear();
}

//Clear screen
void System::clearScreen(){
    std::cout << "Loading..." << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(750) );
    for (int  n = 0; n < 10; n++){
        cout << "\n\n\n\n\n\n\n\n\n\n" ;
    }
    
}

//Set current member when logging in
void System::setCurrentMember(Member *member) {
    currentMember = member;
    memberSuitableSkillList.clear();
}

//Add member when registering
void System::addMember(Member *member) {
    systemMemberList.push_back(member);
}

//Add motorbike after entering motorbike info
void System::addSkill(Skill *skill) {
    systemSkillList.push_back(skill);
}

//Add new admin
void System::addAdmin(Admin *admin) {
    systemAdminList.push_back(admin);
}

//Function to show suitable supporter list
bool System::memberSearchSuitableSkillList(DateTime *startDate, DateTime *endDate, int cityID) {
    memberSuitableSkillList.clear();

    // If that member already send another request for this supporter and that request is overlapped with the current request
    for (auto &request: currentMember->memberRequestList) {

        // If two time frame are not overlapped
        if (*request->requestTo < *startDate || *endDate < *request->requestFrom) {
            continue;
        }

        std::cout << "\nYour request is overlapped!!!\n\n";
        return false;
    }

    // Find the avalaible supporter
    for (auto &skill : systemSkillList) {
    if (isSuitableSkill(startDate, endDate, cityID - 1, skill)) {
        suitableSkillsList.push_back(skill); // Add the skill
        // If you also want to keep track of the members
        memberSuitableSkillList.push_back(skill->skillOwner); // Add the skill owner
    }
}



    // If there is no availalble supporters
    if (memberSuitableSkillList.empty()) {
        std::cout << "\nNo supporter is suitable for you!\n";
        return false;
    }

    // Print the suitable supporter list for user
    std::cout << "\nThe list of suitable supporters for you:\n\n";
    for (size_t i = 0; i < suitableSkillsList.size(); ++i) {
        Skill* skill = suitableSkillsList[i];
        Member* supporter = memberSuitableSkillList[i];
        auto [avgSkillRating, avgSupporterRating, avgHostRating] = supporter->getRatingScore();

        std::cout << "--> " << i + 1 << ". ";
        std::cout << "Name: " << supporter->get_name() << ", ";
        std::cout << "Skills: " << skill->getSkillInfo() << ", ";
        std::cout << "Credit Per Hour: " << skill->creditCostPerHour << ", ";
        std::cout << "Available: From " << skill->availableFrom->toString() << " to " << skill->availableTo->toString() << ", ";
        std::cout << "Skill's Rating: " << avgSkillRating << ", ";
        std::cout << "Supporter's Rating: " << avgSupporterRating << "\n";
    }
    std::cout << "\n\n";

    return true;
}

//Function to check if the supporter is suitable to the member
bool System::isSuitableSkill(DateTime *startDate, DateTime *endDate, int cityID, Skill *skill) {

    auto [avgSkillRating, avgSupporterRating, avgHostRating] = skill->skillOwner->getRatingScore();

    // If the skill is not listed
    if (!skill->isListed) {
        return false;
    }

    // If the skill is owned by currentMember
    if (skill == currentMember->ownedSkill) {
        return false;
    }

    // If the member do not have minimum required score
    if (avgHostRating < skill->minHostRating) {
        return false;
    }

    // If the motorbike is not available from the start and end date
    if (*startDate - *(skill->availableFrom) < 0 || *skill->availableTo - *endDate < 0) {
        return false;
    }

    // If the user do not have enough credit Points
    int differenceBetweenStartAndEndDate = *endDate - *startDate;
    if (differenceBetweenStartAndEndDate * skill->creditCostPerHour > currentMember->creditPoints) {
        return false;
    }

    // If the supporter is in different city
    if (cityID != skill->cityID) {
        return false;
    }

    // Loop over the rent status of the motorbike for checking
    for (auto &rent: skill->skillRentList) {

        // If the startDate and endDate is not rented by another
        if ((startDate < rent->rentFrom && endDate < rent->rentTo)
            || (startDate > rent->rentFrom && endDate > rent->rentTo)) {
            continue;
        }

        return false;
    }

    

    return true;
}

//Function to view motorbike reviews
bool System:: memberViewSkillReviewList(int SkillID, DateTime *sD, DateTime *eD) {
    if (SkillID>=  memberSuitableSkillList.size()) {
        return false;
    }

    auto skill =  memberSuitableSkillList[SkillID];
    std::cout << skill->viewSkillReview();
    return true;
}

//Function to send request to the motorbike owner
bool System::memberSendRequest(DateTime *startDate, DateTime *endDate, int SkillID) {
    if (SkillID >= memberSuitableSkillList.size()) {
        return false;
    }

    for (auto &memberRequest: currentMember->memberRequestList) {
        if (*(memberRequest->requestTo) < *startDate || *endDate < *(memberRequest->requestFrom)) {
            continue;
        }

        return false;
    }

    // Create an object for the request
    auto *request = new Request(startDate, endDate, currentMember);

    // Add the request to the request list of the motorbike
    memberSuitableSkillList[SkillID]->addRequestToSkillRequestList(request);

    // Add the request to the request list of the member
    currentMember->addToRequestList(request);

    return true;
}

//Function to accept motorbike rent request
bool System::memberAcceptRequest(int requestID) {

    // Current member accept the request
    currentMember->acceptTheRequest(requestID);

    return true;
}

//Function to view rent list
bool System::memberViewRentList() {

    currentMember->showRenter();
    return true;
}

//Function for current member to turnback the motorbike
bool System::turnbackSkill(int turnbackRentSkillID) {
    currentMember->turnbackSkill(turnbackRentSkillID);
    return true;
}

//Function to view unrated renters
bool System::memberViewUnratedRenterList() {
    return currentMember->showUnrateMemList();
}

//Function to rate renter
bool System::memberRateRenter(int renterID) {
    int rating;
    std::string comment;
    std::cout << "\nRate this member from 1 to 10\n";
    rating = choiceFunc(1,10);
    std::cout << "\nEnter a comment about this member: ";
    std::cin.ignore();
    std::getline(std::cin, comment);
    return currentMember->reviewRenter(renterID, rating, comment);
}

//Function to rate skill
bool System:: memberRateSkill(Skill *ratedSkill) {
    int rating;
    std::string comment;
    std::cout << "\n---Rate this skill from 1 to 5---\n";
    rating = choiceFunc(1,10);
    std::cout << "\nEnter a comment about this skill: ";
    std::cin.ignore();
    std::getline(std::cin, comment);
    return currentMember->reviewSkill(ratedSkill, rating, comment);
}

//Function for entering choice
int System::choiceFunc(int a, int b) {
    std::string temp;
    int tempInt;
    int err;

    do{
        err = 0;
        std::cout << "\nEnter your choice (Number only): ";
        std::cin >> temp;
        if (!isNumber(temp)){
            std::cout << "Error. Please try again!\n";
            err++;
            continue;
        }
        tempInt = convertStringToInt(temp);
        if (tempInt < a || tempInt > b){
            std::cout << "Error. Please try again!\n";
            err++;
            continue;
        }
    } while (err);

    return tempInt;
}



//main menu
void System::mainMenu() {
    std :: cout << "\n\n\n    *    MOTORBIKE RENTAL APPLICATION    *\n\n\n";
    cout << "\t--> MAIN MENU <--\n\n"
         << "\t--> 1. Guest\n"
         << "\t--> 2. Member\n"
         << "\t--> 3. Admin\n"
         << "\t--> 4. Exit\n\n";
    switch (choiceFunc(1, 4)){
        case 1:
            guestMenu();
            break;

        case 2:
            memberLoginMenu();
            break;

        case 3:
            if(adminLogin()){
                std::cout << "Login successful\n";
                adminMenu();
            } else {
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
//end main




//guest (feature 2)
void System::guestMenu() {
    clearScreen();
    std :: cout << "\n\n\n    *    SKILL RENTAL APPLICATION    *\n\n\n";
    std::cout << "\t--> GUEST MENU <--\n\n"
              << "--> 1.\tView SKILL list\n"
              << "--> 2.\tRegister for an account\n"
              << "--> 3.\tBack to main menu\n";
    switch (choiceFunc(1, 3)){
        case 1:
            //Print skill list
            guestViewSkillList();
            break;
        case 2:
            clearScreen();
            //Register new member
            if(memberRegister()){
                std::cout << "Registration successful\n";
                memberLoginMenu();
            }
            break;
        case 3:
            //Back to main menu
            clearScreen();
            mainMenu();
            break;
    }
}

void System::guestViewSkillList(){
    int choice;
    clearScreen();
    std :: cout << "\n\n\n    *    SKILL RENTAL APPLICATION    *\n\n\n";
    std::cout << "\n\t\t--> SKILL LIST <-- \n"
              << "ID    Information \n\n";
    //Print skill list
    for (Skill *mot : systemSkillList){
        std::cout << std::setw(6) << std::left << std::to_string(mot->skillID) + ".";
        std::cout << mot->getSkillInformation() << "\n";
    }
    std::cout << "\n" << systemSkillList.size()+1 << ". Back to guest menu";
    choice = choiceFunc(1, systemSkillList.size()+1);
    if (choice == systemSkillList.size()+1){
        //Go back to guest menu
        guestMenu();
    } else {
        //View motorbike info
        highlighter();
        cout << "\nYou have chosen motorbike number " << choice;
        std::cout << "\n" << systemSkillList[choice - 1]->viewSkillInfoByGuest();
        std::cout << "\n1. Back to skill list\n";
        switch(choiceFunc(1, 1)){
            case 1:
                guestViewSkillList();
                break;
        }
    }
}
//end guest




//admin (feature 5)
bool System::adminLogin() {
    clearScreen();
    std :: cout << "\n\n\n    *    SKILL RENTAL APPLICATION    *\n\n\n";
    std::string username, password;
    //Enter admin username and password
    std::cout << "\t--> ADMIN LOGIN <--\n\n"
              << "Username: ";
    std::cin.ignore();
    std::getline(std::cin, username);
    std::cout << "\nPassword: ";
    std::getline(std::cin, password);

    for (auto &admin: systemAdminList) {

        // Check whether the password and username is valid
        if (username != admin->username || password != admin->password) {
            continue;
        }

        return true;
    }

    return false;
}

void System::adminMenu() {
    clearScreen();
    std :: cout << "\n\n\n    *    SKILL RENTAL APPLICATION    *\n\n\n";
    std::cout << "\t--> ADMIN MENU <--\n\n"
              << "--> 1.\tMember List \n"
              << "--> 2.\tSkill List \n"
              << "--> 3.\tSign out";
    switch (choiceFunc(1, 3)){
        case 1:
            //View member list
            clearScreen();
            adminViewMemberList();
            break;

        case 2:
            //View motorbike list
            adminViewSkillList();
            break;

        case 3:
            //Back to main menu
            clearScreen();
            mainMenu();
            break;
    }
}

void System::adminViewMemberList() {
    int choice;
    std :: cout << "\n\n\n    *    MOTORBIKE RENTAL APPLICATION    *\n\n\n";
    std::cout << "\n\t--> MEMBER LIST <-- \n\n"
              << "ID    Name  \n";
    //Print member list
    for (Member *mem : systemMemberList){
        std::cout << std::setw(6) << std::left << (std::to_string(mem->memberID) + ".");
        std::cout << mem->get_name() << "\n";
    }
    std::cout << systemMemberList.size()+1 << ".\tBack to admin menu\n";
    choice = choiceFunc(1, systemMemberList.size()+1);
    
    clearScreen();
    system("clear");
    cout << "    *    SKILL RENTAL APPLICATION    *\n";
    if (choice == systemMemberList.size()+1){
        //Back to admin menu
        adminMenu();
    } else {
        //View member info
        systemMemberList[choice - 1]->showMemInfo();
        std::cout << "1. View member reviews\n"
                  << "2. Back to member list\n";
        switch(choiceFunc(1, 2)){
            case 1:
                //View member reviews
                std::cout << systemMemberList[choice - 1]->viewMemberReview();
                std::cout << "1. Back to member list\n";
                choiceFunc(1, 1);
                //Back to member list
                adminViewMemberList();
                break;
            case 2:
                //Back to member list
                adminViewMemberList();
                break;
        }

    }
}

void System::adminViewSkillList() {
    int choice;
    clearScreen();
    std :: cout << "\n\n\n    *    MOTORBIKE RENTAL APPLICATION    *\n\n\n";
    std::cout << "\n\t\t--> MOTORBIKE LIST <-- \n"
              << "ID    Information \n\n";
    //Print motorbike list
    for (Skill *mot : systemSkillList){
        std::cout << std::setw(6) << std::left << std::to_string(mot->skillID) + ".";
        std::cout << mot->getSkillInformation() << "\n";
    }
    std::cout << systemSkillList.size()+1 << ". \tBack to Admin Menu\n";
    choice = choiceFunc(1, systemSkillList.size()+1);
    if (choice == systemSkillList.size()+1){
        adminMenu();
    } else {
        clearScreen();
        //Print motorbike info
        std :: cout << "\n\n\n    *    MOTORBIKE RENTAL APPLICATION    *\n\n\n";
        std::cout << "\n" << systemSkillList[choice - 1]->viewSkillInfo() << "\n\n";
        std::cout << "--> 1.\tView reviews\n"
                  << "--> 2.\tBack to motorbike list\n";
        switch(choiceFunc(1, 2)){
            case 1:
                //Motorbike review
                std::cout << systemSkillList[choice - 1]->viewSkillReview();
                std::cout << "1. Back to motorbike list";
                choiceFunc(1, 1);
                //Back to motorbike list
                adminViewSkillList();
                break;
            case 2:
                //Back to motorbike list
                adminViewSkillList();
                break;
        }
    }
}
//end admin




//member
    //member login (feature 4)
void System::memberLoginMenu() {
    clearScreen();
    std :: cout << "\n\n\n    *    SKILL RENTAL APPLICATION    *\n\n\n";
    std::cout << "\t--> MEMBER LOGIN MENU <--\n\n"
              << "--> 1.\tLogin\n"
              << "--> 2.\tBack to main menu\n";
    switch(choiceFunc(1, 2)){
        case 1:
            //Login to a member account
            if(memberLogin()){
                std::cout << "Login successful\n";
                memberMenu();
            } else {
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

bool System::memberLogin() {
    std::string username, password;
    //Enter member account info
    std::cout << "\n\t--> MEMBER LOGIN <--\n\n"
              << "Username: ";
    std::cin >> username;
    std::cout << "\nPassword: ";
    std::cin >> password;
    //Check if username and password is correct
    for (Member *mem : systemMemberList){
        if (username == mem->username && password == mem->password){
            setCurrentMember(mem);
            return true;
        }
    }
    return false;
}
    //end login


    //member register (feature 1)
bool System::memberRegister() {
      std::string username, password,  firstName,  lastName,
            phoneNumber,  email, address;

    std :: cout << "\n\n\n    *    SKILL RENTAL APPLICATION    *\n\n\n";

    //Enter member information
    std::cout << "\t--> CUSTOMER REGISTRATION <--\n\n";
    do {
        std::cout << "Enter a username: ";
        std::cin.ignore();
        std::getline(std::cin, username);
    } while (!validate.username(username, this));

    do {
        std::cout << "\nEnter a password: ";
        std::getline(std::cin, password);
    } while (!validate.password(password));

    do {
        std::cout << "\nEnter your first name: ";
        std::getline(std::cin, firstName);
    } while (!validate.name(firstName));

    do {
        std::cout << "\nEnter your last name: ";
        std::getline(std::cin, lastName);
    } while (!validate.name(lastName));

    do {
        std::cout << "\nEnter your phone number: ";
        std::getline(std::cin, phoneNumber);
    } while (!validate.phoneNumber(phoneNumber , this));

        std::cout << "\nEnter your email: ";
        std::getline(std::cin, email);
    
    do {
        std::cout << "\nEnter your address: ";
        std::getline(std::cin, address);
    } while (!validate.address(address , this));
    


    auto *newMember = new Member(systemMemberList.size() + 1, username, password, firstName, lastName, phoneNumber, email, address);
    //Add member to member list
    addMember(newMember);
    return true;
}
    //end register


    //menu for member
void System::memberMenu(){
    clearScreen();
    std :: cout << "\n\n\n    *    MOTORBIKE RENTAL APPLICATION    *\n\n\n";
    std::cout << "\t--> MEMBER MENU <--\n\n"
              << "--> 1.\tView account info\n"
              << "--> 2.\tMotorbike Menu\n"
              << "--> 3.\tSearch for available motorbike\n"
              << "--> 4.\tView motorbike request list\n"
              << "--> 5.\tView unrated renter list\n"
              << "--> 6.\tView currently rented motorbike\n"
              << "--> 7.\tSign out\n";
    switch(choiceFunc(1,7)){
        case 1:
            //View member info
            currentMember->showMemInfo();
            std::cout << "1. Back to member menu";
            choiceFunc(1,1);
            memberMenu();
            break;

        case 2:
            //Go to member list
            memberListMenu();
            break;

        case 3:
            //Go to search suitable motorbike
            memberSearchSuitableSkillMenu();
            break;

        case 4:
            //View motorbike requests
            memberViewSkillRequestListMenu();
            break;

        case 5:
            //View unrated renter
            UnrateMemRatingMenu();
            break;

        case 6:
            //View rented motorbike
            memberViewRentedSkill();
            break;

        case 7:
            //Sign out and go back to main menu
            setCurrentMember(nullptr);
            clearScreen();
            mainMenu();
            break;
    }
}
    //end menu


    //list menu (feature 5)
void System::memberListMenu(){
    std :: cout << "\n\n\n    *    MOTORBIKE RENTAL APPLICATION    *\n\n\n";
    cout << "\t--> LIST MENU <--\n\n";
    //Check if member has an owned motorbike
    if (currentMember->ownedSkill != nullptr){
        //Check if member's motorbike is listed
        if (currentMember->ownedSkill->isListed) {
            //Print motorbike info
            std::cout << "Currently listed motorbike:\n"
                      << currentMember->ownedSkill->viewSkillInfo() << "\n";
            if (currentMember->ownedSkill->skillRequestList.empty()) {
                std::cout << "\n--> 1.\tUnlist motorbike\n"
                          << "--> 2.\tBack to member menu\n";
                switch (choiceFunc(1, 2)) {
                    case 1:
                        //Unlist motorbike
                        memberUnlistSkill();
                        std::cout << "1. Back to list menu\n";
                        choiceFunc(1, 1);
                        memberListMenu();
                        break;
                    case 2:
                        //Back to member menu
                        memberMenu();
                        break;
                }
            } else {
                std::cout << "1. Back to member menu\n";
                int choice = choiceFunc(1, 1);
                memberMenu();
            }
        } else {
            //List motorbike if motorbike isn't listed
            std::cout << "Your motorbike isn't listed\n\n"
                      << "--> 1. List your motorbike\n"
                      << "--> 2. Back to member menu\n";
            switch (choiceFunc(1, 2)){
                case 1:
                    memberListSkill();
                    std::cout << "\nSuccessfully listed motorbike\n\n";
                    memberListMenu();
                    break;
                case 2:
                    memberMenu();
                    break;
            }
        }
    } else {
        //Enter motorbike info if member has no owned motorbike
        std :: cout << "You haven't had any motorbike on our system yet!!\n";
        cout << "1. Enter your motorbike info\n";
        cout << "2. Back to member menu\n";
        switch (choiceFunc(1, 2)){
                case 1:
                    std::cout << "Please enter your MOTORBIKE INFO \n\n";
                    memberEnterSkillInfo();
                    memberListMenu();
                    break;
                case 2:
                    memberMenu();
                    break;
            }
    }
}

bool System::memberEnterSkillInfo() {
    std::string model, color, enginesize, transmode, year, motorbikeDescrip;
    int cityID;
    //Enter motorbike info
    std::cout << "Enter the motorbike information: \n"
              << " - Motorbike model: ";
    std::cin.ignore();
    std::getline(std::cin, model);
    std::cout << "- Color: ";
    std::getline(std::cin, color);
    std::cout << "- Engine size: ";
    std::getline(std::cin, enginesize);
    std::cout << "- Motorbike transmission mode: ";
    std::getline(std::cin, transmode);
    std::cout << "- Motorbike year of produce: ";
    std::getline(std::cin, year);
    std::cout << "- Motorbike description: ";
    std::getline(std::cin, motorbikeDescrip);

    std::cout << "\nChoose the city your motorbike is in: \n\n"
              << "--> 1.\tHanoi\n"
              << "--> 2.\tSaigon\n"
              << "--> 3.\tHue\n";
    cityID = choiceFunc(1, 3) - 1;
    auto *newSkill = new Skill(systemSkillList.size() + 1, model, color, enginesize, transmode, year, cityID, SkillDescrip);
    //Add the motorbike into the motorbike list
    addSkill(newSkill);
    //Set owned motorbike to the newly added motorbike
    currentMember->setNewSkill(newSkill);
    return true;
}

        //list motorbike (feature 6.1)
bool System::memberListSkill() {
    std::string startDate, endDate, conPoints, minRentRating;
    //Enter listing info (with validation)
    std::cout << "\nEnter listing information: \n";
    int conPointsNum, minRentRatingNum;

    do{
        do {
            std::cout << " - Start date(dd/mm/yyyy): ";
            std::cin >> startDate;
        } while (!validate.date(startDate));
        do {
            std::cout << "- End date(dd/mm/yyyy): ";
            std::cin >> endDate;
        } while (!validate.date(endDate));
    } while (!validate.listDate(startDate, endDate));

    do {
        std::cout << "- Consuming points per day: ";
        std::cin >> conPoints;
        if (!isNumber(conPoints)){
            std::cout << "Your option is not a number. Please try again.\n";
        }
    } while (!isNumber(conPoints));
    conPointsNum = convertStringToInt(conPoints);

    do {
        std::cout << "- Minimum Renter Rating: ";
        std::cin >> minRentRating;
        if (!isDouble(minRentRating)){
            std::cout << "\nYour option is not a number. Please try again.\n";
        }
    } while (!isDouble(minRentRating));
    minRentRatingNum = convertStringToDouble(minRentRating);

    Date *sD = convertStringToDate(startDate), *eD = convertStringToDate(endDate);
    //Add listing info to data
    currentMember->listMotorbike(sD, eD, conPointsNum, minRentRatingNum);
    return true;
}

        //unlist motorbike (feature 6.2)
bool System::memberUnlistSkill() {
    if(currentMember->unListSkill()){
        std::cout << "\nSuccessfully unlist motorbike \n\n";
        return true;
    } else {
        std::cout << "Failed to unlist motorbike. Please accept member request before unlisting the motorbike.\n";
        return false;
    }
}
    //end list menu


    //search suitable motorbike (feature 3, 7)
void System::memberSearchSuitableSkillMenu(){
    std::string start, end;
    int cityID;
    //Enter search information
    std::cout << "\nSearch for a suitable motorbike: \n\n";
    
    std::cout << "\nChoose the city you want to search for: \n\n"
              << "--> 1. Hanoi\n"
              << "--> 2. Saigon\n"
              << "--> 3. Hue\n";
    cityID = choiceFunc(1, 3);
    do {
        do {
            std::cout << "Enter start date (dd/mm/yyyy): ";
            std::cin >> start;
        } while (!validate.date(start));

        do {
            std::cout << "\n\nEnter end date (dd/mm/yyyy): ";
            std::cin >> end;
        } while (!validate.date(end));
    } while (!validate.listDate(start, end));
    Date *startDate = convertStringToDate(start), *endDate = convertStringToDate(end);
    //Go to suitable motorbike list
    memberSuitableSkillMenu(startDate, endDate, cityID);
}

void System::memberSuitableSkillMenu(Date *sD, Date *eD, int cityID) {
    //Clear the suitable motorbike vector
    memberSuitableSkillList.clear();
    //Show suitable motorbikes
    memberSearchSuitableSkillList(sD, eD, cityID);
    std::cout << memberSuitableSkillList.size() + 1 << ". Back to member menu\n";
    int choice = choiceFunc(1, memberSuitableSkillList.size() + 1);
    if (choice == (memberSuitableSkillList.size()+1)){
        //Back to member menu
        memberMenu();
    } else {
        //View motorbike info
        std::cout << memberSuitableMotorbikeList[choice - 1]->viewMotorbikeInfo();
        std::cout << "\n\n--> 1.\tRequest motorbike\n\n"
                  << "--> 2.\tView motorbike reviews\n\n"
                  << "--> 3.\tBack to motorbike list\n";
        switch (choiceFunc(1,3)){
            case 1:
                //Send motorbike request to the owner of the motorbike and go back to member menu
                memberSendRequest(sD, eD, choice - 1);
                std::cout << "\n\tRequest sent\n";
                memberMenu();
                break;
            case 2:
                //View motorbike reviews
                memberViewSkillReviewList(choice - 1, sD, eD);
                std::cout << "\n\n--> 1.\tRequest motorbike\n\n"
                          << "--> 2.\tBack to motorbike list\n";
                switch (choiceFunc(1, 2)) {
                    case 1:
                        //Send motorbike request to the owner of the motorbike
                        memberSendRequest(sD, eD, choice - 1);
                        std::cout << "\tRequest sent\n";
                        memberMenu();
                        break;
                    case 2:
                        //Back to suitable motorbike list
                        memberSuitableSkillMenu(sD, eD, cityID);
                        break;
                }
                break;
            case 3:
                //Back to suitable motorbike list
                memberSuitableSkillMenu(sD, eD, cityID);
                break;
        }
    }
}

void System::suitableSkillMenu(int choice, Date *sD, Date *eD, int cityID) {
    std::cout << memberSuitableSkillList[choice - 1]->viewMotorbikeInfo() << "\n";
    std::cout << "1. View reviews\n"
              << "2. Request motorbike\n"
              << "3. Back to motorbike list\n";
    switch (choiceFunc(1,3)){
        case 1:
            std::cout << memberSuitableSkillList[choice - 1]->viewSkillReview();
            std::cout << "1. Back to motorbike menu\n";
            choiceFunc(1, 1);
            suitableSkillMenu(choice, sD, eD, cityID);
        case 2:
            if(memberSendRequest(sD, eD, memberSuitableSkillList[choice-1]->skillID)){
                std::cout << "Request sent\n";
            } else {
                std::cout << "Failed to send request\n";
            }
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            memberMenu();
            break;
        case 3:
            memberSuitableSkillMenu(sD, eD, cityID);
            break;
    }
}
    //end search suitable motorbike


    //view motorbike request (feature 8, 9, 10, 11)
void System:: memberSuitableSkillMenu() {
    //Check if member has an owned motorbike
    if (currentMember->ownedSkill == nullptr){
        std::cout << "You haven't entered your motorbike info\n"
                  << "1. Back to member menu\n";
        choiceFunc(1, 1);
        memberMenu();
        //Check if owned motorbike is listed
    } else if (!currentMember->ownedSkill->isListed){
        std::cout << "You haven't listed a motorbike yet!\n"
                  << "1. Back to member menu\n";
        choiceFunc(1, 1);
        memberMenu();
    }

    std::cout << "\nChoose a request for view more detail: \n\n";

    //View motorbike request list
    if (currentMember->showListOfRequest()) {
        std::cout << "\n" << currentMember->ownedSkill->skillRequestList.size() + 1 << ". Back to member menu\n\n";
        int choice = choiceFunc(1, currentMember->ownedSkill->skillRequestList.size() + 1);
        if (choice == currentMember->ownedSkill->skillRequestList.size() + 1) {
            //Back to member menu
            memberMenu();
        } else {
            std::cout << "\n\n-->\t1. Accept the request\n"
                      << "-->\t2. View the reviews of this member\n"
                      << "-->\t3. Back to menu\n";
            switch (choiceFunc(1, 3)) {
                case 1: {
                    //Accept motorbike request
                    if (currentMember->acceptTheRequest(choice - 1)) {
                        std::cout << "Accepting Request successfully\n";
                    } else {
                        std::cout << "Failed to accept request\n";
                    }
//                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    memberMenu();
                    break;
                }
                case 2: {
                    //View member reviews
                    memberViewRenterReviewList(choice);
                    break;
                }
                case 3: {
                    //Back to member menu
                    memberMenu();
                    break;
                }
            }

        }
    } else {
        //Back to member menu
        std::cout << "1. Back to member menu\n";
        choiceFunc(1, 1);
        memberMenu();
    }
}

    //end view motorbike request


    //view unrated renter (feature 12)
void System::UnrateMemRatingMenu() {
    //Check if member has an owned motorbike
    if (currentMember->ownedSkill == nullptr){
        std::cout << "\nYou don't have an owned motorbike.\n\n"
                  << "1. Back to member menu\n";
        choiceFunc(1,1);
        memberMenu();
    }
    //Unrated renter list
    std::cout << "\nUnrated Renter List: \n\n";
    currentMember->showUnrateMemList();
    std::cout << "\n" << currentMember->ownedSkill->unratedRenterList.size()+1 << ". Back to member menu\n";
    int choice = choiceFunc(1, currentMember->ownedSkill->unratedRenterList.size() + 1);
    if (choice == currentMember->ownedSkill->unratedRenterList.size() + 1){
        //Back to member menu
        memberMenu();
    } else {
        //View unrated Renter info
        memberUnratedRenterRatingMenu(choice - 1);
    }
}

void System::memberUnratedRenterRatingMenu(int member){
    currentMember->ownedSkill->unratedRenterList[member]->rentedByMember->showMemInfo();
    std::cout << "-->\t1. Rate this member\n"
              << "-->\t2. Back to unrated renter list\n";
    switch (choiceFunc(1, 2)){
        case 1:
            //Rate unrated renter
            if (memberRateRenter(member)){
                std::cout << "\nSuccessfully rated member\n";
            } else {
                std::cout << "\nRating failed\n";
            }
            UnrateMemRatingMenu();
            break;
        case 2:
            //Back to unrated renter
            UnrateMemRatingMenu();
            break;
    }

}
    //end view unrated renter


    //view currently rented motorbike (turnback motorbike)
void System::memberViewRentedSkill() {
    //Check if current member is renting any motorbike
    if (currentMember->memberRentList.empty()) {
        std::cout << "\nYou are not currrently renting any motorbike\n\n"
                  << "1. Back to member menu\n";
        choiceFunc(1, 1);
        memberMenu();
    } else {
        //View rent list
        currentMember->showRenter();
        std::cout << "--> "<< currentMember->memberRentList.size() + 1 << ". Back to member menu\n";
        int choice = choiceFunc(1, currentMember->memberRentList.size() + 1);
        if (choice == currentMember->memberRentList.size() + 1) {
            //Back to member menu
            memberMenu();
        } else {
            //Check for renting information
            auto rentMotorbike = currentMember->memberRentList[choice - 1]->rentSkill;
            std::cout << "\n" << rentMotorbike->viewMotorbikeInfo() << "\n"
                      << "\n-->\t1. Turnback the motorbike\n"
                      << "\n-->\t2. Back to member menu\n";
            switch (choiceFunc(1, 2)){
                case 1:{
                    //Leave a motorbike review
                    if(memberLeaveReview(rentMotorbike)){
                        std::cout << "Motorbike reviewed\n";
                    }
                    //Turnback the motorbike
                    if(currentMember->turnbackSkill(choice - 1)){
                        std::cout << "\nMotorbike turnback successfully\n"
                                  << "Thank you for using our service\n"
                                  << "SEE YOU AGAIN!!!\n\n";
                    } else {
                        std::cout << "\nProcess unsuccessful\n";
                    }
                    std::cout << "-->\t1. Back to member menu\n";
                    choiceFunc(1, 1);
                    memberMenu();
                    break;
                }
                case 2:{
                    //Back to member menu
                    memberMenu();
                    break;
                }
            }
//                memberMenu();
        }
    }
}    //end view currently rented motorbike


    //leave review from client (feature 12)
    bool System::memberLeaveReview(Skill *turnbackSkill) {
        std::cout << "\nWould you like to leave a review?\n\n"
                  << "-->\t1. Yes\n"
                  << "-->\t2. No\n";
        switch(choiceFunc(1, 2)){
            case 1:
                //Rate motorbike
                if(memberRateSkill(turnbackSkill)){
                    return true;
                } else {
                    std::cout << "\nFailed to leave a review\n";
                    return false;
                }
                break;
            case 2:
                return false;
                break;
        }
        return true;
    }

    //Save data to files when exiting the program;
void System::systemExit() {
    auto *outputStorageManager = new Loader();
    outputStorageManager->outputStorageLoadDataFromSystem(this);
    outputStorageManager->outputStorageToFileList();
    std::exit;
}

void System::memberViewRenterReviewList(int choice) {
    auto motorbikeRequestList = currentMember->ownedSkill->skillRequestList;
    auto renter = motorbikeRequestList[choice - 1]->requestedByMember;
    //Renter reviews
    std::cout << renter->viewMemberReview();

    std::cout << "\n\n--> 1.\tAccept the request\n\n"
              << "--> 2.\tBack to menu list\n";

    switch (choiceFunc(1, 2)) {
        case 1: {
            //Accept motorbike request
            if (currentMember->acceptTheRequest(choice - 1)) {
                std::cout << "Accepting Request successfully\n";
            } else {
                std::cout << "Failed to accept request\n";
            }
            memberMenu();
            break;
        }
        case 2: {
            //Back to member menu
            memberMenu();
            break;
        }
    }
}


//end leave review


//end member