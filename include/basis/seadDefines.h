#pragma once

#define SEAD_NO_COPY(CLASS)                  \
public:                                      \
    CLASS(const CLASS&) = delete;            \
    CLASS& operator=(const CLASS&) = delete

#define SEAD_UNUSED(VARIABLE) static_cast<void>(VARIABLE)