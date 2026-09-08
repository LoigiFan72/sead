#pragma once

#include "stream/seadStream.h"

namespace sead
{
class BitmapBuilder
{
public:
    BitmapBuilder(WriteStream* stream, s32 width, s32 height);
    virtual ~BitmapBuilder();

    void writeInfoHeader();
    void writeFileHeader();

    void writeImageLineA1BGR5(void* ptr, u32 height);
    void writeImageLineA1RGB5(void* ptr, u32 height);

    void writeImageLineABGR4(void* ptr, u32 height);
    void writeImageLineABGR8(void* ptr, u32 height);

    void writeImageLineBGR565(void* ptr, u32 height);
    void writeImageLineBGR8(void* ptr, u32 height);

    void writeImageLineRGB565(void* ptr, u32 height);
    void writeImageLineRGB8(void* ptr, u32 height);

    void writeImageLineRGBA4(void* ptr, u32 height);
    void writeImageLineRGBA8(void* ptr, u32 height);
private:
    WriteStream* mWriteStream;
    s32 mWidth;
    s32 mHeight;
};
}