#ifndef REQUEST_H
#define REQUEST_H

#include "../DateTime/DateTime.h"

class Member;
class DateTime;

class Request {
    DateTime* requestFrom;
    DateTime* requestTo;
    Member* requestedByMember;

public:
    Request(DateTime *requestFrom, DateTime *requestTo, Member *requestedByMember);

    friend class System;

    friend class Member;

    friend class OutputData;
};


#endif //TEST_REQUEST_H
