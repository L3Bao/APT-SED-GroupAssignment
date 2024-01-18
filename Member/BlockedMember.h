#ifndef BLOCKEDMEMBER_H
#define BLOCKEDMEMBER_H

#include <string>
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

    void updateBlockSettings(bool newBlockView, bool newBlockRequestSupport);

    friend class Member;
    friend class System;

    friend class OutputData;
    friend class InputData;
};

#endif