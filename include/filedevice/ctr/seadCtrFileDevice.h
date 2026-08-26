#pragma once

#include "filedevice/ctr/seadCtrFileStreamFileDeviceCtr.h"

namespace sead
{
class CtrFileDevice : public FileDevice
{
    SEAD_RTTI_OVERRIDE(CtrFileDevice, FileDevice)
public:
    CtrFileDevice(){ }
protected:
    virtual ~CtrFileDevice(){ }
    virtual const char* getArchiveName_() const;
}
}