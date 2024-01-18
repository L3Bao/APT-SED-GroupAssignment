#include "SkillRent.h"

SkillRent::SkillRent(DateTime *rentFrom, DateTime *rentTo, Member *rentedByMember, Member *supportedByMember) {
    this->rentFrom = rentFrom;
    this->rentTo = rentTo;
    this->rentedByMember = rentedByMember;
    this->supportedByMember = supportedByMember;
}
