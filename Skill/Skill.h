
#ifndef Skill_H
#define Skill_H
#include <iostream>
#include <vector>
#include <optional>
const std::string CITY_NAME_LIST[] = {"Hanoi", "Saigon"};
class DateTime;
class Rating;
class Request;
class SkillRent;
class Member;

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

    std::vector<Rating*> skillRatingList;

    std::vector<Request*> skillRequestList;

    std::vector<SkillRent*> skillRentList;

public:
    Skill(int skillID, std::vector<std::string> skillList, int cityID);

    std::string getSkillInfo();

    std::string getCityName();

    double getRatingScore();

    bool addRequestToSkillRequestList(Request* request);

    bool removeRequestFromSkillRequestList(Request *request);

    bool addReviewToSkill(Rating *skillReview);

    std::string viewSkillInfo();

    std::string viewSkillReview();

    ~Skill();

    friend class Member;

    friend class System;
};
#endif //SKILL_H



