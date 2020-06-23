#pragma once



#ifdef LILYGO_DEBUG
namespace
{
template<typename T>
static void DBG_PLAIN(T last)
{
    LILYGO_DEBUG.println(last);
}

template<typename T, typename... Args>
static void DBG_PLAIN(T head, Args... tail)
{
    LILYGO_DEBUG.print(head);
    LILYGO_DEBUG.print(' ');
    DBG_PLAIN(tail...);
}

template<typename... Args>
static void DBG(Args... args)
{
    LILYGO_DEBUG.print("[");
    LILYGO_DEBUG.print(millis());
    LILYGO_DEBUG.print("] ");
    DBG_PLAIN(args...);
}
}
#else
#define DBG_PLAIN(...)
#define DBG(...)
#endif