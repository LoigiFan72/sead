#pragma once

#include <nn/fs.h>
#include "filedevice/seadFileDevice.h"
#include "prim/seadSafeString.h"

namespace sead
{
class CtrFileStreamFileDevice : public FileDevice
{
    SEAD_RTTI_OVERRIDE(CtrFileStreamFileDevice, FileDevice)
public:
    CtrFileStreamFileDevice(const SafeString& name);
protected:

    virtual ~CtrFileStreamFileDevice(){ }
    virtual bool doIsAvailable_() const;
    virtual FileDevice* doOpen_(FileHandle* handle, const SafeString& path, FileOpenFlag flag);
    virtual bool doClose_(FileHandle* handle);
    virtual bool doFlush_(FileHandle* handle);
    virtual bool doRemove_(const SafeString& path);
    virtual bool doRead_(u32* bytesRead, FileHandle* handle, u8* outBuffer, u32 bytesToRead);
    virtual bool doWrite_(u32* bytesWritten, FileHandle* handle, const u8* inBuffer,
                  u32 bytesToWrite);
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin);
    virtual bool doGetCurrentSeekPos_(u32* seekPos, FileHandle* handle);
    virtual bool doGetFileSize_(u32* fileSize, const SafeString& path);
    virtual bool doGetFileSize_(u32* fileSize, FileHandle* handle);
    virtual bool doIsExistFile_(bool* exists, const SafeString& path);
    virtual bool doIsExistDirectory_(bool* exists, const SafeString& path);
    virtual FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& path);
    virtual bool doCloseDirectory_(DirectoryHandle* handle);
    virtual bool doReadDirectory_(u32* entriesRead, DirectoryHandle* handle, DirectoryEntry* entries,
                          u32 entriesToRead);
    virtual bool doMakeDirectory_(const SafeString& path, u32 u_32);
    virtual s32 doGetLastRawError_() const { return error_result.GetValue(); }
    virtual void doResolvePath_(BufferedSafeString* out, const SafeString& path) const;

    nn::Result openDirectryImpl_(nn::fs::Directory* dir, SafeString const& pathInner, SafeString const& pathOutter);
    nn::Result openFileStreanImpl_(nn::fs::FileStream* strm, SafeString const& pathInner, SafeString const& pathOutter);

    nn::Result error_result;
    SafeString mMountPoint;
};
}