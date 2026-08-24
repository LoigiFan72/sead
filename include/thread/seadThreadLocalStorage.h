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

    ThreadLocalStorage(const ThreadLocalStorage&) = delete;
    ThreadLocalStorage& operator=(const ThreadLocalStorage&) = delete;

    void setValue(uptr value){}
    uptr getValue() const{}

private:
#ifdef CTRSDK
    nn::os::ThreadLocalStorage mTls;
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