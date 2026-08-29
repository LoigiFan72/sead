#pragma once

#include "prim/seadDelegate.h"
#include "prim/seadDelegateEventSlot.h"

namespace sead
{
class PrintConfig
{
public:
    using PrintEventArg = DelegateEvent<const PrintEventArg&>;

    static void registerCallback(PrintEventArg::Slot& slot);
    static void unregisterCallback(PrintEventArg::Slot& slot);
    static void registerFinalCallback(IDelegate1<const PrintEventArg&>* cb);
    static void execCallbacks(const PrintEventArg& assertMessage);

private:
    static PrintEventArg sPrintEvent;
    static IDelegate1<const PrintEventArg&>* sFinalCallback;
};
}  // namespace sead
