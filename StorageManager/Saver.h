#ifndef SAVER_H
#define SAVER_H

#include <vector>
#include <utility>


class Member;
class Skill;
class System;
class BlockedMember;

class OutputData {
    std::vector<Member*> outputStorageMemberList;
    std::vector<Skill*> outputStorageSkillList;
    std::vector<BlockedMember*> outputStorageBlockedMemberList;

    void outputStorageLoadMemberListFromSystem(System *system);
    void outputStorageLoadSkillListFromSystem(System *system);
    void outputStorageBlockedMemberListFromSystem(System *system);

    //All output to file function
    void outputMemberListToFile();
    void outputSkillListToFile();
    void outputMemberOwnSkillToFile();
    void outputMemberListSkillToFile();
    void outputMemberRequestSkillToFile();
    void outputMemberRentSkillToFile();
    void outputMemberRatingSkillAndSupporterToFile();

    void outputMemberRatingHostToFile();
    void outputCompletedSessionListToFile();
    void outputBlockedMemberToFile();

public:
    OutputData();

    void outputStorageToFileList();

    void outputStorageLoadDataFromSystem(System *system);

    void updateMemInfotoFile(std::vector<Member*> memberList);

};




#endif //TEST_OutputData_H
