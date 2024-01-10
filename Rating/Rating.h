#ifndef RATING_H
#define RATING_H

#include <iostream>

class Member;

struct RatingScores {
    int skillRating;
    int supporterRating;
    int hostRating;

    RatingScores(int skillRating = 0, int supporterRating = 0, int hostRating = 0) : skillRating(skillRating), supporterRating(supporterRating), hostRating(hostRating) {}
};

class Rating {
private:
    RatingScores scores;
    std::string comment;
    Member* reviewedByMember;
public:
    Rating (const RatingScores& scores, std::string comment, Member* reviewedByMember);

    friend class System;
    friend class Member;
    friend class Skill;
};

#endif