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

    SafeString outter;
    doGetLastRawError_();
    SafeString inner;
    error_result = openDirectryImpl_(&dir, inner, outter);
    return error_result.IsSuccess();
}

nn::Result CtrFileStreamFileDevice::openDirectryImpl_(nn::fs::Directory* dr, SafeString const& pathInner, SafeString const& pathOutter)
{
    SEAD_ASSERT(dr);
    WFixedSafeString<256> sstring;
    s32 len = sstring.format(L"%s:%s", pathInner.cstr(), pathOutter.cstr());
    SEAD_ASSERT(len < cFileNameFormatBufSize - 1);
    return dr->TryInitialize(sstring.cstr());
}
}
