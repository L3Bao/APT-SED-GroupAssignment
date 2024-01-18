#ifndef LOADER_H
#define LOADER_H

#include <map>
#include <utility>
#include <vector>

class Member;
class Skill;
class System;
class Admin;

class InputData {
    std::map<int, Member*> inputStorageMemberList;
    std::map<int, Skill*> inputStorageSkillList;
    std::vector<Admin*> inputStorageAdminList;

    void inputStorageLoadSkillListToSystem(System *system);
    void inputStorageLoadMemberListToSystem(System *system);
    void inputStorageLoadAdminListToSystem(System *system);

    void inputMemberListFromFile();
    void inputSkillListFromFile();
    void inputAdminListFromFile();
    void inputBlockedMemberListFromFile();

    void inputMemberOwnSkillFromFile();
    void inputMemberListSkillFromFile();
    void inputMemberRequestSkillFromFile();
    void inputMemberRentSkillFromFile();
    void inputMemberRatingSkillAndSupporterFromFile();
    void inputMemberRatingHostFromFile();
    void inputCompletedSessionFromFile();

public:
    InputData();

    void inputStorageFromFileList();

    void inputStorageLoadDataToSystem(System *system);
};


#endif //LOADER_H