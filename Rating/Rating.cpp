#include "Rating.h"

Rating::Rating(const RatingScores& scores, std::string comment, Member* reviewedByMember)
    : scores(scores), comment(std::move(comment)), reviewedByMember(reviewedByMember) {}