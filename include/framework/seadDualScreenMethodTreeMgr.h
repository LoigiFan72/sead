#pragma once

#include <framework/seadMethodTree.h>
#include <framework/seadMethodTreeMgr.h>

namespace sead 
{
class DualScreenMethodTreeMgr : public MethodTreeMgr
{
    SEAD_RTTI_OVERRIDE(DualScreenMethodTreeMgr, MethodTreeMgr)

public:
    DualScreenMethodTreeMgr();
    virtual ~DualScreenMethodTreeMgr();
    virtual void attachMethod(s32 methodType, MethodTreeNode* node);
    virtual MethodTreeNode* getRootMethodTreeNode(s32 methodType);
    virtual void pauseAll(bool p);
    virtual void pauseAppCalc(bool p);

    bool getSysDrawScreen() const { return mIsSysBtmScreen; }
    bool getAppDrawScreen() const { return mIsAppBtmScreen; }

    void calc();
    void drawTop();
    void drawBtm();

protected:
    MethodTreeNode mRootCalcNode;
    MethodTreeNode mSysCalcNode;
    MethodTreeNode mAppCalcNode;
    MethodTreeNode mTopRootDrawNode;
    MethodTreeNode mTopSysDrawNode;
    MethodTreeNode mTopAppDrawNode;
    MethodTreeNode mTopAppDrawFinalNode;
    MethodTreeNode mBtmRootDrawNode;
    MethodTreeNode mBtmSysDrawNode;
    MethodTreeNode mBtmAppDrawNode;
    MethodTreeNode mBtmAppDrawFinalNode;
    bool mIsSysBtmScreen;
    bool mIsAppBtmScreen;
};

} // namespace sead