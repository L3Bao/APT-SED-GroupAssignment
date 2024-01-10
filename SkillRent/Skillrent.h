#ifndef SKILLRENT_H
#define SKILLRENT_H

class DateTime;
class Member;

class SkillRent {
    DateTime *rentFrom;
    DateTime *rentTo;
    Member *rentedByMember;

public:
    SkillRent(DateTime *rentFromDate, DateTime *rentToDate, Member *rentedByMember);

    friend class System;

    friend class Member;

    friend class Skill;

    friend class OutputData;
};


#endif //SKILLRENT_H
