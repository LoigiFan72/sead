// Filename: seadThreadCtr.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "thread/seadThread.h"

namespace sead
{
Thread::Thread(const SafeString& name, Heap* heap, s32 priority, MessageQueue::BlockType block_type,MessageQueue::Element quit_msg, s32 stack_size, s32 message_queue_size):
    IDisposer(),
    INamable(name),
    mMessageQueue(),
    mStackSize(stack_size),
    mCurrentHeap(nullptr),
    mBlockType(block_type), 
    mQuitMsg(quit_msg),
    mId(0),
    mState(State::cInitialized),
    mPriority(priority)
{
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
    mThread()
{
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

bool Thread::start()
{
    if (mState)
    {
        SEAD_WARN("Thread is running or done. Can not start.\n");
        return false;
    }

    mThread->TryStart(ctrThreadFunc_, reinterpret_cast<uptr>(this), *this, mPriority);

    if (state == State::cInitialized)
        mState = State::cRunning;

    return true;
}

void Thread::waitDone()
{
    if ((mState.value() | State::cReleased) == State::cReleased)
        return;

    mThread->Join();
    SEAD_ASSERT_MSG(mState == State::cTerminated, "Join failed?");
    mState = State::cReleased;
}

void Thread::setPriority(s32 prio)
{
    mPriority = prio;
    if(isActive)
    {
        Thread* thread = ThreadMgr::instance()->getCurrentThread();
        if(thread == this)
        {
            nn::os::Thread::ChangeCurrentPriority(prio);
        }
        else
        {
            mThread->ChangePriority(prio)
        }
    }
}

s32 Thread::getPriority() const
{
    return mPriority;
}

void Thread::yield()
{
    mThread->Yield();
}

void Thread::sleep(TickSpan howLong)
{
    mThread->Sleep(howLong.toS64());
}

uintptr_t Thread::getStackCheckStartAddress_() const
{
    return uintptr_t(mStackTopForCheck);
}

#ifdef CTRSDK
void Thread::ctrThreadFunc_(uptr arg)
{
    sead::Thread* self = static_cast<Thread*>(arg);

    ThreadMgr::instance()->mTls.setValue(uintptr_t(self));

    const u32 id = self->mThread->GetThreadId();
    self->mState = State::cRunning;
    self->mId = id;
    self->run_();
    self->mState = State::cTerminated;
}
#endif

/* sead::ThreadMgr */

ThreadMgr::ThreadMgr()
{ 

}

u32 ThreadMgr::getCurrentThreadID_()
{
    return u32(uintptr_t(nn::os::Thread::GetCurrentId()));
}

void ThreadMgr::initMainThread_(Heap* heap)
{
    nn::os::Thread* nn_thread = nn::os::Thread::GetMainThread();
    const u64 thread_id = nn::os::Thread::GetThreadId();

    MainThread thread = new (heap) MainThread(heap, nn_thread, thread_id);
    mMainThread = thread;
    mThreadPtrTLS.setValue(uintptr_t(thread));
}
}// namespace sead