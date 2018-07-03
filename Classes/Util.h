#pragma once
#include "headers.h"

class UTIL
{
public:
    static int clamp(const int& _x, const int& _low, const int& _high)
    {
        assert(_low <= _high);

        return std::min(std::max(_x, _low), _high);
    }

    static int wrap(const int& _x, const int& _low, const int& _high)
    {
        assert(_low < _high);
        const int n = (_x - _low) % (_high - _low);
        return n >= 0 ? n + _low : n + _high;
    }

    static int randomRagne(const int& begin, const int& end)
    {
        static random_device rd;
        static mt19937 re(rd());
        uniform_int_distribution<int> range(begin, end);
        
        return range(re);
    }
};