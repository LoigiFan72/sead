#pragma once

#include <controller/seadController.h>
#include <controller/SeadControlDevice.h>
#include <nn/hid.h>

namespace sead
{
class CtrHidDevice : public ControlDevice
{
    SEAD_RTTI_OVERRIDE(CtrHidDevice, ControlDevice)
public:
    CtrHidDevice(ControllerMgr* mgr);
    virtual ~CtrHidDevice() { }
    virtual void calc();

    PadReader mMainPad;
    TouchPanelReader mMainTouchPanel;
    GyroscopeReader mMainGyroscope;

    PadReader* mPadReaderPtr;
    PadStatus mPadStatus;
    TouchPanelReader* mTouchPanelReaderPtr;
    TouchPanelStatus mTouchPanelStatus;
    AccelerometerReader* mAccelerometerReaderPtr;
    AccelerometerStatus mAccelerometerStatus;
    AccelerationFloat mAccelerometerFloat;
    GyroscopeReader* mGyroscopeReaderPtr;
    GyroscopeStatus mGyroscopeStatus;
#ifdef SEAD_DEBUG
    DebugPadReader* mDebugPadReaderPtr;
    DebugPadStatus mDebugPadStatus;
#endif
    u32 mFlags;
};
}