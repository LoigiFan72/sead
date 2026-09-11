#ifndef SEAD_CRITICAL_SECTION_H_
#define SEAD_CRITICAL_SECTION_H_

#include <nn/os.h>

#include <basis/seadTypes.h>
#include <heap/seadDisposer.h>

namespace sead
{
class Heap;

class CriticalSection
#if not SEAD_CRITICALSECTION_PURE
    : public IDisposer
#endif
{
public:
    CriticalSection();
    explicit CriticalSection(Heap* disposer_heap);
#if not SEAD_CRITICALSECTION_PURE
    CriticalSection(Heap* disposer_heap, HeapNullOption heap_null_option);
#endif
    virtual ~CriticalSection();

    CriticalSection(const CriticalSection&){ };
    CriticalSection& operator=(const CriticalSection&){ };

    void lock();
    bool tryLock();
    void unlock();

    nn::os::CriticalSection mCriticalSectionInner;
};

}  // namespace sead

#endif  // SEAD_CRITICAL_SECTION_H_
