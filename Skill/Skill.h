
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

public:
    Skill(int skillID, std::vector<std::string> skillList, int cityID);

    std::vector<std::string> getSkillList() const;
    int getSkillID() const;
    int getCityID() const;
    DateTime* getAvailableFrom() const;
    DateTime* getAvailableTo() const;
    int getCreditCostPerHour() const;
    std::optional<double> getMinHostRating() const;

    void addSkillRent(SkillRent* skillRent);
    std::string getSkillInfo() const;
    std::string getCityName() const;
    std::string getListOfSkill() const;

    bool addRequestToSkillRequestList(Request* request);
    bool removeRequestFromSkillRequestList(Request *request);

    ~Skill();

    friend class Member;
    friend class System;

    friend class InputData;
    friend class OutputData;
};

#endif //SKILL_H



