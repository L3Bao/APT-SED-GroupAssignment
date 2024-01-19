#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

#include "Admin\Admin.h"
#include "DateTime\DateTime.h"
#include "Member\Member.h"
#include "MemberRent\MemberRent.h"
#include "Middleware\Middleware.h"
#include "Rating\Rating.h"
#include "Request\Request.h"
#include "Skill\Skill.h"
#include "SkillRent\SkillRent.h"
#include "System\System.h"
#include "User\User.h"
#include "StorageManager\Loader.h"
#include "StorageManager\Saver.h"
using std::cout;

class User;
class Request;
class Rating;
class SkillRent;
class MemberRent;
class Skill;
class Member;
class System;

int main() {
    cout << "\n\nEEET2482/COSC2082 ASSIGNMENT\n"
        << "\"TIME BANK\" APPLICATION\n\n" 
        << "Instructor: Mr. Tran Nhat Linh\n"
        << "\tGroup: 10\n"
        << "1.\ts3986287, Nguyen Vinh Gia Bao\n"
        << "2.\ts3979654, Le Viet Bao\n"
        << "3.\ts3974929, Truong Phung Tan Tai\n"
        << "4.\ts3822040, Tran Phu Van\n";

    InputData inputStorageManager;
    System system;

    inputStorageManager.inputStorageFromFileList();
    inputStorageManager.inputStorageLoadDataToSystem(&system);

    system.mainMenu();
    return 0;
}