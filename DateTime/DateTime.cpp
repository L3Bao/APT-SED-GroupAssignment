#include "DateTime.h"
#include <sstream>
#include <iomanip>

DateTime::DateTime(int hour, int minute) {
    this->hour = hour;
    this->minute = minute;
}

bool DateTime::operator<(DateTime &other) {
    // Compare the hours
    if (this -> hour < other.hour) {
        return true;
    } else if (this -> hour > other.hour) {
        return false;
    } else {
        return this -> minute < other.minute;
    }
}

bool DateTime::operator>(DateTime &other) {
    // Compare the hours
    if (this -> hour > other.hour) {
        return true;
    } else if (this -> hour < other.hour) {
        return false;
    } else {
        return this -> minute > other.minute;
    }
}

bool DateTime::operator<=(DateTime &other)  {
    if (hour < other.hour || (hour == other.hour && minute <= other.minute)) {
        return true;
    }
    return false;
}


bool DateTime::operator>=(DateTime &other)  {
    if (hour > other.hour || (hour == other.hour && minute >= other.minute)) {
        return true;
    }
    return false;
}
 
int DateTime::operator-(DateTime &other) {
    // Convert the time to minutes
    int thisTime = this -> hour * 60 + this -> minute;
    int otherTime = other.hour * 60 + other.minute;

    // Return the difference
    return abs(thisTime - otherTime);
}

std::string DateTime::toString() {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hour << ":" << std::setfill('0') << std::setw(2) << minute;
    return ss.str();
}