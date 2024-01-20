#ifndef RATING_H
#define RATING_H

#include <iostream>

class Member;

struct RatingScores {
    int skillRating;
    int supporterRating;
    int hostRating;

    RatingScores(int skillRating = 0, int supporterRating = 0, int hostRating = 0) : skillRating(skillRating), supporterRating(supporterRating), hostRating(hostRating) {}

    int getSkillRating() const { return skillRating; }
    int getSupporterRating() const { return supporterRating; }
    int getHostRating() const { return hostRating; }
};

class Rating {
private:
    RatingScores scores;
    std::string comment;
    Member* reviewedByMember;
    Member* receivedByMember;
public:
    Rating (const RatingScores& scores, std::string comment, Member* reviewedByMember, Member* receivedByMember);

    friend class System;
    friend class Member;
    friend class Skill;
    
    friend class InputData;
    friend class OutputData;
};

#endif