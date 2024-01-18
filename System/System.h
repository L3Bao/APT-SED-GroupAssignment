#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <string>
#include <ctype.h>
#include "../Validation/Validation.h"


class Skill;
class Member;
class Admin;
class DateTime;
class OutputData;
class Validation;
class BlockedMember;
class SkillRent;

class System {

public:

    std::vector<Skill*> systemSkillList;
    std::vector<Member*> systemMemberList;
    std::vector<Member*> memberSuitableSkillList;
    std::vector<Skill*> suitableSkillsList;
    std::vector<Admin*> systemAdminList;
    std::vector<BlockedMember*> systemBlockedMemberList;
    std::vector<SkillRent*> completedSessionList;
    Member* currentMember;
    Validation validate;

    System();

    void setCurrentMember(Member *member);

    void clearScreen();

    void addMember(Member *member);

    void addSkill(Skill *Skill);

    void addAdmin(Admin *admin);

    void addBlockedMember(BlockedMember *blockedMember);

    bool memberLogin();

    bool memberSearchSuitableSkillList(DateTime *startDate, DateTime *endDate, int cityID);

    bool isSuitableSkill(DateTime *startDate, DateTime *endDate, int cityID, Skill *Skill);

    bool memberSendRequest(DateTime *startDate, DateTime *endDate, int SkillID);

    bool memberAcceptRequest(int requestID);

    bool memberViewRentList();

    bool completeRequest(int turnbackRentSkillID);

    bool memberRateHost(Member* host);

    bool memberRateSupporterAndSkill(Member* supporter);

    void mainMenu();

    void adminMenu();

    bool adminLogin();

    void memberMenu();

    void memberLoginMenu();

    void completedSessionListMenu();

    void hostRateSupporterMenu(int member);

    bool memberRegister();

    void memberListMenu();

    bool memberListSkill();

    bool memberUnlistSkill();

    void memberSearchSuitableSkillMenu();

    void memberSuitableSkillMenu(DateTime *sD, DateTime *eD, int cityID);

    bool memberViewSkillReviewList(int skillID, DateTime *sD, DateTime *eD);

    void memberViewRentedSkill();

    bool memberEnterSkillInfo();

    bool adminChangePassword();

    void displayMemberList();

    Member* chooseMember();

    bool getUserYesNoResponse();

    bool blockMemberInteraction(Member* requestingMember);

    void topUpCreditPoints();

    void guestMenu();

    void viewMemberInformation();

    void guestViewSupporterList();

    void memberViewSkillRequestListMenu();

    void suitableSkillMenu(int choice, DateTime *sD, DateTime *eD, int cityID);

    void memberViewRenterReviewList(int choice);

    int choiceFunc(int a, int b);

    void systemExit();

    friend class Loader;

    friend class Saver;

};


#endif //SYSTEM_H
