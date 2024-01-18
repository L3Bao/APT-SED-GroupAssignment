#ifndef BLOCKEDMEMBER_H
#define BLOCKEDMEMBER_H

#include <string>

struct BlockKey {
    int blockerID;
    int blockedID;

    BlockKey(int blocker, int blocked) : blockerID(blocker), blockedID(blocked) {}

    bool operator<(BlockKey& other) {
        return std::tie(blockerID, blockedID) < std::tie(other.blockerID, other.blockedID);
    }
};

class BlockedMember {
    int blockerID;
    int blockedID;
    bool blockView;
    bool blockRequestSupport;
public:
    BlockedMember(int blockerID, int blockedID, bool blockView, bool blockRequestSupport);


    int getBlockerID();
    int getBlockedID();
    bool isBlockView();
    bool isBlockRequestSupport();

    friend class Member;
    friend class System;

    friend class OutputData;
    friend class InputData;
};

#endif