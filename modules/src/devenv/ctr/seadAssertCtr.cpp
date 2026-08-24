// Filename: seadAssertCtr.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "devenv/seadAssertConfig.h"
#include "prim/seadMemUtil.h"

#include <stdio.h>
#include <nn/dbg.h>

namespace sead{
namespace system{

char tmp[0x800];

void Halt()
{
    nn::dbg::Break();
}

void HaltWithDetail(const char* file, int lineNo, const char* fmt, ...)
{
    va_list list;
    va_start(list, fmt);
    MemUtil::fillZero(&tmp, sizeof(tmp));

    snprintf(tmp, sizeof(tmp), "\n//================= PROGRAM HALT ==================//\nSource File: %s\nLine Number: %d\nDescription: " file, lineNo);
    vsnprintf(tmp, sizeof(tmp), fmt, list);
    snprintf(tmp, sizeof(tmp), "\n//=================================================//\n");

    va_end(list);

    // TODO

    Halt();
}

void HaltWithDetailNoFormat(const char* pos, s32 line, const char* str)
{
    HaltWithDetail(pos, line, "%s", str);
}

}
}