#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <string>
#include <ctype.h>
#include "Validation\Validation.h"


class Skill;
class Member;
class Admin;
class DateTime;
class OutputData;
class Validation;

class System {

public:

    std::vector<Skill*> systemSkillList;
    std::vector<Member*> systemMemberList;
    std::vector<Skill*> memberSuitableSkillList;
    std::vector<Admin*> systemAdminList;
    Member* currentMember;
    Validation validate;

    System();

    void setCurrentMember(Member *member);

    void clearScreen();

    void addMember(Member *member);

    void addSkill(Skill *Skill);

    void addAdmin(Admin *admin);

    bool memberLogin();

    bool memberSearchSuitableSkillList(DateTime *startDate, DateTime *endDate, int cityID);

    bool isSuitableSkill(DateTime *startDate, DateTime *endDate, int cityID, Skill *Skill);

    bool memberSendRequest(DateTime *startDate, DateTime *endDate, int SkillID);

    bool memberSeeRequestList();

    bool memberAcceptRequest(int requestID);

    bool memberViewRentList();

    bool turnbackSkill(int turnbackRentSkillID);

    bool memberViewUnratedRenterList();

    bool memberRateRenter(int renterID);

    bool memberRateSkill(Skill* ratedSkill);

    void mainMenu();

    void adminMenu();

    bool adminLogin();

    void adminViewMemberList();

    void adminViewSkillList();

    void memberMenu();

    void memberLoginMenu();

    bool memberRegister();

    void memberListMenu();

    bool memberListSkill();

    bool memberUnlistSkill();

    void memberSearchSuitableSkillMenu();

    void memberSuitableSkillMenu(DateTime *sD, DateTime *eD, int cityID);

    void memberViewSkillRequestList(int skillID);

    bool memberViewSkillReviewList(int skillID, DateTime *sD, DateTime *eD);

    void memberUnratedRenterRatingMenu(int member);

    void memberViewRentedSkill();

    bool memberLeaveReview(Skill *turnbackSkill);

    bool memberEnterSkillInfo();

    void guestMenu();

    void guestViewSkillList();

    void UnrateMemRatingMenu();

    void memberViewSkillRequestListMenu();

    void suitableSkillMenu(int choice, DateTime *sD, DateTime *eD, int cityID);

    void memberViewRenterReviewList(int choice);

    int choiceFunc(int a, int b);

    void systemExit();

    friend class InputStorageManager;

    friend class OutputStorageManager;

};


#endif //SYSTEM_H
