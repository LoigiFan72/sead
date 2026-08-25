#pragma once

#include "filedevice/ctr/seadCtrFileStreamFileDeviceCtr.h"

namespace sead
{
class CtrSDFileDevice: public CtrFileStreamFileDevice
{
    SEAD_RTTI_OVERRIDE(CtrFileStreamFileDevice, CtrFileStreamFileDevice)
public:
    CtrSDFileDevice();
protected:

};
}