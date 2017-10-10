#ifndef UTILS_H
#define UTILS_H
#include <numeric>

template <typename T>
T clamp(T val, T low, T high)
{
    if (val < low) return low;
    else if (val > high) return high;
    else return val;
}
#endif