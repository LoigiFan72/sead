// Filename: seadGfxMemoryMgrCtr.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "gfx/ctr/seadPrimitiveRendererCtr.h"
#include "gfx/ctr/seadGraphicsCtr.h"
#include "gfx/ctr/seadTextureCtr.h"
#include "gfx/seadCamera.h"
#include "gfx/seadProjection.h"
#include "filedevice/seadFileDeviceMgr.h"
#include "filedevice/seadFileDevice.h"

namespace
{
u32* makeUniformCommand_(u32* command, u32 symbol, const sead::Vector4f* value)
{
    *command++ = symbol | 0x80000000;
    *command++ = 0x804F02C0;

    *command++ = nn::math::F32AsU32(value->w);
    *command++ = nn::math::F32AsU32(value->z);
    *command++ = nn::math::F32AsU32(value->y);
    *command++ = nn::math::F32AsU32(value->x);

    return command;
}
}

namespace sead
{
PrimitiveRendererCtr::PrimitiveRendererCtr():
    mCacheVramA(),
    mSymbolWVP_VramA(),
    mSymbolUser_VramA(),
    mSymbolColor0_VramA(), 
    mSymbolColor1_VramA(),
    mSymbolUvSrc_VramA(), 
    mSymbolUvSize_VramA(),
    mAttrVertexLoc_VramA(),
    mAttrTexCoord0Loc_VramA(),
    mAttrColorRateLoc_VramA(),

    mCacheVramB(),
    mSymbolWVP_VramB(),
    mSymbolUser_VramB(),
    mSymbolColor0_VramB(), 
    mSymbolColor1_VramB(),
    mSymbolUvSrc_VramB(), 
    mSymbolUvSize_VramB(),
    mAttrVertexLoc_VramB(),
    mAttrTexCoord0Loc_VramB(),
    mAttrColorRateLoc_VramB(),

    mDmpLineWidth(),

    mTexture3D(),
    mTexture2D(),

    mCombiner3D(),
    mCombiner2D(),

    mMode(cDrawMax),
    
    mSphere4x8(),
    mSphere8x16(),
    mCube(),
    mWireCube(),
    mDisk16(),
    mDisk32(),
    mLine(),
    mWireCube16Index(),
    mWireCube32Index(),
    mCircle16Index(),
    mCircle32Index(),
    mCylinder16(),
    mCylinder32(),

    mViewMtx(),
    mProjectionMtx()
{
}

PrimitiveRendererCtr::~PrimitiveRendererCtr()
{
}

void PrimitiveRendererCtr::prepareFromBinaryImpl(Heap* heap, const void* bin_data, u32 bin_size)
{
    /* Setup Global Shader Object */

    Shader* shader = const_cast<Shader*>(reinterpret_cast<const Shader*>(bin_data));
    new(shader) Shader();

    /* Setup VramA */

    shader->SetupBinary(bin_data, 0, -1);
    bool result = shader->SearchBindSymbol(&mSymbolWVP_VramA, "wvp");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mSymbolUser_VramA, "user");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mSymbolColor0_VramA, "color0");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mSymbolColor1_VramA, "color1");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mSymbolUvSrc_VramA, "uv_src");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mSymbolUvSize_VramA, "uv_size");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mAttrVertexLoc_VramA, "Vertex");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mAttrTexCoord0Loc_VramA, "TexCoord0");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mAttrColorRateLoc_VramA, "ColorRate");
    SEAD_ASSERT(result);

    /* Cache VramA */

    mCacheVramA.initialize(heap, 0);
    u32* command = reinterpret_cast<u32*>(mCacheVramA.getTopPtr());
    size_t cache = *shader->MakeFullCommand(command);
    mCacheVramA.adjust(heap, cache);
    
    /* Setup VramB */

    shader->SetupBinary(bin_data, 0, -1);
    result = shader->SearchBindSymbol(&mSymbolWVP_VramB, "wvp");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mSymbolUser_VramB, "user");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mSymbolColor0_VramB, "color0");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mSymbolColor1_VramB, "color1");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mSymbolUvSrc_VramB, "uv_src");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mSymbolUvSize_VramB, "uv_size");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mAttrVertexLoc_VramB, "Vertex");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mAttrTexCoord0Loc_VramB, "TexCoord0");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mAttrColorRateLoc_VramB, "ColorRate");
    SEAD_ASSERT(result);
    result = shader->SearchBindSymbol(&mDmpLineWidth, "dmp_Line.width");
    SEAD_ASSERT(result);

    

    mCacheVramB.initialize(heap, 0);
    u32* command = reinterpret_cast<u32*>(mCacheVramB.getTopPtr());
    size_t cache = *shader->MakeFullCommand(command);
    mCacheVramB.adjust(heap, cache);

    loadQuadVertex_(heap);
    loadLineVertex_(heap);
    loadBoxIndex_(heap);
    loadWireCubeIndex_(heap);

    loadSphereVertex_(heap, &mSphere4x8, 8, 4);
    loadSphereVertex_(heap, &mSphere8x16, 16, 8);

    loadDiskVertex_(heap, &mDisk16, 16);
    loadDiskVertex_(heap, &mDisk32, 32);

    loadCircleIndex_(heap, &mCircle16Index, 16);
    loadCircleIndex_(heap, &mCircle32Index, 32);

    loadCylinderVertex_(heap, &mCylinder16, 16);
    loadCylinderVertex_(heap, &mCylinder32, 32);

    mTexture3D.unit0.texType   = PICA_DATA_TEXTURE0_SAMPLER_TYPE_FALSE;
    mTexture3D.unit0.minFilter = PICA_DATA_TEXTURE_MIN_FILTER_LINEAR;
    mTexture3D.unit0.magFilter = PICA_DATA_TEXTURE_MAG_FILTER_LINEAR;
    mTexture2D.unit0.texType   = PICA_DATA_TEXTURE0_SAMPLER_TYPE_TEXTURE_2D;

    mCombiner3D.stage->SetupPrimary();
    mCombiner2D.stage->SetupPrimaryModulateTexture0();
}

void PrimitiveRendererCtr::prepareImpl(Heap* heap, SafeString const& path)
{
    {
        ScopedCurrentHeapSetter chs(heap);
        FileDevice::LoadArg arg;
        arg.path = path;
        arg.alignment = 32;
        arg.heap = heap;

        const void* file = FileDeviceMgr::instance()->tryLoad(arg);
        prepareFromBinaryImpl(heap, file, arg.read_size);
    }
}

void PrimitiveRendererCtr::setCameraImpl(const Camera& camera)
{
    mViewMtx = camera.getViewMatrix();
}

void PrimitiveRendererCtr::setProjectionImpl(const Projection& projection)
{
    mViewMtx = projection.getDeviceProjectionMatrix();
}

void PrimitiveRendererCtr::beginImpl()
{
    mMode = cDrawMax;
    mCylinder32.mShapeIndex.mTextureIndex = 0;
    mCtrTexture = nullptr;

    nngxGetCmdlistParameteri(NN_GX_CMDLIST_CURRENT_BUFADDR, reinterpret_cast<s32*>(mCylinder32.mShapeIndex.physicalAddr));
    mCylinder32.mShapeIndex.drawVtxNum   = mCylinder32.mShapeIndex.physicalAddr;
    mCylinder32.mShapeIndex.physicalAddr = *nn::gr::CTR::Vertex::MakeDisableCommand(&mCylinder32.mShapeIndex.physicalAddr);
    s32 ctop;
    nngxGetCmdlistParameteri(NN_GX_CMDLIST_TOP_BUFADDR, &ctop);
    s32 caddr = 0;
    nngxGetCmdlistParameteri(NN_GX_CMDLIST_CURRENT_BUFADDR, &caddr);
    mCmdlistBufSize = PtrUtil::addOffset(reinterpret_cast<const void*>(ctop), caddr);
}

void PrimitiveRendererCtr::endImpl()
{
    nngxMoveCommandbufferPointer((mCylinder32.mShapeIndex.physicalAddr - mCylinder32.mShapeIndex.drawVtxNum >> 2) << 2);
}

void PrimitiveRendererCtr::drawQuadImpl(const Matrix34f& model_mtx, const Color4f& colorL, const Color4f& colorR)
{
    setup_(cDraw, nullptr);
    
}
}