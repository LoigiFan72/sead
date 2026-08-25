// Filename: seadCtrFileStreamFileDeviceCtr.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "filedevice/ctr/seadCtrFileStreamFileDeviceCtr.h"

namespace sead
{

CtrFileStreamFileDevice::CtrFileStreamFileDevice(const SafeString& name):
    FileDevice(name)
{

}

bool CtrFileStreamFileDevice::doIsAvailable_() const
{
    nn::fs::Directory dir;
    SafeString slash("/");

    s32 error = doGetLastRawError_();

    //SafeString err(error);

}

Result CtrFileStreamFileDevice::openDirectryImpl_(nn::fs::Directory* dir, SafeString const& pathInner, SafeString const& pathOutter)
{

}
}
