#ifndef SEAD_METHODTREE_H_
#define SEAD_METHODTREE_H_

#include <container/seadTreeNode.h>
#include <heap/seadDisposer.h>
#include <prim/seadBitFlag.h>
#include <prim/seadDelegate.h>
#include <prim/seadNamable.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadStorageFor.h>

namespace sead
{
class CriticalSection;

template <typename T, typename U>
class IDelegate2;

class MethodTreeNode : public IDisposer, public TTreeNode<MethodTreeNode*>, public INamable
{
    SEAD_RTTI_BASE(MethodTreeNode)

public:
    enum PauseFlag
    {
        cPause_None = 0,
        cPause_Self = 1,
        cPause_Child = 2,
        cPause_Both = 3,
    };

    using PauseEventDelegate = IDelegate2<MethodTreeNode*, PauseFlag>;

    explicit MethodTreeNode(CriticalSection* cs) : 
        TTreeNode(this), 
        INamable(), 
        IDisposer(), 
        mDelegateHolder(),
        mCriticalSection(cs),
        mPauseFlag(cPause_None), 
        mPauseEventDelegate(nullptr), 
        mUserID(nullptr)

    {
    }

    virtual ~MethodTreeNode() { detachAll(); }

    template <typename T>
    void bind(T* object, typename Delegate<T>::PTMF method, const char* name)
    {
        lock_();
        mDelegateHolder.bind(object, method);
        unlock_();

        if (name)
            setName(name);
    }

    void call();
    void detachAll();
    void pushBackChild(MethodTreeNode* node);
    void pushFrontChild(MethodTreeNode* node);

    void setPauseFlag(PauseFlag flag)
    {
        lock_();
        if (mPauseEventDelegate)
            mPauseEventDelegate->invoke(this, flag);
        mPauseFlag = flag;
        unlock_();
    }

private:
    void attachMutexRec_(CriticalSection* m) const;
    void callRec_();

    void lock_();
    void unlock_();

    StorageFor<sead::AnyDelegate> mDelegateHolder;
    mutable CriticalSection* mCriticalSection;
    u32 mPriority;
    BitFlag32 mPauseFlag;
    PauseEventDelegate* mPauseEventDelegate;
    void* mUserID;
};

}  // namespace sead

#endif  // SEAD_METHODTREE_H_
