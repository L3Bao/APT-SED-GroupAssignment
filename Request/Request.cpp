#include "Request.h"

Request::Request(DateTime *requestFrom, DateTime *requestTo, Member *requestedByMember) {
    this->requestFrom = requestFrom;
    this->requestTo = requestTo;
    this->requestedByMember = requestedByMember;
}