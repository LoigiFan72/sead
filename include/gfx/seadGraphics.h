#pragma once

#include <gfx/seadColor.h>
#include <gfx/seadDrawLockContext.h>
#include <heap/seadDisposer.h>

namespace sead
{
class Graphics : public IDisposer
{
protected:
    using UnknownCallback = void (*)(int);
    static Graphics* sInstance;

public:
    using LockFunc = void (*)(bool isLock);
    
    enum DevicePosture
    {
        cDevicePosture_Same = 0,
        cDevicePosture_RotateRight = 1,
        cDevicePosture_RotateLeft = 2,
        cDevicePosture_RotateHalfAround = 3,
        cDevicePosture_FlipX = 4,
        cDevicePosture_FlipY = 5,
        cDevicePosture_FlipXY = 3,
        cDevicePosture_Invalid = 4,
    };

    enum class AlphaFunc;
    enum class DepthFunc;
    enum class StencilFunc;
    enum class StencilOp;
    enum class PolygonMode;
    enum class BlendEquation;
    enum class BlendFactor;
    enum class CullingMode;

    Graphics();
    virtual ~Graphics();

    void initialize(Heap* heap);
    void lockDrawContext();
    void unlockDrawContext();
    void initHostIO();

    virtual void initializeImpl(Heap* heap) = 0;
    virtual void setViewportImpl(f32 x, f32 y, f32 w, f32 h) = 0;
    virtual void setScissorImpl(f32 x, f32 y, f32 w, f32 h) = 0;
    virtual void setDepthTestEnableImpl(bool enable) = 0;
    virtual void setDepthWriteEnableImpl(bool enable) = 0;
    virtual void setDepthFuncImpl(DepthFunc func) = 0;
    virtual bool setVBlankWaitIntervalImpl(u32 interval) = 0;
    virtual void setCullingModeImpl(CullingMode mode) = 0;
    virtual void setBlendEnableImpl(bool enable) = 0;
    virtual void setBlendEnableMRTImpl(u32 target, bool enable) = 0;
    virtual void setBlendFactorImpl(BlendFactor srcFactorRgb, BlendFactor dstFactorRgb, BlendFactor srcFactorA, BlendFactor dstFactorA) = 0;
    virtual void setBlendFactorMRTImpl(u32 target, BlendFactor srcFactorRgb, BlendFactor dstFactorRgb, BlendFactor srcFactorA, BlendFactor dstFactorA) = 0;
    virtual void setBlendEquationImpl(BlendEquation equationRgb, BlendEquation equationA) = 0;
    virtual void setBlendEquationMRTImpl(u32 target, BlendEquation equationRgb, BlendEquation equationA) = 0;
    virtual void setBlendConstantColorImpl(const Color4f& color) = 0;
    virtual void lockDrawContextImpl();
    virtual void unlockDrawContextImpl();
    virtual void waitForVBlankImpl() = 0;
    virtual void setColorMaskImpl(bool r, bool g, bool b, bool a) = 0;
    virtual void setColorMaskMRTImpl(u32 target, bool r, bool g, bool b, bool a) = 0;
    virtual void setAlphaTestEnableImpl(bool enable) = 0;
    virtual void setAlphaTestFuncImpl(AlphaFunc func, f32 ref) = 0;
    virtual void setStencilTestEnableImpl(bool enable) = 0;
    virtual void setStencilTestFuncImpl(StencilFunc func, s32 ref, u32 mask) = 0;
    virtual void setStencilTestOpImpl(StencilOp fail, StencilOp zfail, StencilOp zpass) = 0;
    virtual void setPolygonModeImpl(PolygonMode front, PolygonMode back) = 0;
    virtual void setPolygonOffsetEnableImpl(bool fillFrontEnable, bool fillBackEnable, bool pointLineEnable) = 0;

    void clear(u32 colorIdx, Color4f const& color, f32, u32);

    static Graphics* instance(){ return sInstance; }

    static void setInstance(Graphics* inst){ sInstance = inst; }

    void waitForVBlank(){ waitForVBlankImpl(); }
private:
    LockFunc mContextLockFunc;
    DrawLockContext* mDrawLockContext;

    static DevicePosture sDefaultDevicePosture;
    static f32 sDefaultDeviceZScale;
    static f32 sDefaultDeviceZOffset;
};

}  // namespace sead
