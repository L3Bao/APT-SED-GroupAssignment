#include "SkillRent.h"

SkillRent::SkillRent(DateTime *rentFrom, DateTime *rentTo, Member *rentedByMember) {
    this->rentFrom = rentFrom;
    this->rentTo = rentTo;
    this->rentedByMember = rentedByMember;
}


