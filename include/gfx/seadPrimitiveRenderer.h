#ifndef SEAD_PRIMITIVE_RENDERER_H_
#define SEAD_PRIMITIVE_RENDERER_H_

#include <gfx/seadPrimitiveDrawer.h>
#include <heap/seadDisposer.h>

namespace sead
{
class Heap;
class Camera;
class Projection;
class Texture;

class PrimitiveRendererBase
{
public:
    PrimitiveRendererBase()
    {
    }
    
    virtual void prepareFromBinaryImpl(Heap* heap, const void* bin_data, u32 bin_size) = 0;
    virtual void prepareImpl(Heap* heap, const SafeString& path) = 0;
    virtual void setCameraImpl(const Camera& camera) = 0;
    virtual void setProjectionImpl(const Projection& projection) = 0;
    virtual void beginImpl() = 0;
    virtual void endImpl() = 0;
    virtual void drawQuadImpl(const Matrix34f& model_mtx, const Color4f& colorL,
                              const Color4f& colorR) = 0;
    virtual void drawQuadImpl(const Matrix34f& model_mtx, const Texture& texture,
                              const Color4f& colorL, const Color4f& colorR, const Vector2f& uv_src,
                              const Vector2f& uv_size) = 0;
    virtual void drawBoxImpl(const Matrix34f& model_mtx, const Color4f& colorL, const Color4f& colorR) = 0;
    virtual void drawCubeImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1) = 0;
    virtual void drawWireCubeImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1) = 0;
    virtual void drawLineImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1) = 0;
    virtual void drawSphere4x8Impl(const Matrix34f& model_mtx, const Color4f& north, const Color4f& south) = 0;
    virtual void drawSphere8x16Impl(const Matrix34f& model_mtx, const Color4f& north, const Color4f& south) = 0;
    virtual void drawDisk16Impl(const Matrix34f& model_mtx, const Color4f& center, const Color4f& edge) = 0;
    virtual void drawDisk32Impl(const Matrix34f& model_mtx, const Color4f& center, const Color4f& edge) = 0;
    virtual void drawCircle16Impl(const Matrix34f& model_mtx, const Color4f& edge) = 0;
    virtual void drawCircle32Impl(const Matrix34f& model_mtx, const Color4f& edge) = 0;
    virtual void drawCylinder16Impl(const Matrix34f& model_mtx, const Color4f& top, const Color4f& btm) = 0;
    virtual void drawCylinder32Impl(const Matrix34f& model_mtx, const Color4f& top, const Color4f& btm) = 0;
};

class PrimitiveRenderer : public IDisposer
{
    SEAD_SINGLETON_DISPOSER(PrimitiveRenderer)
public:
    using QuadArg = PrimitiveDrawer::QuadArg;
    using UVArg = PrimitiveDrawer::UVArg;
    using CubeArg = PrimitiveDrawer::CubeArg;
    
    PrimitiveRenderer();
    virtual ~PrimitiveRenderer() {}

    void prepareFromBinary(Heap* heap, const void* bin_data, u32 bin_size);
    void prepare(Heap* heap, const SafeString& path);
    void setCamera(const Camera& camera);
    void setProjection(const Projection& projection);
    void setModelMatrix(const Matrix34f& model_mtx);
    void begin();
    void end();

    void drawQuad(const Color4f& colorL, const Color4f& colorR);
    void drawQuad(const Color4f& color) { drawQuad(color, color); }
    void drawQuad(const Texture& texture, const Color4f& colorL, const Color4f& colorR, const Vector2f& uv_src = Vector2f::zero, const Vector2f& uv_size = Vector2f::ones);
    void drawQuad(const Texture& texture, const Color4f& color = Color4f::cWhite, const Vector2f& uv_src = Vector2f::zero, const Vector2f& uv_size = Vector2f::ones) { drawQuad(texture, color, color, uv_src, uv_size); }
    void drawBox(const Color4f& colorL, const Color4f& colorR);
    void drawBox(const Color4f& color) { drawBox(color, color); }
    void drawCube(const Color4f& c0, const Color4f& c1);
    void drawCube(const Color4f& color) { drawCube(color, color); }
    void drawWireCube(const Color4f& c0, const Color4f& c1);
    void drawWireCube(const Color4f& color) { drawWireCube(color, color); }
    void drawLine(const Color4f& c0, const Color4f& c1);
    void drawLine(const Color4f& color) { drawLine(color, color); }
    void drawSphere4x8(const Color4f& north, const Color4f& south);
    void drawSphere4x8(const Color4f& color) { drawSphere4x8(color, color); }
    void drawSphere8x16(const Color4f& north, const Color4f& south);
    void drawSphere8x16(const Color4f& color) { drawSphere8x16(color, color); }
    void drawDisk16(const Color4f& center, const Color4f& edge);
    void drawDisk16(const Color4f& color) { drawDisk16(color, color); }
    void drawDisk32(const Color4f& center, const Color4f& edge);
    void drawDisk32(const Color4f& color) { drawDisk32(color, color); }
    void drawCircle16(const Color4f& color);
    void drawCircle32(const Color4f& color);
    void drawCylinder16(const Color4f& top, const Color4f& btm);
    void drawCylinder16(const Color4f& color) { drawCylinder16(color, color); }
    void drawCylinder32(const Color4f& top, const Color4f& btm);
    void drawCylinder32(const Color4f& color) { drawCylinder32(color, color); }

    void drawQuad(const QuadArg& arg);
    void drawQuad(const Texture& texture, const QuadArg& arg, const UVArg& uv_arg = UVArg());
    void drawBox(const QuadArg& arg);
    void drawCube(const CubeArg& arg);
    void drawWireCube(const CubeArg& arg);
    void drawLine(const Vector3f& from, const Vector3f& to, const Color4f& c0, const Color4f& c1);
    void drawLine(const Vector3f& from, const Vector3f& to, const Color4f& color);
    void drawSphere4x8(const Vector3f& pos, f32 radius, const Color4f& north, const Color4f& south);
    void drawSphere4x8(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawSphere8x16(const Vector3f& pos, f32 radius, const Color4f& north, const Color4f& south);
    void drawSphere8x16(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawDisk16(const Vector3f& pos, f32 radius, const Color4f& center, const Color4f& edge);
    void drawDisk32(const Vector3f& pos, f32 radius, const Color4f& center, const Color4f& edge);
    void drawDisk16(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawDisk32(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawCircle16(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawCircle32(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawCylinder16(const Vector3f& pos, f32 radius, f32 height, const Color4f& top_color, const Color4f& btm_color);
    void drawCylinder16(const Vector3f& pos, f32 radius, f32 height, const Color4f& color);
    void drawCylinder32(const Vector3f& pos, f32 radius, f32 height, const Color4f& top_color, const Color4f& btm_color);
    void drawCylinder32(const Vector3f& pos, f32 radius, f32 height, const Color4f& color);

    void drawAxis(const Vector3f& pos, f32 scale);
private:
    void doPrepare_(Heap* heap);

    PrimitiveRendererBase* mRendererImpl;
    Matrix34f mViewMtx;
    Matrix44f mProjMtx;
    Matrix34f mModelMtx;
};

}  // namespace sead

#endif  // SEAD_PRIMITIVE_RENDERER_H_
