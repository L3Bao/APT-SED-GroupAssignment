#ifndef MEMBER_H
#define MEMBER_H

#include "../User/User.h"
// #include "../../Constant.h"
// #include "../../Validation/Validation.h"
#include <iostream>
#include <vector>

class Skill;
class Rating;
class Request;
class MemberRent;
class DateTime;

class Member: public User {
    int memberID;

    std::string firstName;
    std::string lastName;
    std::string phoneNumber;
    std::string email;
    std::string address;
    int creditPoints;

    Skill *ownedSkill;

    std::vector<Rating*> memberRatingList;

    std::vector<Request*> memberRequestList;

    std::vector<MemberRent*> memberRentList;

    std::vector<Member*> blockedMemberList;

public:
    Member(int memberID, std::string username, std::string password, std::string firstName, std::string lastName,
           std::string phoneNumber, std::string email, std::string address, int creditPoints = 20);

    void rateAsHost(Member* supporter, int skillRating, int supporterRating);

    void rateAsSupporter(Member* host, int hostRating);

    std::tuple<double, double, double> getRatingScore();

    bool showMemInfo();

    bool listSkill(DateTime *startTime, DateTime *endTime, int creditPointsPerHour, double minimumHostRating);

    bool unlistSkill();

    bool showListOfRequest();

    bool denyRequest(int acceptedRequestID);

    bool acceptRequest(int acceptedRequestID);

    bool completeRequest(int completedSkillID); 

    // bool ReviewSkill (Motorbike *rentMotorbike, int scoreRating, std::string comment); <?????>

    // bool showUnrateMemList(); <?????>

    // bool RatingRenter(int RatingedRenterID, int score, std::string comment); <?????>

//    std::string toStr();

    bool setNewSkill(Skill *skill); 

    std::string get_name();

    // bool addToRequestList(Request *addedRequest);

    bool RemoveFromRequestList(Request *removedRequest);

    bool addHost(MemberRent *addedRent);

    // bool removeHost(MemberRent *completeRequest);

    // bool showRenter();

    // bool addRatingToMember(Rating *memberRating);

    bool minusCreditPoints(int points);

    bool addCreditPoints(int points);

    /*
     * View Member Rating by admin
     */

    // std::string viewMemberRating();

    /*
    Get current rented motorbike
    */

    ~Member();

    friend class System;

    friend class OutputData;
    friend class InputData;

    friend class Validation;
};


#endif //MEMBER_H
