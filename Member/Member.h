#ifndef MEMBER_H
#define MEMBER_H

#include "../User/User.h"
#include "Member.h"
#include "../Validation/Validation.h"
#include <iostream>
#include <vector>
#include <map>
#include <optional>

class Skill;
class Rating;
class Request;
class MemberRent;
class DateTime;
class Member;
class SkillRent;
class BlockedMember;
class Member: public User {
    int memberID;

    std::string firstName;
    std::string lastName;
    std::string phoneNumber;
    std::string email;
    std::string address;
    int creditPoints;

    Skill *ownedSkill;

    std::vector<Rating*> memberRatingSupporterAndSkillList;

    std::vector<Rating*> memberRatingHostList;

    std::vector<Request*> memberRequestList;

    std::vector<MemberRent*> memberRentList;

    std::vector<SkillRent*> completedSessionList;

    std::map<int, BlockedMember*> blockedMembers;

public:
    Member(int memberID, std::string username, std::string password, std::string firstName, std::string lastName, std::string phoneNumber, std::string email, std::string address, int creditPoints = 20);

    std::tuple<double, double, double> getRatingScore();

    bool showMemInfo(Member* memberToShow = nullptr);

    bool listSkill(DateTime *startTime, DateTime *endTime, int creditPointsPerHour, std::optional<double> minimumHostRating);

    bool unlistSkill();

    bool showListOfRequest();

    bool denyRequest(int acceptedRequestID);

    bool acceptRequest(int acceptedRequestID);

    bool completeRequest(int completedSkillID); 

    bool rateSupporterAndSkill(Member* supporter, int skillRating, int supporterRating, std::string comment);

    bool setNewSkill(Skill *skill); 

    std::string get_name();

    bool addToRequestList(Request *addedRequest);

    bool RemoveFromRequestList(Request *removedRequest);

    bool addHost(MemberRent *addedRent);

    bool removeHost(MemberRent *completedRequest);

    bool rateHost(Member* host, int hostRating, std::string comment);

    bool showCurrentSkillRent();

    bool showCompletedSession();

    bool addCompletedSession(SkillRent* skillRent);

    bool addToRateSupporterAndSkillList(Rating *memberRating);

    bool addToRateHostList(Rating *memberRating);

    bool minusCreditPoints(int points);

    bool addCreditPoints(int points);

    std::string viewSupporterRateHost();

    std::string viewHostRateSupporter();

    std::string guestViewSupporterInfo();

    std::string viewSupporterInfoInDetail();

    void addBlockedMember(Member* blockedMember, bool blockView, bool blockRequestSupport);

    // Method for blocking another member
    bool blockMember(Member* memberToBlock, bool blockView, bool blockRequestSupport);

    // Method to check if a member is already blocked
    bool isMemberBlocked(Member* member);

    bool isBlockedForViewing(Member* member);
    bool isBlockedForRequesting(Member* member);

    void setPassword(const std::string& newPassword);

    bool topUpCreditPoints(int amount, const std::string& password);

    ~Member();

    friend class System;

    friend class OutputData;
    friend class InputData;

    friend class Validation;
};


#endif //MEMBER_H
