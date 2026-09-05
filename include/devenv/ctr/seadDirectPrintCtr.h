#pragma once

#include <math/seadVector.h>
#include <math/seadBoundBox.h>
#include <gfx/seadColor.h>
#include <prim/seadSafeString.h>

#include <cstdlib>

namespace sead
{
class DirectPrintCtr
{
public:
    DirectPrintCtr();
    virtual ~DirectPrintCtr();

    void changeDisplayBuffer(void* stringTop);
    void changeDisplayBuffer(void* stringTop, u32 byte, Vector2<int> const& bufSize, BoundBox2<int> const& box);
    void checkBufferIsNotOnVRAM_();
    void clear(BoundBox2<int> const& box);
    void convertColorFormat_(unsigned char* fmt, Color4u8 const& color, u32 byte);
    void convertPositionUserOriginToDeviceOrigin_(Vector2<int>* org, Vector2<int> const& bufSize);
    void flush();
    void printf(Vector2<int> const& bufSize, const char* msg, ...);
    void printf(Vector2<int> const& bufSize, bool autoWrap, bool drawDot, const char* msg,  ...);
    void putChar(Vector2<int> const& bufSize, bool drawDot, char c);
    void putChar(Vector2<int> const& bufSize, char c);
    void* putDot_(Vector2<int> const& bufSize, const u8* src, s32 size);

    void putString(Vector2<int> const& bufSize, bool autoWrap, bool drawDot, SafeString const& string);
    void putString(Vector2<int> const& bufSize, SafeString const& string);

    void vprintf(Vector2<int> const& bufSize, SafeString const& msg, std::__va_list list);
    void vprintf(Vector2<int> const& bufSize, bool autoWrap, bool drawDot, SafeString const& msg, std::__va_list list);

    BoundBox2<int> getBoundBox() const{ return mDispBox; }
    Color4u8 getBGColor() const{ return mBGColor; }
    Color4u8 getCharColor() const{ return mCharColor; }
    Vector2<int> getLastCursorPos() const{ return mCursorPos; }
    void setCharColor(const Color4u8& color){ mCharColor = color; }
private:
    void* mStringTop;
    u32 mStringByte;
    Vector2<int> mBufferSize;
    BoundBox2<int> mDispBox;
    Color4u8 mBGColor;
    Color4u8 mCharColor;
    Vector2<int> mCursorPos;
    u8 mCharSize;
};
}