#include <cstdlib>
#include <limits>

#ifdef CTRSDK
    #include <nn/os.h>
#endif

#include <time/seadTickSpan.h>

namespace sead
{
#ifdef CTRSDK
    const s64 TickSpan::cFrequency = nn::os::Tick::GetSystemTick();
#else
#error "Unknown platform"
#endif

s64 TickSpan::toNanoSeconds() const
{
    //multiplyAndDivide_();
}

void TickSpan::setNanoSeconds(s64 nsec)
{

}
}  // namespace sead
