#pragma once

#include <Arduino.h>

#ifdef LILYGO_DEBUG
namespace
{
template<typename T>
static void DBGX_PLAIN(T last)
{
    LILYGO_DEBUG.println(last);
}

template<typename T, typename... Args>
static void DBGX_PLAIN(T head, Args... tail)
{
    LILYGO_DEBUG.print(head);
    LILYGO_DEBUG.print(' ');
    DBG_PLAIN(tail...);
}

template<typename... Args>
static void DBGX(Args... args)
{
    LILYGO_DEBUG.print("[");
    LILYGO_DEBUG.print(millis());
    LILYGO_DEBUG.print("] ");
    DBG_PLAIN(args...);
}
}
#else
#define DBGX_PLAIN(...)
#define DBGX(...)
#endif