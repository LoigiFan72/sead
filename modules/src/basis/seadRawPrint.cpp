// Filename: seadRawPrint.cpp
//
// Project: StandardEAD C++ Library for CTR

#include <basis/seadRawPrint.h>
#include <nn/svc.h>
#include <nn/dbg.h>
#include <stdio.h>

namespace sead { 
namespace system {

void PrintString(const char* str, s32 len){

}

void PrintStringImpl(const char* string, s32 len){
    if(0 < len){
        PutString(string);
    }
}

void Print(const char* format, ...){

}

void PrintV(const char* format, va_list list){

}

void Halt(){
    nndbgBreak(BREAK_REASON_PANIC);
    while(true);
}

void HaltWithDetail(const char* pos, s32 line, const char* format, ...){
    va_list list;
    va_start(list, format);

    sead::system::Print("\n//================= PROGRAM HALT ==================//\nSource File: %s\nLine Number: %d\nDescription: ", pos, line);
    sead::system::PrintV(format, list);
    sead::system::Print("\n//=================================================//\n");

    va_end(list);

    nndbgBreak(BREAK_REASON_PANIC);
    while(true);
}

void HaltWithDetailNoFormat(const char* pos, s32 line, const char* str){
    HaltWithDetail(pos, line, "%s", str);
}

} 
}