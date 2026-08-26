#include "framework/seadTaskBase.h"
#include "framework/seadTaskMgr.h"

namespace sead
{
TaskBase::CreateArg::CreateArg(){ }

TaskBase::CreateArg::CreateArg(const TaskClassID& factory) : factory(factory) {}

TaskBase::TaskBase(const TaskConstructArg& arg):
    mTaskListNode(), IDisposer(), INamable()
{

}
}  // namespace sead

// For any openEAD Members: Luigifan27 on discord, YOUR PLATFORM SUCKS BOOTY CHEEKS COMPARED TO CTR MWAHAHAHAHAHHAHAHAHAHAHAHAHAHGAHAHAHAHAHAHAHAHHAHA
//
// Also, Mario 3D Land IS better than Oddyessy, fight me.
//
// (All jokes!)