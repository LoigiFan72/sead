#pragma once

#include "filedevice/ctr/seadCtrFileDevice.h"

namespace sead
{
class CtrBackupFileDevice : public CtrFileDevice
{
    SEAD_RTTI_OVERRIDE(CtrBackupFileDevice, CtrFileDevice)
public:
    CtrBackupFileDevice(const SafeString& name){ }
protected:

    virtual ~CtrBackupFileDevice(){ }

    virtual const char* getArchiveName_() const;
};
}