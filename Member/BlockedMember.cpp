#include "BlockedMember.h"
#include "Member.h"

BlockedMember::BlockedMember(int blockerID, int blockedID, bool blockView, bool blockRequestSupport) : blockerID(blockerID), blockedID(blockedID), blockView(blockView), blockRequestSupport(blockRequestSupport) {}

bool BlockedMember::isBlockView() {
    return blockView;
}

bool BlockedMember::isBlockRequestSupport(){
    return blockRequestSupport;
}

int BlockedMember::getBlockerID(){
    return blockerID;
}

int BlockedMember::getBlockedID(){
    return blockedID;
}

void BlockedMember::updateBlockSettings(bool newBlockView, bool newBlockRequestSupport) {
    this->blockView = newBlockView;
    this->blockRequestSupport = newBlockRequestSupport;
}