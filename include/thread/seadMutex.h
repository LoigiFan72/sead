#pragma once

#include <nn/os.h>

#include <basis/seadTypes.h>
#include <heap/seadDisposer.h>

namespace sead
{
class Heap;

class Mutex : public IDisposer
{
public:
    Mutex();
    explicit Mutex(Heap* disposer_heap);
    Mutex(Heap* disposer_heap, HeapNullOption heap_null_option);
    virtual ~Mutex();

    Mutex(const Mutex&){ };
    Mutex& operator=(const Mutex&){ };

    void lock();
    bool tryLock();
    void unlock();

    // For compatibility with the standard Lockable concept.
    bool try_lock() { return tryLock(); }

    nn::os::Mutex mMutexInner;
};

}  // namespace sead
