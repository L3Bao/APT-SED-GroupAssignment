#ifndef SAVER_H
#define SAVER_H

#include <vector>
#include <utility>
#include "../Skill/Skill.h"
#include "../Member/Member.h"


class Member;
class Skill;
class System;

class OutputData {
    std::vector<Member*> outputStorageMemberList;
    std::vector<Skill*> outputStorageSkillList;

    void outputStorageLoadMemberListFromSystem(System *system);
    void outputStorageLoadSkillListFromSystem(System *system);

    //All output to file function
    void outputMemberListToFile();
    void outputSkillListToFile();
    void outputMemberOwnSkillToFile();
    void outputMemberListSkillToFile();
    void outputMemberRequestSkillToFile();
    void outputMemberRentSkillToFile();
    void outputMemberRatingSkillToFile();

    void outputMemberRatingRenterToFile();
    void outputSkillUnratedRenterToFile();

public:
    OutputData();

    void outputStorageToFileList();

    void outputStorageLoadDataFromSystem(System *system);
};




#endif //TEST_OutputData_H
