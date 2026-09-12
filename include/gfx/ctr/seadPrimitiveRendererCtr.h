#ifndef SEAD_PRIMITIVE_RENDERER_CTR_H_
#define SEAD_PRIMITIVE_RENDERER_CTR_H_

#include "gfx/seadPrimitiveRenderer.h"
#include "gfx/ctr/seadRamCommandCacheCtr.h"
#include <nn/gr.h>

namespace sead
{
class TextureCtr;

class PrimitiveRendererCtr : public PrimitiveRendererBase
{
public:
    enum Mode
    {
        cDraw,
        cDrawTexture,
        cDrawAttr,
        cDrawMax
    };

    struct Shape : public Vertex
    {
        struct Index : public nn::gr::CTR::Vertex::IndexStream
        {
            Index():
                Vertex::IndexStream()
            {
            }

            u32 mTextureIndex;
        };

        Shape():
            Vertex(),
            mShapeIndex()
        {
        }

        Index mShapeIndex;
    };

    PrimitiveRendererCtr();

    virtual void prepareFromBinaryImpl(Heap* heap, const void* bin_data, u32 bin_size);
    virtual void prepareImpl(Heap* heap, const SafeString& path);
    virtual void setCameraImpl(const Camera& camera);
    virtual void setProjectionImpl(const Projection& projection);
    virtual void beginImpl();
    virtual void endImpl();
    virtual void drawQuadImpl(const Matrix34f& model_mtx, const Color4f& colorL,
                              const Color4f& colorR);
    virtual void drawQuadImpl(const Matrix34f& model_mtx, const Texture& texture,
                              const Color4f& colorL, const Color4f& colorR, const Vector2f& uv_src,
                              const Vector2f& uv_size);
    virtual void drawBoxImpl(const Matrix34f& model_mtx, const Color4f& colorL, const Color4f& colorR);
    virtual void drawCubeImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1);
    virtual void drawWireCubeImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1);
    virtual void drawLineImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1);
    virtual void drawSphere4x8Impl(const Matrix34f& model_mtx, const Color4f& north, const Color4f& south);
    virtual void drawSphere8x16Impl(const Matrix34f& model_mtx, const Color4f& north, const Color4f& south);
    virtual void drawDisk16Impl(const Matrix34f& model_mtx, const Color4f& center, const Color4f& edge);
    virtual void drawDisk32Impl(const Matrix34f& model_mtx, const Color4f& center, const Color4f& edge);
    virtual void drawCircle16Impl(const Matrix34f& model_mtx, const Color4f& edge);
    virtual void drawCircle32Impl(const Matrix34f& model_mtx, const Color4f& edge);
    virtual void drawCylinder16Impl(const Matrix34f& model_mtx, const Color4f& top, const Color4f& btm);
    virtual void drawCylinder32Impl(const Matrix34f& model_mtx, const Color4f& top, const Color4f& btm);
    virtual ~PrimitiveRendererCtr();
protected:
    void setup_(Mode mode, TextureCtr const* pTexture);
    void loadBoxIndex_(Heap* heap);
    void loadCircleIndex_(Heap* heap, Shape::Index* index, s32 size);
    void loadCubeVertex_(Heap* heap);
    void loadCylinderVertex_(Heap* heap, Shape* index, s32 size);
    void loadDiskVertex_(Heap* heap, Shape* index, s32 size);
    void loadLineVertex_(Heap* heap);
    void loadQuadVertex_(Heap* heap);
    void loadSphereVertex_(Heap* heap, Shape* index, s32 height, s32 width);
    void loadWireCubeIndex_(Heap* heap);


private:
    RawCommandCacheCtr mCacheVramA;
    BindSymbolVSFloat mSymbolWVP_VramA;
    BindSymbolVSFloat mSymbolUser_VramA;
    BindSymbolVSFloat mSymbolColor0_VramA;
    BindSymbolVSFloat mSymbolColor1_VramA;
    BindSymbolVSFloat mSymbolUvSrc_VramA;
    BindSymbolVSFloat mSymbolUvSize_VramA;
    BindSymbolVSInput mAttrVertexLoc_VramA;
    BindSymbolVSInput mAttrTexCoord0Loc_VramA;
    BindSymbolVSInput mAttrColorRateLoc_VramA;

    /* CTR Device VRAM-B*/

    RawCommandCacheCtr mCacheVramB;
    BindSymbolVSFloat mSymbolWVP_VramB;
    BindSymbolVSFloat mSymbolUser_VramB;
    BindSymbolVSFloat mSymbolColor0_VramB;
    BindSymbolVSFloat mSymbolColor1_VramB;
    BindSymbolVSFloat mSymbolUvSrc_VramB;
    BindSymbolVSFloat mSymbolUvSize_VramB;
    BindSymbolVSInput mAttrVertexLoc_VramB;
    BindSymbolVSInput mAttrTexCoord0Loc_VramB;
    BindSymbolVSInput mAttrColorRateLoc_VramB;

    /* dmp_Line.width */

    BindSymbolGSFloat mDmpLineWidth;

    /* Texture */
    
    nn::gr::CTR::Texture mTexture3D;
    nn::gr::CTR::Texture mTexture2D;

    /* Combiners */

    Combiner mCombiner3D;
    Combiner mCombiner2D;

    /* Utils */

    Mode mMode;
    TextureCtr* mCtrTexture;

    /* Shapes */
    
    Shape mSphere4x8; 
    Shape mSphere8x16;
    Shape mCube;
    Shape mWireCube;
    Shape mDisk16; 
    Shape mDisk32;
    Shape mLine;
    Shape::Index mWireCube16Index; 
    Shape::Index mWireCube32Index; 
    Shape::Index mCircle16Index; 
    Shape::Index mCircle32Index;
    Shape mCylinder16;
    Shape mCylinder32;

    /* Matrixes */

    Matrix34f mViewMtx;
    Matrix44f mProjectionMtx;

    void* mCmdlistBufSize;
};
}

#endif