// Filename: seadRawPrint.cpp
//
// Project: StandardEAD C++ Library for CTR

#include <basis/seadRawPrint.h>
#include "devenv/seadAssertConfig.h"
#include <nn/svc.h>
#include <nn/dbg.h>
#include <stdio.h>

namespace sead { 
namespace system {

void PrintStringImpl(const char* string, s32 len)
{
    if(0 < len)
    {
        PutString(string);
    }
}

void Print(const char* format, ...)
{
    va_list list;
    va_start(list, format);

    PrintV(format, list);

    va_end(list);
}

void PrintV(const char* format, va_list list)
{
    char buf[256];
    int len = vsnprintf(buf, sizeof(buf), format, list);

    if (256 < len) {
        len = 256;
    }

    PrintString(buf, len);
}
} 
}