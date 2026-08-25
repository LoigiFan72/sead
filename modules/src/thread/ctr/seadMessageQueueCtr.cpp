// Filename: seadMessageQueueCtr.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "thread/seadMessageQueue.h"

namespace sead
{
MessageQueue::MessageQueue()
{

}

MessageQueue::~MessageQueue()
{

}

void MessageQueue::allocate(s32 size, Heap* heap)
{
    if (size <= 0)
    {
        SEAD_ASSERT_MSG(false, "MessageQueue size must not be zero");
        return;
    }

    mBuffer = new (heap) Element[size];
    mMessageQueueInner.Initialize(reinterpret_cast<uptr*>(mBuffer), size);
}

void MessageQueue::free()
{
    mMessageQueueInner.Finalize();
    if (mBuffer)
    {
        delete[] mBuffer;
        mBuffer = nullptr;
    }
}

bool MessageQueue::push(MessageQueue::Element message, MessageQueue::BlockType block_type)
{
    if (block_type == BlockType::Blocking)
    {
        mMessageQueueInner.Enqueue(message);
        return true;
    }

    return mMessageQueueInner.TryEnqueue(message);
}

MessageQueue::Element MessageQueue::pop(MessageQueue::BlockType block_type)
{
    u64 message;

    if (block_type == BlockType::Blocking)
    {
        mMessageQueueInner.Dequeue(&message);
        return message;
    }

    if (mMessageQueueInner.TryDequeue(&message))
        return message;
    return 0;
}

MessageQueue::Element MessageQueue::peek(MessageQueue::BlockType block_type) const
{
    u64 message;

    if (block_type == BlockType::Blocking)
    {
        mMessageQueueInner.GetFront(&message);
        return message;
    }

    if (mMessageQueueInner.TryGetFront(&message))
        return message;
    return 0;
}

bool MessageQueue::jam(MessageQueue::Element message, MessageQueue::BlockType block_type)
{
    if (block_type == BlockType::Blocking)
    {
        mMessageQueueInner.Jam(message);
        return true;
    }

    return mMessageQueueInner.TryJam(message);
}
} // namespace sead