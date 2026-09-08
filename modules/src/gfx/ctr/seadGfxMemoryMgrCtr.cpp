// Filename: seadGfxMemoryMgrCtr.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "gfx/ctr/seadGfxMemoryMgrCtr.h"
#include <nn/gx.h>

namespace sead
{
GfxMemoryMgrCtr::GfxMemoryMgrCtr():
    mIsInitialized(false)
{
}

DefaultGfxMemoryMgrCtr(Heap* heap):
    GfxMemoryMgrCtr(),
    mGfxHeap(heap),
    mMemVramAStart(0),
    mMemVramBStart(0),
    _1C(0),
    _20(0)
{
    mMemVramAStart = GetVramStartAddr(MEM_VRAMA);
    mMemVramBStart = GetVramStartAddr(MEM_VRAMB);
    mMemVramAEnd = GetVramEndAddr(MEM_VRAMA);
    mMemVramBEnd = GetVramEndAddr(MEM_VRAMB);
}

u32 GfxMemoryMgrCtr::aimToAlignment(u32 alignment)
{
    u32 newAlignment = 128;
    switch(alignment)
    {
    case NN_GX_MEM_SYSTEM:
    {
        return newAlignment = 4;
    }
    case NN_GX_MEM_TEXTURE:
        break;
    case NN_GX_MEM_VERTEXBUFFER:
    case NN_GX_MEM_RENDERBUFFER:
    {
        return newAlignment = 192;
    }
    case NN_GX_MEM_DISPLAYBUFFER:
    case NN_GX_MEM_COMMANDBUFFER:
    {
        return newAlignMent = 16;
    }
    default:
    {
        SEAD_ASSERT_MSG(false, "undefined aim(%d).", alignment);
    }
    }
}

int DefaultGfxMemoryMgrCtr::allocate(u32 area, u32 alignment, u32 buf,s32 size, Heap* heap)
{
    u32 aligned = aimToAlignment(alignment);

    if (area == reinterpret_cast<u32>(&DeleteArray<void>))
    {
        return new (mGfxHeap) u8[size];
    }

    if (area == NN_GX_MEM_VRAMB)
    {
        int start = MathCalcCommon<int>::roundUpN(mMemVramAStart, aligned);

        void* end = PtrUtil::addOffset(start, size );

        if (PtrUtil::diff(end, mMemVramAEnd) < 0)
        {
            SEAD_ASSERT_MSG(false, "out of memory VRAM-A");

            return nullptr;
        }

        mMemVramAStart = reinterpret_cast<uintptr_t>(end);

        return start;
    }

    if (area == NN_GX_MEM_VRAMB)
    {
        int start = MathCalcCommon<int>::roundUpN(mMemVramBStart, aligned);

        void* end = PtrUtil::addOffset(start, size);

        if (PtrUtil::diff(end, mMemVramBEnd) < 0)
        {
            SEAD_ASSERT_MSG(false, "out of memory on VRAM-B.");

            return nullptr;
        }

        mMemVramBStart = reinterpret_cast<uintptr_t>(end);

        return start;
    }

    SEAD_ASSERT_MSG(false, "undefined area(%d).", area);

    return nullptr;
}

void DefaultGfxMemoryMgrCtr::deallocate(u32 area, u32 alignment, u32 buf,s32 size, void* obj)
{
    if(area == NN_GX_MEM_FCRAM)
    {
        delete obj;
    }
    else if((alignment != NN_GX_MEM_VRAMA) && (alignment != NN_GX_MEM_VRAMB))
    {
        SEAD_ASSERT_MSG(false, "undefined area(%d).", alignment);
    }
}

void DefaultGfxMemoryMgrCtr::loadCurrentState(const State& dst)
{
    mMemVramAStart = dst.mVramAStart;
    mMemVramBStart = dst.mVramBStart;
}

void DefaultGfxMemoryMgrCtr::saveCurrentState(State* dst) const
{
    SEAD_ASSERT(dst);

    dst->mVramAStart = mMemVramAStart;
    dst->mVramBStart = mMemVramBStart;
}
}