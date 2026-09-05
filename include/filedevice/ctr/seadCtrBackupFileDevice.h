#pragma once

#include "filedevice/ctr/seadCtrFileStreamFileDeviceCtr.h"

namespace sead
{
class CtrBackupFileDevice : public CtrFileStreamFileDevice
{
    SEAD_RTTI_OVERRIDE(CtrBackupFileDevice, CtrFileStreamFileDevice)
public:
    CtrBackupFileDevice():
        CtrFileStreamFileDevice("backup")
    {
    }
protected:

    virtual ~CtrBackupFileDevice(){ }

    virtual const char* getArchiveName_() const;
};
}