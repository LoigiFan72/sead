// Filename: seadArenaCtr.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "heap/seadArena.h"
#include "basis/seadRawPrint.h"
#include <nn/os.h>

namespace sead
{
Arena::Arena():
    mStart(NULL), mSize(0), mInitWithStartAddress(false)
{
}

Arena::~Arena()
{
}

void Arena::initialize(size_t size)
{
    SEAD_ASSERT_MSG(mStart, "initialize twice");
    SEAD_ASSERT_MSG(sIsUsingDeviceMemory, "already using DeviceMemory");
    sIsUsingDeviceMemory = true;
    mSize = size;
    mStart = reinterpret_cast<u8*>(nn::os::GetDeviceMemoryAddress());
}

void Arena::destroy()
{
    SEAD_ASSERT_MSG(mStart, "not intialized");
    mSize = 0;
    mStart = NULL;
    sIsUsingDeviceMemory = false;
    mInitWithStartAddress = false;
}
}