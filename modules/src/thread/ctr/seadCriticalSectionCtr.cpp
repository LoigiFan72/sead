// Filename: seadCriticalSectionCtr.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "thread/seadCriticalSection.h"

namespace sead
{
CriticalSection::CriticalSection():
    IDisposer()
{
    
}

CriticalSection::~CriticalSection(){}

void CriticalSection::lock()
{
    mCriticalSectionInner.Enter();
}

bool CriticalSection::tryLock()
{
#if SEAD_VERSION_OLD_SDK == 1
    /* Older Versions of the CTR-SDK used TryEnterImpl(). */
    /* IF the SDK Version is less than 2.4.1 Use this. */
    mCriticalSectionInner.TryEnterImpl();
#else
    mCriticalSectionInner.TryEnter();
#endif
}

void CriticalSection::unlock()
{
    mCriticalSectionInner.Leave();
}
}// namespace sead