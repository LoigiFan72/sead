// Filename: seadCtrHidDeviceCtr.cpp
//
// Project: StandardEAD C++ Library for CTR

#include <controller/ctr/seadCtrHidDeviceCtr.h>

namespace sead
{
/*
Pad to align for Line nos for assert


*/
CtrHidDevice::CtrHidDevice(ControllerMgr* mgr):
    ControlDevice(mgr),
    mPadReaderPtr(NULL),
    mTouchPanelReaderPtr(NULL),
    mAccelerometerReaderPtr(NULL),
    mGyroscopeReaderPtr(NULL),
#ifdef SEAD_DEBUG
    mDebugPadReaderPtr(NULL),
#endif
    mFlags(0)
{
    nn::Result result = nn::hid::CTR::Initialize();

    SEAD_ASSERT(result.IsSuccess() || result.GetDescription() == nn::Result::DESCRIPTION_ALREADY_INITIALIZED);
    mPadReaderPtr = new (&mMainPad) nn::hid::CTR::PadReader(nn::hid::CTR::GetPad());

    mTouchPanelReaderPtr = new(&mMainTouchPanel) nn::hid::CTR::TouchPanelReader(nn::hid::CTR::GetTouchPanel());
}

void CtrHidDevice::calc()
{
    if (mPadReaderPtr != nullptr)
    {
        if (mPadReaderPtr->ReadLatest(&mPadStatus))
        {
            mFlags |= nn::hid::CTR::BUTTON_A;
        }
        else
        {
            mFlags &= nn::hid::CTR::BUTTON_A;
        }
    }

    if (mTouchPanelReaderPtr != nullptr)
    {
        if (mTouchPanelReaderPtr->ReadLatest(&mTouchPanelStatus))
        {
            mFlags |= nn::hid::CTR::BUTTON_B;
        }
        else
        {
            mFlags &= ~nn::hid::CTR::BUTTON_B;
        }
    }
    else
    {
        mFlags &= ~nn::hid::CTR::BUTTON_B;
    }

    if (mAccelerometerReaderPtr != nullptr)
    {
        if (mAccelerometerReaderPtr->ReadLatest(&mAccelerometerStatus))
        {
            mFlags |= nn::hid::CTR::PSEUDO_BUTTON_SELECT;

            mAccelerometerReaderPtr->ConvertToAcceleration(&mAccelerometerFloat, 1, &mAccelerometerStatus);
        }
        else
        {
            mFlags &= ~nn::hid::CTR::PSEUDO_BUTTON_SELECT;
        }
    }
    else
    {
        mFlags &= ~nn::hid::CTR::PSEUDO_BUTTON_SELECT;
    }

    if (mGyroscopeReaderPtr != nullptr)
    {
        if (mGyroscopeReaderPtr->ReadLatest(&mGyroscopeStatus))
        {
            mFlags |= nn::hid::CTR::BUTTON_START;
        }
        else
        {
            mFlags &= ~nn::hid::CTR::BUTTON_START;
        }
    }
    else
    {
        mFlags &= ~nn::hid::CTR::BUTTON_START;
    }
#ifdef SEAD_DEBUG
    if (mDebugPadReaderPtr != nullptr)
    {
        if (mDebugPadReaderPtr->ReadLatest(&mDebugPadStatus))
        {
            mFlags |= nn::hid::CTR::DEBUG_PAD_BUTTON_A;
        }
        else
        {
            mFlags &= ~nn::hid::CTR::DEBUG_PAD_BUTTON_A;
        }
    }
    else
    {
        mFlags &= ~nn::hid::CTR::DEBUG_PAD_BUTTON_A;
    }
#endif
}

}