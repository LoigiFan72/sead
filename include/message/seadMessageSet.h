#pragma once

#include <LMS/msgfile.h>

#include "heap/seadHeap.h"

namespace sead
{
class MessageSetBase
{
public:
    virtual ~MessageSetBase();
    s32 initialize(void* data, Heap* heap);
    void finalize();
    static void* allocForLibms_(u32 size);
    static void freeForLibms_(void* ptr);

    LMS_MessageBinary* mMsgFile;
    s32 mTextNum;

    static Heap* sHeap;
};
} // namespace sead