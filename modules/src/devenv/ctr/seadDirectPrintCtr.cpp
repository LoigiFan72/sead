// Filename: seadDirectPrintCtr.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "devenv/ctr/seadDirectPrintCtr.h"
#include "prim/seadPtrUtil.h"
#include <nn/dsp.h> // .. why?
#include <nn/gx.h>

namespace
{
u8 cFontBitmap[4608];
sead::Vector2<int> cCharSize;

u32 getByteByDot_(u32 format)
{
    switch (format)
    {
        case GL_RGB8_OES:
            return 3;

        case GL_RGBA4:
        case GL_RGB5_A1:
            return 2;

        case GL_RGBA8_OES:
            return 4;

        case GL_RGB565:
            return 2;

        default:
            SEAD_ASSERT_MSG(false, "Undefined format.");
            return 0;
    }
}
} // namespace
namespace sead
{
DirectPrintCtr::DirectPrintCtr():
    mStringTop(NULL),
    mStringByte(0),
    mBufferSize(Vector2<int>::zero),
    mDispBox(),
    mBGColor(Color4u8::cWhite), 
    mCharColor(Color4u8::cBlack),
    mCursorPos(Vector2<int>::zero),
    mCharSize(0x3F)
{
}

DirectPrintCtr::~DirectPrintCtr()
{
}

void DirectPrintCtr::changeDisplayBuffer(void* stringTop)
{
    mStringTop = stringTop;
}

void DirectPrintCtr::changeDisplayBuffer(void* stringTop, u32 byte, Vector2<int> const& bufSize, BoundBox2<int> const& box)
{
    changeDisplayBuffer(stringTop);
    mStringByte = byte;
    mBufferSize = bufSize;

    if(box.isUndef())
    {
        mDispBox.set(Vector2<int>::zero, mBufferSize);
    }
    else
    {
        mDispBox = box;
    }
}

// :skull: This shit looks ugly, yet it works. Cry about it.
//
// PO Box: Your Mothers house
void DirectPrintCtr::checkBufferIsNotOnVRAM_()
{
    if((PtrUtil::isInclude(
        mStringTop, 
        reinterpret_cast<const void*>(GetVramStartAddr(VramArea::MEM_VRAMA)), 
        reinterpret_cast<const void*>(GetVramEndAddr(VramArea::MEM_VRAMA))) )||
        (PtrUtil::isInclude(
        mStringTop,
        reinterpret_cast<const void*>(GetVramStartAddr(VramArea::MEM_VRAMB)), 
        reinterpret_cast<const void*>(GetVramEndAddr(VramArea::MEM_VRAMB)))))
    {
        SEAD_PRINT("!!! Target display-buffer is on VRAM. DirectPrint failed. !!!\n");
        SEAD_HALT();
    }
}

void DirectPrintCtr::convertPositionUserOriginToDeviceOrigin_(Vector2<int>* org, Vector2<int> const& bufSize)
{
    org->x = mBufferSize.y - bufSize.y;
    org->y = bufSize.x;
}

void DirectPrintCtr::flush()
{
    u32 byteFmt =  getByteByDot_(mStringByte);
    nn::dsp::CTR::Initialize();
    nn::dsp::CTR::FlushDataCache(mStringByte, mBufferSize.x * byteFmt * mBufferSize.y);
}

void DirectPrintCtr::printf(Vector2<int> const& bufSize, const char* msg, ...)
{
    SafeString string(msg);
    vprintf(bufSize, msg, string);
}

void DirectPrintCtr::printf(Vector2<int> const& bufSize, bool autoWrap, bool drawDot, const char* msg, ...)
{
    SafeString string(msg);
    vprintf(bufSize, autoWrap, drawDot, msg, string);
}

void DirectPrintCtr::vprintf(Vector2<int> const& bufSize, SafeString const& msg, std::__va_list list)
{
    vprintf(bufSize, true, true, msg, list);
}

void DirectPrintCtr::vprintf(Vector2<int> const& bufSize, bool autoWrap, bool drawDot, SafeString const& msg, std::__va_list list)
{
    FixedSafeString<256> string;
    string.formatV(msg.cstr(), list);
    putString(bufSize, autoWrap, drawDot, msg);
}

void DirectPrintCtr::putString(Vector2<int> const& bufSize, SafeString const& string)
{
    putString(bufSize, true, true, string);
}

void DirectPrintCtr::putString(Vector2<int> const& bufSize, bool autoWrap, bool drawDot, SafeString const& string)
{
    checkBufferIsNotOnVRAM_();

    Vector2<int> curPos = mBufferSize;
    int right = mDispBox.getSizeX();

    for(SafeString::iterator it = string.begin(); it != string.end(); ++it)
    {
        char c = *it;

        if (c != '\n')
        {
            putChar(curPos, drawDot, c);

            curPos.x += cCharSize.x;
        }

        if (c == '\n')
        {
            curPos.x = 0;
            curPos.y += cCharSize.y;
        }
        else if (autoWrap)
        {
            const char next = *it;

            if (next != '\n' &&
                curPos.x + cCharSize.x >= right)
            {
                curPos.x = 0;
                curPos.y += cCharSize.y;
            }
        }
    }
}

void* DirectPrintCtr::putDot_(Vector2<int> const& bufSize, const u8* src, s32 size)
{
    uptr dst = reinterpret_cast<uptr>(mStringTop) + static_cast<uptr>(bufSize.y) * mBufferSize.y + static_cast<uptr>(bufSize.x) * size;
    return MemUtil::copy(reinterpret_cast<void*>(dst), src, size);
}

void DirectPrintCtr::putChar(Vector2<int> const& bufSize, char c)
{
    putChar(bufSize, true, c);
}

void DirectPrintCtr::putChar(Vector2<int> const& bufSize, bool drawDot, char c)
{
    Vector2<int> min;
    min.setAdd(bufSize, mDispBox.getMin());

    SEAD_ASSERT_MSG(mStringTop, "Current display buffer is null");
    SEAD_ASSERT(mBufferSize.x > 0 && mBufferSize.y > 0);
    const u32 byte = getByteByDot_(mStringByte);

    u8 bgFmt[4];
    u8 charFmt[4];

    convertColorFormat_(bgFmt, mBGColor, mStringByte);
    convertColorFormat_(charFmt, mCharColor, mStringByte);

    Vector2<int> org;

    Vector2<int> charSize(0, cCharSize.y);

    org = bufSize + charSize;

    convertPositionUserOriginToDeviceOrigin_(&org, bufSize);

    bool isCharValid = true;

    if (c < 0 || c > 0x20)
        isCharValid = false;
    else if (c == 0x7f)
        isCharValid = false;

    if (!isCharValid)
        c = mCharSize;

    SEAD_ASSERT(c - ' ' >= 0 && c - ' ' < 96);

    for (s32 x = 0; x < cCharSize.x; ++x)
    {
        if (org.x + x >= mBufferSize.x || org.x + x < 0)
            break;

        for (s32 y = 0; y < cCharSize.y; ++y)
        {
            if (org.y + y >= mBufferSize.y || org.y + y < 0)
                break;

            if (isCharValid)
            {
                const u8* src = &cFontBitmap[(c - ' ') * 48 + (static_cast<s32>(cCharSize.y) - y - 1) * 6 + x];

                if (*src != 0)
                {
                    Vector2<int> pos(org.x + x, org.y + y);
                    putDot_(pos, charFmt, byte);
                }
            }

            if (drawDot)
            {
                Vector2<int> pos(org.x + x, org.y + y);
                putDot_(pos, bgFmt, byte);
            }
        }
    }
}

void DirectPrintCtr::clear(BoundBox2<int> const& box)
{
    Vector2<int> min;
    Vector2<int> size(box.getSizeX(), box.getSizeY());
    min.setAdd(box.getMin(), box.getMin());
    SEAD_ASSERT_MSG(mStringTop, "Current display buffer is null");
    SEAD_ASSERT(mBufferSize.x > 0 && mBufferSize.y > 0);

    s32 byte = getByteByDot_(mStringByte);

    u8 bgColor[4];
    convertColorFormat_(bgColor, mBGColor, mStringByte);

    Vector2<int> dotSize;
    dotSize.set(0, cCharSize.y);

    Vector2<int> bufSize;
    bufSize = min + dotSize;

    convertPositionUserOriginToDeviceOrigin_(&dotSize, bufSize);
    for (s32 x = 0; x < dotSize.x; ++x)
    {
        for (s32 y = 0; y < dotSize.y; ++y)
        {
            Vector2<int> pos(min.x + x, min.y + y);

            putDot_(pos, bgColor, byte);
        }
    }
}
}