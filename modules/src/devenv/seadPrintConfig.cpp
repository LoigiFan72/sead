#include "devenv/seadPrintConfig.h"

namespace sead
{
PrintConfig::PrintEventArg PrintConfig::sPrintEvent{};
IDelegate1<const PrintConfig::PrintEventArg&>* PrintConfig::sFinalCallback = nullptr;

void PrintConfig::registerCallback(PrintEventArg::Slot& slot)
{
    sPrintEvent.connect(slot);
}

void PrintConfig::unregisterCallback(PrintEventArg::Slot& slot)
{
    sPrintEvent.disconnect(slot);
}

void PrintConfig::registerFinalCallback(IDelegate1<const PrintEventArg&>* cb)
{
    sFinalCallback = cb;
}

void PrintConfig::execCallbacks(const PrintEventArg& assertMessage)
{
    sPrintEvent.fire(assertMessage);
    if (sFinalCallback)
        sFinalCallback->invoke(assertMessage);
    else
        system::PrintStringImpl(assertMessage);
}
}  // namespace sead
