#pragma once

#include <nn/os.h>

namespace sead
{
class Heap;

class MessageQueue
{
public:
    using Element = s64;

    enum class BlockType
    {
        Blocking = 0,
        NonBlocking = 1,
    };

    MessageQueue();
    ~MessageQueue();

    void allocate(s32 size, Heap* heap);
    void free();
    bool push(Element message, BlockType block_type);
    Element pop(BlockType block_type);
    Element peek(BlockType block_type) const;
    bool jam(Element message, BlockType block_type);

    static constexpr Element cNullElement = 0;

private:
    nn::os::BlockingQueue mMessageQueueInner;
    Element* mBuffer;
};
}  // namespace sead
