#pragma once

#include "container/seadListImpl.h"
#include "controller/seadControllerDefine.h"
#include "prim/seadRuntimeTypeInfo.h"

namespace sead
{
class ControllerMgr;

class ControlDevice
{
    SEAD_RTTI_BASE(ControlDevice)

public:
#ifdef CTRSDK
    explicit ControlDevice(ControllerMgr* mgr) : mId(ControllerDefine::cDevice_CtrHid), mMgr(mgr) {}
#else
    explicit ControlDevice(ControllerMgr* mgr) : mId(ControllerDefine::cDevice_Null), mMgr(mgr) {}
#endif
    virtual ~ControlDevice(){};
    virtual void calc() = 0;

    ControllerDefine::DeviceId getId() const { return mId; }

protected:
    ListNode mListNode;
    ControllerDefine::DeviceId mId;
    ControllerMgr* mMgr;

    friend class ControllerMgr;
};

}  // namespace sead
