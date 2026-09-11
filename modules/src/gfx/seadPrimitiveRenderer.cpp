#ifdef cafe
#include <gfx/cafe/seadPrimitiveRendererCtr.h>
#endif  // cafe

#include <gfx/seadPrimitiveRenderer.h>

namespace sead
{
SEAD_SINGLETON_DISPOSER_IMPL(PrimitiveRenderer)

PrimitiveRenderer::PrimitiveRenderer():
    IDisposer(),
    mRendererImpl(nullptr),
    mModelMtx(Matrix34f::ident)
{
}

void PrimitiveRenderer::doPrepare_(Heap* heap)
{
    mRendererImpl = new (heap) PrimitiveRendererCtr(heap);
    SEAD_ASSERT(mRendererImpl);
}

void PrimitiveRenderer::prepareFromBinary(Heap* heap, const void* bin_data, u32 bin_size)
{
    doPrepare_(heap);
    mRendererImpl->prepareFromBinaryImpl(heap, bin_data, bin_size);
}

void PrimitiveRenderer::prepare(Heap* heap, const SafeString& path)
{
    doPrepare_(heap);
    mRendererImpl->prepareImpl(heap, path);
}

void PrimitiveRenderer::setCamera(const Camera& camera)
{
    mRendererImpl->setCameraImpl(camera);
}

void PrimitiveRenderer::setProjection(const Projection& projection)
{
    mRendererImpl->setProjectionImpl(projection);
}

void PrimitiveRenderer::setModelMatrix(const Matrix34f& model_mtx)
{
    mModelMtx = model_mtx;
}

void PrimitiveRenderer::begin()
{
    mRendererImpl->beginImpl();
}

void PrimitiveRenderer::end()
{
    mRendererImpl->endImpl();
}

}  // namespace sead
