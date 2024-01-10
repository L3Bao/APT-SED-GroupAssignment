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

public:
    void test();
    Member(int memberID, std::string username, std::string password, std::string firstName, std::string lastName,
           std::string phoneNumber, std::string email, std::string address, int creditPoints = 20);


    double getRatingScore();

    bool showMemInfo();

    bool listSkill(DateTime *startTime, DateTime *endTime, int creditPointsPerHour, double minimumHostRating);

    bool unlistSkill();

    bool showListOfRequest();

    bool denyRequest(int acceptedRequestID);

    bool acceptTheRequest(int acceptedRequestID);

    // bool completRequest(int turnbackRentMotorbikeID); <?????>

    // bool ReviewSkill (Motorbike *rentMotorbike, int scoreRating, std::string comment); <?????>

    // bool showUnrateMemList(); <?????>

    // bool RatingRenter(int RatingedRenterID, int score, std::string comment); <?????>

//    std::string toStr();

    // // bool setNewMotorbike(Motorbike *motorbike); 

    // std::string get_name();

    // bool addToRequestList(Request *addedRequest);

    // bool RemoveFromRequestList(Request *removedRequest);

    // bool addRenter(MemberRent *addedRent);

    // bool removeRenter(MemberRent *turnbackRent);

    // bool showRenter();

    // bool addRatingToMember(Rating *memberRating);

    // bool minusCreditPoints(int points);

    // bool addCreditPoints(int points);

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
