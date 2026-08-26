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
    //ScopedLock(getMethodTreeMgr::getTreeCriticalSection());
    //if(mTaskListNode.parent() == NULL)
}

}