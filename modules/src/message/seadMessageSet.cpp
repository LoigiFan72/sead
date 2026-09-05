// Filename: seadMessageSet.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "message/seadMessageSet.h"
#include "basis/seadNew.h"

#include <LMS/libms.h>

namespace sead
{

Heap* MessageSetBase::sHeap = NULL;

MessageSetBase::~MessageSetBase()
{
}

s32 MessageSetBase::initialize(void* data, Heap* heap)
{
    SEAD_ASSERT(mMsgFile == nullptr);
    SEAD_ASSERT(sHeap == nullptr);

    sHeap = heap;

    /* Setup libms */
    LMS_SetMemFuncs(allocForLibms_, freeForLibms_);

    mMsgFile = LMS_InitMessage(data);

    s32 msgNum = LMS_GetTextNum(mMsgFile);

    if(msgNum < 0)
    {
        SEAD_ASSERT_MSG(false, "failed to init message.[%d]", msgNum);
        LMS_CloseMessage(mMsgFile);
        mMsgFile = NULL;
        mTextNum = 0;
    }
    else
    {
        mTextNum = 0;
    }

    LMS_SetMemFuncs(0,0);
    sHeap = NULL;
    return msgNum >> 0x1F + 1;
}

void MessageSetBase::finalize()
{
    SEAD_ASSERT(mMsgFile);
    LMS_SetMemFuncs(0, freeForLibms_);
    LMS_CloseMessage(mMsgFile);
    mMsgFile = NULL;
    mTextNum = 0;
    LMS_SetMemFuncs(0, 0);
}

void* MessageSetBase::allocForLibms_(u32 size)
{
    return new(size, sHeap)[];
}

void MessageSetBase::freeForLibms_(void* ptr)
{
    return new(ptr, sHeap)[];
}
}