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
        nn_result = openDirectryImpl_(&dir, inner, path);
    }
    if(nn_result.IsSuccess())
    {
        exists = NULL;
        return true;
    }
    if(Result::ConstRange<Result::Level::LEVEL_STATUS, Result::Summary::SUMMARY_NOT_FOUND, Result::Module::MODULE_NN_FS, 100, 100, 179>::Includes(nn_result))
    {
        FileStream fstream;
        {
            doGetLastRawError_();
            SafeString inner;
            nn_result = openFileStreamImpl_(&fstream, inner, path, nn::fs::OPEN_MODE_READ);
        }
        if(nn_result.IsSuccess())
        {
            *exists = true;
            return true;
        }
        if(Result::ConstRange<Result::Level::LEVEL_STATUS, Result::Summary::SUMMARY_NOT_FOUND, Result::Module::MODULE_NN_FS, 100, 100, 179>::Includes(nn_result))
        {
            *exists = false;
            return true;
        }
    }
    
    *exists = false;
    return false;
}

const char* CtrSDFileDevice::getArchiveName_() const
{
    return "sdmc";
}
}