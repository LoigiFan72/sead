// Filename: seadThreadCtr.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "thread/seadThread.h"
#include "basis/seadDelete.h"

namespace sead{

Thread::Thread(const SafeString& name, Heap* heap, s32 priority, MessageQueue::BlockType block_type,MessageQueue::Element quit_msg, s32 stack_size, s32 message_queue_size): 
    INamable(name), 
    mStackSize(stack_size), 
    mBlockType(block_type), 
    mQuitMsg(quit_msg),
    mPriority(priority){
    mListNode.mData = this;
    mMessageQueue.allocate(message_queue_size, heap);
    mStackTop = new (heap, 0x1000) u8[stack_size];

    mThread = new(heap) nn::os::Thread();

    initStackCheck_();
    if (ThreadMgr::instance())
        ThreadMgr::instance()->addThread_(this);
    else
        SEAD_ASSERT_MSG(false, "ThreadMgr not initialized");
}

Thread::Thread(Heap* heap, nn::os::Thread* pThread, u32 thread_id): 
    INamable("sead::MainThread"), 
    mBlockType(MessageQueue::BlockType::NonBlocking), mQuitMsg(0x7FFFFFFF), mId(thread_id),
    mState(State::cRunning), 
    mThread(){
    mListNode.mData = this;
    mMessageQueue.allocate(32, heap);
    pThread->GetPriority();

    setAffinity(mAffinity);
}

Thread::~Thread()
{
    if (!ThreadMgr::instance())
    {
        SEAD_ASSERT_MSG(false, "ThreadMgr not initialized");
        return;
    }

    if (ThreadMgr::instance()->getMainThread() != this)
    {
        ThreadMgr::instance()->removeThread_(this);

        if (mState != State::cQuitting && mState != State::cTerminated)
        {
            if (mState == State::cRunning)
            {
                SEAD_ASSERT_MSG(false, "Thread is running. Do quit and waitDone");
                quitAndWaitDoneSingleThread(false);
            }
        }
        else
        {
            SEAD_ASSERT_MSG(false, "Thread is not done. Do waitDone");
            waitDone();
        }

        mThread->Finalize();

        if(mThread)
            delete(mThread);

        if (mStackTop)
            delete[] static_cast<u8*>(mStackTop);
    }

    mMessageQueue.free();
}

ThreadMgr::ThreadMgr() = default;

u32 ThreadMgr::getCurrentThreadID_()
{
    return u32(uintptr_t(nn::os::Thread::GetCurrentId()));
}

void ThreadMgr::initMainThread_(Heap* heap)
{
    nn::os::Thread* nn_thread = nn::os::GetMainThread();
    const u64 nn_thread_id = nn::os::Thread::GetThreadId();

    auto thread = new (heap) MainThread(heap, nn_thread, nn_thread_id);
    mMainThread = thread;
    mThreadPtrTLS.setValue(uintptr_t(thread));
}


}