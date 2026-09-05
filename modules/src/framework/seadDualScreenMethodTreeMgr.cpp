#include <framework/seadDualScreenMethodTreeMgr.h>

namespace sead 
{
DualScreenMethodTreeMgr::DualScreenMethodTreeMgr(): 
    MethodTreeMgr(), 
    mRootCalcNode(getTreeCriticalSection()), 
    mSysCalcNode(getTreeCriticalSection()), 
    mAppCalcNode(getTreeCriticalSection()), 
    mTopRootDrawNode(getTreeCriticalSection()), 
    mTopSysDrawNode(getTreeCriticalSection()), 
    mTopAppDrawNode(getTreeCriticalSection()), 
    mTopAppDrawFinalNode(getTreeCriticalSection()),
    mBtmRootDrawNode(getTreeCriticalSection()), 
    mBtmSysDrawNode(getTreeCriticalSection()), 
    mBtmAppDrawNode(getTreeCriticalSection()), 
    mBtmAppDrawFinalNode(getTreeCriticalSection())
{
    mRootCalcNode.setName("sead::RootCalc");
    mSysCalcNode.setName("sead::SysCalc");
    mAppCalcNode.setName("sead::AppCalc");

    mTopRootDrawNode.setName("sead::TopRootDraw");
    mTopSysDrawNode.setName("sead::TopSysDraw");
    mTopAppDrawNode.setName("sead::TopAppDraw");
    mTopAppDrawFinalNode.setName("sead::TopAppDrawFinal");

    mBtmRootDrawNode.setName("sead::BtmRootDraw");
    mBtmSysDrawNode.setName("sead::BtmSysDraw");
    mBtmAppDrawNode.setName("sead::BtmAppDraw");
    mBtmAppDrawFinalNode.setName("sead::BtmAppDrawFinal");

    mRootCalcNode.pushBackChild(&mSysCalcNode);
    mRootCalcNode.pushBackChild(&mAppCalcNode);

    mTopRootDrawNode.pushBackChild(&mTopAppDrawNode);
    mTopRootDrawNode.pushBackChild(&mTopAppDrawFinalNode);
    mTopRootDrawNode.pushBackChild(&mTopSysDrawNode);

    mBtmRootDrawNode.pushBackChild(&mBtmAppDrawNode);
    mBtmRootDrawNode.pushBackChild(&mBtmAppDrawFinalNode);
    mBtmRootDrawNode.pushBackChild(&mBtmSysDrawNode);

    mSysCalcNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    mTopSysDrawNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    mBtmSysDrawNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);

    mAppCalcNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    mTopAppDrawNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    mTopAppDrawFinalNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    mBtmAppDrawNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    mBtmAppDrawFinalNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);

    mRootCalcNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Both);
    mTopRootDrawNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Both);
    mBtmRootDrawNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Both);
}

DualScreenMethodTreeMgr::~DualScreenMethodTreeMgr()
{
}

void DualScreenMethodTreeMgr::attachMethod(s32 methodType, MethodTreeNode* node)
{
    switch (methodType)
    {
        case 0:
            mSysCalcNode.pushBackChild(node);
            break;

        case 1:
            mAppCalcNode.pushBackChild(node);
            break;

        case 2:
            if (mIsSysBtmScreen == false)
            {
                mTopSysDrawNode.pushFrontChild(node);
            }
            else if (mIsSysBtmScreen == true)
            {
                mBtmSysDrawNode.pushFrontChild(node);
            }
            else
            {
                SEAD_ASSERT_MSG(false, "Undefined screen.");
            }
            break;

        case 3:
            if (mIsAppBtmScreen == false)
            {
                mTopAppDrawNode.pushFrontChild(node);
            }
            else if (mIsAppBtmScreen == true)
            {
                mBtmAppDrawNode.pushFrontChild(node);
            }
            else
            {
                SEAD_ASSERT_MSG(false, "Undefined Screen.");
            }
            break;

        case 4:
            if (mIsAppBtmScreen == false)
            {
                mTopAppDrawFinalNode.pushFrontChild(node);
            }
            else if (mIsAppBtmScreen == true)
            {
                mBtmAppDrawFinalNode.pushFrontChild(node);
            }
            else
            {
                SEAD_ASSERT_MSG(false, "Undefined Screen.");
            }
            break;

        case 5:
            mTopSysDrawNode.pushBackChild(node);
            break;

        case 6:
            mTopAppDrawNode.pushBackChild(node);
            break;

        case 7:
            mTopAppDrawFinalNode.pushBackChild(node);
            break;

        case 8:
            mBtmSysDrawNode.pushBackChild(node);
            break;

        case 9:
            mBtmAppDrawNode.pushBackChild(node);
            break;

        case 10:
            mBtmAppDrawFinalNode.pushBackChild(node);
            break;

        default:
            SEAD_ASSERT_MSG(false, "Undefined MethodType(%d)", methodType);
            break;
    }
}

MethodTreeNode* DualScreenMethodTreeMgr::getRootMethodTreeNode(s32 methodType)
{
    switch (methodType)
    {
        case 0:
            return &mSysCalcNode;

        case 1:
            return &mAppCalcNode;

        case 2:
            if (mIsSysBtmScreen == false)
                return &mTopSysDrawNode;
            else if (mIsSysBtmScreen == true)
                return &mBtmSysDrawNode;
            else
            {
                SEAD_ASSERT_MSG(false, "Undefined Screen.");
                return nullptr;
            }

        case 3:
            if (mIsAppBtmScreen == false)
                return &mTopSysDrawNode;
            else if (mIsAppBtmScreen == true)
                return &mBtmAppDrawNode;
            else
            {
                SEAD_ASSERT_MSG(false, "Undefined Screen.");
                return nullptr;
            }

        case 4:
            if (mIsAppBtmScreen == false)
                return &mTopAppDrawFinalNode;
            else if (mIsAppBtmScreen == true)
                return &mBtmAppDrawFinalNode;
            else
            {
                SEAD_ASSERT(false);
                return nullptr;
            }

        case 5:
            return &mTopSysDrawNode;

        case 6:
            return &mTopAppDrawNode;

        case 7:
            return &mTopAppDrawFinalNode;

        case 8:
            return &mBtmSysDrawNode;

        case 9:
            return &mBtmAppDrawNode;

        case 10:
            return &mBtmAppDrawFinalNode;

        default:
            SEAD_ASSERT_MSG(false, "Undefined MethodType(%d).", methodType);
            return nullptr;
    }
}


void DualScreenMethodTreeMgr::pauseAll(bool p)
{
    if (p)
    {
        mRootCalcNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Both);
        mTopRootDrawNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Both);
        mBtmRootDrawNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Both);
    }
    else
    {
        mRootCalcNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
        mTopRootDrawNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
        mBtmRootDrawNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    }
}

void DualScreenMethodTreeMgr::pauseAppCalc(bool p)
{
    if (p)
    {
        mAppCalcNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Both);
    }
    else
    {
        mAppCalcNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    }
}

void DualScreenMethodTreeMgr::calc()
{
    mRootCalcNode.call();
}

void DualScreenMethodTreeMgr::drawTop()
{
    mTopRootDrawNode.call();
}

void DualScreenMethodTreeMgr::drawBtm()
{
    mBtmRootDrawNode.call();
}

} // namespace sead