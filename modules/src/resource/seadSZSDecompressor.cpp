#include <filedevice/seadFileDeviceMgr.h>
#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>
#include <math/seadMathCalcCommon.h>
#include <prim/seadBitUtil.h>
#include <prim/seadEndian.h>
#include <prim/seadPtrUtil.h>
#include <prim/seadSafeString.h>
#include <resource/seadSZSDecompressor.h>

namespace
{
__attribute__((naked)) s32 decodeSZSCtrAsm_(void* dst, const void* src)
{
    asm("push {r4-r8,lr}\n");
    asm("ldr r4, [r1,#4]\n");
    asm("eor r5, r4, r4,ror#16\n");
    asm("bic r5, r5, #0xff0000\n");
    asm("mov r4, r4,ror#8\n");
    asm("eor r4, r4, r5,lsr#8\n");
    asm("mov r2, r4\n");
    asm("add r1, r1, #0x10\n");
    asm("mov r5, #0\n");
    asm("mov lr, #0x1000\n");
    asm("sub lr, lr, #1\n");

    asm("_decloop0: movs r5, r5,lsr#1\n");
    asm("bne _decloop1\n");
    asm("ldrb r6, [r1],#1\n");
    asm("mov r5, #0x80\n");

    asm("_decloop1: tst r6, r5\n");
    asm("bne _decloop5\n");
    asm("ldrb r3, [r1],#1\n");
    asm("ldrb r7, [r1],#1\n");
    asm("add r3, r7, r3,lsl#8\n");
    asm("movs r7, r3,lsr#12\n");
    asm("and r3, r3, lr\n");
    asm("add r3, r3, #1\n");
    asm("add r7, r7, #2\n");
    asm("bne _decloop2\n");
    asm("ldrb r7, [r1],#1\n");
    asm("add r7, r7, #0x12\n");

    asm("_decloop2: sub r4, r4, r7\n");

    asm("_decloop3: ldrb r12, [r0,-r3]\n");
    asm("subs r7, r7, #1\n");
    asm("strb r12, [r0],#1\n");
    asm("bne _decloop3\n");
    asm("cmp r4, #0\n");
    asm("bne _decloop0\n");
    asm("b _decloop8\n");

    asm("_decloop5: ldrb r12, [r1],#1\n");
    asm("subs r4, r4, #1\n");
    asm("strb r12, [r0],#1\n");
    asm("bne _decloop0\n");

    asm("_decloop8: pop {r4-r8,lr}\n");
    asm("mov r0, r2\n");
    asm("bx lr\n");
}
}  // namespace

namespace sead
{
SZSDecompressor::DecompContext::DecompContext()
{
    initialize(NULL);
}

SZSDecompressor::DecompContext::DecompContext(void* dst)
{
    initialize(dst);
}

void SZSDecompressor::DecompContext::initialize(void* dst)
{
    destp = static_cast<u8*>(dst);
    destCount = 0;
    forceDestCount = 0;
    flagMask = 0;
    flags = 0;
    packHigh = 0;
    step = SZSDecompressor::cStepNormal;
    lzOffset = 0;
    headerSize = 0x10;
}

SZSDecompressor::SZSDecompressor(u32 workSize, u8* workBuffer) : Decompressor("szs")
{
    if (workBuffer == NULL)
    {
        mWorkSize = Mathu::roundUpPow2(workSize, FileDevice::cBufferMinAlignment);
        mWorkBuffer = NULL;
    }

    else
    {
        mWorkSize = workSize;
        mWorkBuffer = workBuffer;
    }
}

u8* SZSDecompressor::tryDecompFromDevice(const ResourceMgr::LoadArg& loadArg, Resource* resource,
                                         u32* outSize, u32* outAllocSize, bool* outAllocated)
{
    Heap* heap = loadArg.load_data_heap;
    if (heap == NULL)
        heap = HeapMgr::sInstancePtr->getCurrentHeap();

    FileHandle handle;
    FileDevice* device;
    u8* src;

    if (loadArg.device != NULL)
        device = loadArg.device->tryOpen(&handle, loadArg.path, FileDevice::cFileOpenFlag_ReadOnly,
                                         loadArg.div_size);
    else
        device = FileDeviceMgr::instance()->tryOpen(
            &handle, loadArg.path, FileDevice::cFileOpenFlag_ReadOnly, loadArg.div_size);

    if (device != NULL &&
        ((src = mWorkBuffer, src != NULL) ||
         (src = new (heap, -FileDevice::cBufferMinAlignment) u8[mWorkSize], src != NULL)))
    {
        u32 bytesRead = handle.read(src, mWorkSize);
        if (bytesRead >= 0x10)
        {
            u32 decompSize = getDecompSize(src);
            s32 decompAlignment = getDecompAlignment(src);

            u32 allocSize = loadArg.load_data_buffer_size;
            u8* dst = loadArg.load_data_buffer;

            if (decompSize > allocSize && allocSize != 0)
                decompSize = allocSize;

            bool allocated = false;
            allocSize = Mathu::roundUpPow2(decompSize, 0x20);

            if (dst == NULL)
            {
                DirectResource* directResource = DynamicCast<DirectResource, Resource>(resource);
                if (directResource != NULL)
                {
                    s32 alignment = loadArg.load_data_alignment;
                    if (alignment != 0)
                        decompAlignment = (alignment < 0x20) ? 0x20 : alignment;

                    else
                    {
                        if (decompAlignment == 0)
                            decompAlignment = directResource->getLoadDataAlignment();

                        decompAlignment = ((loadArg.instance_alignment < 0) ? -1 : 1) *
                                          ((decompAlignment < 0x20) ? 0x20 : decompAlignment);
                    }
                }

                else
                    decompAlignment = -(((loadArg.instance_alignment < 0) ? -1 : 1) << 5);

                dst = new (heap, decompAlignment) u8[allocSize];

                if (dst != NULL)
                    allocated = true;
            }

            if (dst != NULL)
            {
                s32 error;
                if (bytesRead < mWorkSize)
                    error = decomp(dst, allocSize, src, mWorkSize);

                else
                {
                    DecompContext context(dst);
                    context.forceDestCount = decompSize;

                    do
                    {
                        error = streamDecomp(&context, src, bytesRead);
                        if (error <= 0)
                            break;
                    } while ((bytesRead = handle.read(src, mWorkSize), bytesRead != 0));
                }

                if (!(error < 0))
                {
                    if (mWorkBuffer == NULL)
                        delete[] src;

                    if (outSize != NULL)
                        *outSize = decompSize;

                    if (outAllocSize != NULL)
                        *outAllocSize = allocSize;

                    if (outAllocated != NULL)
                        *outAllocated = allocated;

                    return dst;
                }

                if (allocated)
                    delete[] dst;
            }
        }

        if (mWorkBuffer == NULL)
            delete[] src;
    }

    return NULL;
}

u32 SZSDecompressor::getDecompAlignment(const void* src)
{
    return Endian::toHostU32(Endian::cBig, BitUtil::bitCastPtr<u32>(src, 8));
}

u32 SZSDecompressor::getDecompSize(const void* src)
{
    return Endian::toHostU32(Endian::cBig, BitUtil::bitCastPtr<u32>(src, 4));
}

s32 SZSDecompressor::readHeader_(DecompContext* context, const u8* src, u32 src_size)
{
    s32 len = 0;

    while (context->headerSize != 0)
    {
        context->headerSize -= 1;

        if (context->headerSize == 0xF)
        {
            if (*src != 0x59)
                return -1;
        }

        else if (context->headerSize == 0xE)
        {
            if (*src != 0x61)
                return -1;
        }

        else if (context->headerSize == 0xD)
        {
            if (*src != 0x7A)
                return -1;
        }

        else if (context->headerSize == 0xC)
        {
            if (*src != 0x30)
                return -1;
        }

        else if (7 < context->headerSize)
            context->destCount |= static_cast<u32>(*src) << (context->headerSize - 8) * 8;

        src++;
        len += 1;
        if (--srcSize == 0 && context->headerSize != 0)
            return len;
    }

    if (context->forceDestCount < 1)
        return len;

    if (context->destCount <= context->forceDestCount)
        return len;

    context->destCount = context->forceDestCount;
    return len;
}

s32 SZSDecompressor::streamDecomp(DecompContext* context, const void* src, u32 src_size)
{
    const u8* _src = static_cast<const u8*>(src);
    u32 n;

    if (context->headerSize != 0)
    {
        s32 len = readHeader_(context, _src, srcSize);
        if (len < 0)
            return len;

        srcSize -= len;
        _src += len;

        if (srcSize == 0)
        {
            if (context->headerSize == 0)
                return context->destCount;

            return -1;
        }
    }

    while (context->destCount > 0)
    {
        if (context->step == cStepLong)
        {
            n = *_src + 0x12;
            if (!context->doCopy(n))
                return -2;
        }

        else if (context->step == cStepShort)
        {
            context->lzOffset = (((context->packHigh << 8) & 0xf00) | *_src) + 1;

            n = context->packHigh >> 4;
            if (n != 0)
            {
                n += 2;
                if (!context->doCopy(n))
                    return -2;
            }

            else
                context->step = cStepLong;
        }

        else
        {
            if (context->flagMask == 0)
            {
                context->flags = *_src++;
                context->flagMask = 0x80;
                if (--srcSize == 0)
                    break;
            }

            if ((context->flags & context->flagMask) == 0)
            {
                context->packHigh = *_src;
                context->step = cStepShort;
            }

            else
            {
                *context->destp++ = *_src;
                context->destCount -= 1;
            }

            context->flagMask >>= 1;
        }

        if (--srcSize == 0)
            break;

        _src++;
    }

    if (context->destCount == 0 && context->forceDestCount == 0 && 0x20 < srcSize)
        return -1;

    else
        return context->destCount;
}

s32 SZSDecompressor::decomp(void* dst, u32 dstSize, const void* src, u32 src_size)
{
    SEAD_ASSERT(dst);
    SEAD_ASSERT(src);

    u32 magic = Endian::toHostU32(Endian::cBig, BitUtil::bitCastPtr<u32>(src));
    if (magic != 0x59617A30)
        return -1;

    u32 decompSize = getDecompSize(src);
    s32 error = -2;
    if (dstSize >= decompSize)
    {
#ifdef CTRSDK
        error = decodeSZSCtrAsm_(dst, src);
#else
        SEAD_ASSERT_MSG(false, "SZSDecompressor::decomp not implemented");
#endif  // cafe
    }

    return error;
}

}  // namespace sead
