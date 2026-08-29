#include "framework/seadTaskBase.h"
#include "framework/seadTaskMgr.h"
#include "hostio/seadHostIOMgr.h"
#include "hostio/seadHostIOEventListener.h"

namespace sead
{
TaskBase::CreateArg::CreateArg(){ }

TaskBase::CreateArg::CreateArg(const TaskClassID& factory) : factory(factory) {}

TaskBase::TaskBase(const TaskConstructArg& arg): 
    TTreeNode<TaskBase*>(this), IDisposer(), INamable(), mParameter(arg.param), 
    mInternalFlag(0), mTaskListNode(this), mHeapArray(*arg.heap_array), 
    mTaskMgr(arg.mgr), mClassID(), mTag(cSystem), mState(cPrepare)
{
    SafeString name;
    setName(name);
}

TaskBase::TaskBase(const TaskConstructArg& arg, const char* name): 
    TTreeNode<TaskBase*>(this), IDisposer(), INamable(), mParameter(arg.param), 
    mInternalFlag(0), mTaskListNode(this), mHeapArray(*arg.heap_array), 
    mTaskMgr(arg.mgr), mClassID(), mTag(cSystem), mState(cPrepare)
{
    setName(name);
}

void TaskBase::pauseCalcChild(bool b)
{
}

void TaskBase::pauseDrawChild(bool b)
{
}

void TaskBase::taskListenPropertyEvent(const hostio::PropertyEvent* event)
{
}

void TaskBase::taskGenMessage(hostio::Context* ctx)
{
}

void TaskBase::prepare()
{
}

void TaskBase::enterCommon()
{
    TaskBase::attachCalcImpl();
    pauseCalc(false);
    pauseDraw(false);
    enter();
}

void TaskBase::enter()
{
}

void TaskBase::exit()
{
}

void TaskBase::onEvent(const TaskEvent&)
{
}

void TaskBase::onDestroy()
{
    TaskBase::doneDestroy();
}

void TaskBase::doneDestroy()
{
// todo rest
}
}  // namespace sead

// For any openEAD Members: Luigifan27 on discord, YOUR PLATFORM SUCKS BOOTY CHEEKS COMPARED TO CTR MWAHAHAHAHAHHAHAHAHAHAHAHAHAHGAHAHAHAHAHAHAHAHHAHA
//
// Also, Mario 3D Land IS better than Oddyessy, fight me.
//
// (All jokes!)