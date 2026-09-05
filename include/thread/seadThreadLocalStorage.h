#pragma once

#ifdef CTRSDK
    #include <nn/os.h>
#endif

#include "basis/seadTypes.h"

namespace sead{

class ThreadLocalStorage{
public:
    ThreadLocalStorage(){ }
    ~ThreadLocalStorage(){}

    ThreadLocalStorage(const ThreadLocalStorage&){ };
    ThreadLocalStorage& operator=(const ThreadLocalStorage&){ };

    void setValue(uptr value){}
    uptr getValue() const{}

private:
#ifdef CTRSDK
    nn::os::ThreadLocalStorage mTlsSlot;
#endif
};
}  // namespace sead

#define SEAD_THREAD_THREAD_LOCAL_STORAGE_H_

#ifdef CTRSDK
#include "thread/ctr/seadThreadLocalStorageCtr.hpp"
#else
#error "Unknown platform"
#endif

#undef SEAD_THREAD_THREAD_LOCAL_STORAGE_H_