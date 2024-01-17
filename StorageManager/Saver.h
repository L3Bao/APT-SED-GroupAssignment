#ifndef InputData_H
#define InputData_H

#include <map>
#include <utility>
#include <vector>

class Member;
class Skill;
class System;
class Admin;

class InputData {
    std::map<int, Member*> inputStorageMemberList;
    std::map<int, Skill*> inputStorageSKILLList;
    std::vector<Admin*> inputStorageAdminList;

    void inputStorageLoadSKILLListToSystem(System *system);
    void inputStorageLoadMemberListToSystem(System *system);
    void inputStorageLoadAdminListToSystem(System *system);

    void inputMemberListFromFile();
    void inputSKILLListFromFile();
    void inputAdminListFromFile();

    void inputMemberOwnSKILLFromFile();
    void inputMemberListSKILLFromFile();
    void inputMemberRequestSKILLFromFile();
    void inputMemberRentSKILLFromFile();
    void inputMemberReviewSKILLFromFile();
    void inputMemberReviewRenterFromFile();
    void inputSKILLUnratedRenterFromFile();

public:
    InputData();

    void inputStorageFromFileList();

    void inputStorageLoadDataToSystem(System *system);
};


#endif //TEST_InputData_H