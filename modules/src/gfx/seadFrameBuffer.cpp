#include <gfx/seadFrameBuffer.h>

namespace sead
{
LogicalFrameBuffer::~LogicalFrameBuffer()
{
};

FrameBuffer::~FrameBuffer()
{ 
};

void FrameBuffer::clearMRT(u32, const Color4f&) const 
{
}

void FrameBuffer::bind() const
{
    bindImpl_();
}
}  // namespace sead
