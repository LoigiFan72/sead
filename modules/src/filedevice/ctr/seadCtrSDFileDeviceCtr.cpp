// Filename: seadCtrSDFileDeviceCtr.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "filedevice/ctr/seadCtrSDFileDeviceCtr.h"

namespace sead
{
bool CtrSDFileDevice::doIsExistFile_(bool* exists, const SafeString& path)
{
    nn::fs::Directory dir;
    {
        doGetLastRawError_();
        SafeString inner;
        error_result = openDirectryImpl_(&dir, inner, path);
    }
    if(error_result.IsSuccess())
    {
        exists = NULL;
        return true;
    }
    if(Result::ConstRange<Result::Level::LEVEL_STATUS, Result::Summary::SUMMARY_NOT_FOUND, Result::Module::MODULE_NN_FS, 100, 100, 179>::Includes(error_result))
    {
        FileStream fstream;
        {
            doGetLastRawError_();
            SafeString inner;
            error_result = openFileStreanImpl_(&fstream, inner, path);
        }
        if(error_result.IsSuccess())
        {
            *exists = true;
            return true;
        }
    }
    exists = NULL;
    return false;
}

const char* CtrSDFileDevice::getArchiveName_() const
{
    return "sdmc";
}
}