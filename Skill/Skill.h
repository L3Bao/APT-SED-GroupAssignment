#include <iostream>
#include <vector>

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
    DateTime* availableFrom;
    DateTime* availableTo;
    int creditCostPerHour;
    double minHostRating;

    std::vector<Rating*> skillRatingList;

    std::vector<Request*> skillRequestList;

    std::vector<SkillRent*> skillRentList;

public:
    Skill(int skillID, std::vector<std::string> skillList);

    std::string getSkillList();

    double getRatingScore();

    bool addRequestToSkillRequestList(Request* request);

    bool removeRequestToSkillRequestList(Request *request);

    bool addReviewToSkill(Rating *skillReview);

    std::string viewSkillInfo();

    std::string viewSkillReview();

    ~Skill();

    friend class Member;

    friend class System;
};