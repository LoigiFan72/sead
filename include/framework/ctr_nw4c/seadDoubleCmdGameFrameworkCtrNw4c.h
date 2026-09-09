#pragma once

#include "framework/ctr_nw4c/seadGameFrameworkCtrNw4c.h"

namespace sead
{
class DoubleCmdGameFrameworkCtrNw4c : public GameFrameworkCtrNw4c
{
    SEAD_RTTI_OVERRIDE(DoubleCmdGameFrameworkCtrNw4c, GameFrameworkCtrNw4c)

public:
    static void initialize(const Framework::InitializeArg& arg);
    void initializeGraphicsSystem(Heap* heap, const Vector2f& topFbSize, const Vector2f& btmFbSize);

    explicit DoubleCmdGameFrameworkCtrNw4c(const CreateArg& arg);

    virtual ~DoubleCmdGameFrameworkCtrNw4c();

    static void cmdlistCallback(GLint list);
protected:
    virtual void mainLoop_();
    virtual void procFrame_();
    virtual void presentTop_();
    virtual void presentBtm_();
    virtual void swapBuffer_();
    virtual void waitForVBlank_();
    virtual void doScreenShotImpl_(const char* shot);
    virtual void cmdlistCallbackImpl_(s32 list);

    static DoubleCmdGameFrameworkCtrNw4c* sInstance;
protected:
    s32 _2068; // 0x2068
    u32 mDoubleBuf[2]; // 0x206C
    GLuint mDoubleBufferSizeTop[3]; // 0x2074
    GLuint mDoubleBufferSizeBtm[3]; // 0x2080
    u32 mDoubleDispBufList; // 0x208C
    u32 mDoubleDispBufState; // 0x2090
    u32 _2094;
    TickTime mLastDoubleTick; // 0x2098
    u8 mProcessMeterBar; // 0x20A0
    bool mWaitCmdlistDone; // 0x20A1
    bool mLastCmdlistDone; // 0x20A2
    bool mWaitForVBlink; // 0x20A3
    GLint mDoubleCmdParam[3]; // 0x20A4;
};

}