#pragma once

#include <functional>

template <typename T>
T const& clamp(T const& value, T const& low, T const& high){
    if(value < low) return low;
    if(high < value) return high;
    return value;
}

template <typename T, typename Compare>
T const& clamp(T const& value, T const& low, T const& high, Compare cmp){
    if(cmp(value,low)) return low;
    if(cmp(high,value)) return high;
    return value;
}