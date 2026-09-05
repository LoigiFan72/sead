#pragma once

#ifdef CTRSDK
    #include <nn/os.h>
#endif

#include <basis/seadTypes.h>
#include <time/seadTickSpan.h>

namespace sead
{
/// A TickTime represents an instant in time.
class TickTime
{
public:
    TickTime() { setNow(); }

    u64 toTicks() const { return mTick; }

#ifdef CTRSDK
    void setNow() { mTick = nn::os::Tick::GetSystemTick(); }
#else
    void setNow();
#endif

    TickSpan diff(const TickTime& other) const { return s64(mTick - other.mTick); }
    TickSpan diffToNow() const;

    TickTime& operator+=(const TickSpan& span)
    {
        mTick += span.toS64();
        return *this;
    }

    TickTime& operator-=(const TickSpan& span)
    {
        mTick -= span.toS64();
        return *this;
    }

private:
    u64 mTick;
};
}  // namespace sead
