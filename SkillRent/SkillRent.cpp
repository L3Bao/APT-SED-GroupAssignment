#include "SkillRent.h"

SkillRent::SkillRent(DateTime *rentFrom, DateTime *rentTo, Member *rentedByMember) {
    this->rentFrom = rentFrom;
    this->rentTo = rentTo;
    this->rentedByMember = rentedByMember;
}

SkillRent::SkillRent(DateTime *rentFromDate, DateTime *rentToDate, Member *supportedByMember, bool isSupporter) {
    this->rentFrom = rentFrom;
    this->rentTo = rentTo;
    this->supportedByMember = supportedByMember;
    this->isSupporter = true;
}
