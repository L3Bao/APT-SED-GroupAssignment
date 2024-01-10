#include "MemberRent.h"

MemberRent::MemberRent(DateTime *rentFrom, DateTime *rentTo, Skill *rentSkill) {
    this->rentFrom = rentFrom;
    this->rentTo = rentTo;
    this->rentSkill = rentSkill;
}