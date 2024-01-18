#ifndef MEMBERRENT_H
#define MEMBERRENT_H

class DateTime;
class Skill;

class MemberRent {
    DateTime *rentFrom;
    DateTime *rentTo;
    Skill *rentSkill;

public:
    MemberRent(DateTime *rentFrom, DateTime *rentTo, Skill *rentSkill);

    friend class Member;

    friend class System;

    friend class OutputData;

    friend class InputData;
};


#endif 
