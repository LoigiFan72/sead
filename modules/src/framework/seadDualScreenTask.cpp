// Filename: seadDualScreenTask.cpp
//
// Project: StandardEAD C++ Library for CTR

#include "framework/seadDualScreenTask.h"
#include "framework/seadFramework.h"

namespace sead
{
DualScreenTask::DualScreenTask(const TaskConstructArg& arg):
    TaskBase(arg), mBothNode(NULL), mTopNode(NULL),mBtmNode(NULL)
{
    mBothNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    mTopNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    mBtmNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);

    mBothNode.bind(this, "Task");
    mTopNode.bind(this, "Task");
    mBtmNode.bind(this, "Task");
}

DualScreenTask::DualScreenTask(const TaskConstructArg& arg, const char* name):
    TaskBase(arg, name), mBothNode(NULL), mTopNode(NULL),mBtmNode(NULL)
{
    mBothNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    mTopNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    mBtmNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);

    mBothNode.bind(this, name);
    mTopNode.bind(this, name);
    mBtmNode.bind(this, name);
}

DualScreenTask::~DualScreenTask()
{
}

void DualScreenTask::pauseCalc(bool b)
{
    if(b == false)
    {
        mBothNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    }
    else
    {
        mBothNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Self);
    }
}

void DualScreenTask::pauseDraw(bool b)
{
    if(b == false)
    {
        mTopNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
        mBtmNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    }
    else
    {
        mTopNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Self);
        mBtmNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Self);
    }
}

void DualScreenTask::pauseCalcRec(bool b)
{
    if(b == false)
    {
        mBothNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    }
    else
    {
        mBothNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Both);
    }
}

void DualScreenTask::pauseDraw(bool b)
{
    if(b == false)
    {
        mTopNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
        mBtmNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_None);
    }
    else
    {
        mTopNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Both);
        mBtmNode.setPauseFlag(MethodTreeNode::PauseFlag::cPause_Both);
    }
}

void DualScreenTask::attachCalcImpl()
{
    ScopedLock<CriticalSection> lock(getMethodTreeMgr()->getTreeCriticalSection());

    TaskBase* parentTask = nullptr;
    if (parent() != nullptr)
        parentTask = *parent()->val();

    if (getTag() == cSystem)
    {
        attachMethodWithCheck(cMethodType_SystemCalc, &mBothNode);
    }
    else
    {
        SEAD_ASSERT_MSG(getTag() == cApp, "Undefined Tag(%d).", mTag);

        if (parentTask == nullptr)
        {
            attachMethodWithCheck(cMethodType_AppCalc, &mBothNode);
        }
        else
        {
            SEAD_ASSERT_MSG(parentTask->isConnectable(this),
                             "illigal parent ( MethodTreeMgr matching failed ) .");

            MethodTreeNode* calcNode = parentTask->getMethodTreeNode(cMethodType_AppCalc);
            SEAD_ASSERT(calcNode);

            calcNode->pushBackChild(&mBothNode);
        }
    }
}

void DualScreenTask::attachDrawImpl()
{
    ScopedLock<CriticalSection> lock(getMethodTreeMgr()->getTreeCriticalSection());

    TaskBase* parentTask = nullptr;
    if (parent() != nullptr)
        parentTask = *parent()->val();

    if (getTag() == cSystem)
    {
        attachMethodWithCheck(cMethodType_SystemDrawTop, &mTopNode);
        attachMethodWithCheck(cMethodType_SystemDrawBtm, &mBtmNode);
    }
    else
    {
        SEAD_ASSERT_MSG(mTag == cApp, "Undefined Tag(%d).", mTag);

        if (parentTask == nullptr)
        {
            attachMethodWithCheck(cMethodType_AppDrawTop, &mTopNode);
            attachMethodWithCheck(cMethodType_AppDrawBtm, &mBtmNode);
        }
        else
        {
            SEAD_ASSERT_MSG(parentTask->isConnectable(this),
                             "illigal parent ( MethodTreeMgr matching failed ) .");

            MethodTreeNode* topDraw = parentTask->getMethodTreeNode(cMethodType_AppDrawTop);
            SEAD_ASSERT(topDraw);
            topDraw->pushFrontChild(&mTopNode);

            MethodTreeNode* btmDraw = parentTask->getMethodTreeNode(cMethodType_AppDrawBtm);
            SEAD_ASSERT(btmDraw);
            btmDraw->pushFrontChild(&mBtmNode);
        }
    }
}

void DualScreenTask::detachCalcImpl()
{
    mBothNode.detachAll();
}

void DualScreenTask::detachDrawImpl()
{
    mTopNode.detachAll();
    mBtmNode.detachAll();
}



}