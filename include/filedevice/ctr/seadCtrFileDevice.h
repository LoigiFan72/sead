#pragma once

#include "filedevice/ctr/seadCtrFileStreamFileDeviceCtr.h"

namespace sead
{
class CtrFileDevice : public CtrFileStreamFileDevice
{
    SEAD_RTTI_OVERRIDE(CtrFileDevice, CtrFileStreamFileDevice)
public:
    CtrFileDevice::CtrFileDevice():
        CtrFileStreamFileDevice("ctr")
    {
    }
protected:
    virtual ~CtrFileDevice(){ }
    virtual const char* getArchiveName_() const;
};
}