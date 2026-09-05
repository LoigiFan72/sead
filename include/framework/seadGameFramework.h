#pragma once

#include <framework/seadFramework.h>
#include <heap/seadHeap.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadSafeString.h>

#include <nw/ut.h>

namespace sead
{
class GameFramework : public Framework
{
    SEAD_RTTI_OVERRIDE(GameFramework, Framework);

public:
    static void initialize(const Framework::InitializeArg&);

    GameFramework();
    
    virtual ~GameFramework();
    virtual void createSystemTasks(TaskBase* base, 
        const Framework::CreateSystemTaskArg& createSystemTaskArg);
    virtual void quitRun_(Heap* heap);
    virtual void createControllerMgr(TaskBase* base);
    virtual void createHostIOMgr(TaskBase* base, HostIOMgr::Parameter* hostioParam, Heap* heap);
    virtual void createProcessMeter(TaskBase* base);
    virtual void createSeadMenuMgr(TaskBase* base);
    virtual void createInfLoopChecker(TaskBase* base, const TickSpan&, int);
    virtual void createCuckooClock(TaskBase* base);
    virtual float calcFps() = 0;
    virtual void saveScreenShot(const SafeString&) {}
    virtual bool isScreenShotBusy() const { return false; }
    virtual void waitStartDisplayLoop_();

    void startDisplay();
    void lockFrameDrawContext();
    void unlockFrameDrawContext();

private:
    int mDisplayState;
    MultiProcessMeterBar<32> mCalcMeter;
    MultiProcessMeterBar<32> mDrawMeter;
    MultiProcessMeterBar<32> mGPUMeter;
    InfLoopCheckerThread* mCheckerThread;
    Graphics::LockFunc mFrameLockFunc;
    ProcDrawCallback mProcDrawCallback;
};
}  // namespace sead
