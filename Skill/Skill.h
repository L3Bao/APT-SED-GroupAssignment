
#ifndef Skill_H
#define Skill_H
#include <iostream>
#include <vector>
#include <optional>
const std::string CITY_NAME_LIST[] = {"Hanoi", "Saigon"};
class DateTime;
class SkillRent;
class Member;
class Request;

class Skill {
    int skillID;
    std::vector<std::string> skillList;
    Member* skillOwner;
    bool isListed;
    int cityID;
    DateTime* availableFrom;
    DateTime* availableTo;
    int creditCostPerHour;
    std::optional<double> minHostRating;

    std::vector<Request*> skillRequestList;

    std::vector<SkillRent*> skillRentList;

    std::vector<SkillRent*> completedSkillList;
public:
    Skill(int skillID, std::vector<std::string> skillList, int cityID);

    std::string getSkillInfo();

    std::string getCityName();

    bool addRequestToSkillRequestList(Request* request);

    bool removeRequestFromSkillRequestList(Request *request);

    bool addCompletedSession(SkillRent *host);

    ~Skill();

    friend class Member;

    friend class System;
};
#endif //SKILL_H



