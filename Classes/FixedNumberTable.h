﻿#pragma once

namespace crd
{
    const int I[][4] = { { 6, 5, 4, 3 },{ 20, 20, 20, 20 } };
    const int J[][4] = { { 3, 4, 5, 5 },{ 21, 21, 21, 20 } };
    const int L[][4] = { { 6, 5, 4, 4 },{ 21, 21, 21, 20 } };
    const int S[][4] = { { 6, 5, 5, 4 },{ 21, 21, 20, 20 } };
    const int Z[][4] = { { 4, 5, 5, 6 },{ 21, 21, 20, 20 } };
    const int T[][4] = { { 4, 5, 6, 5 },{ 21, 21, 21, 20 } };
    const int O[][4] = { { 4, 5, 4, 5 },{ 21, 21, 20, 20 } };
}

namespace loc
{
    const int I[][4] = { { 1, 0,-1,-2 },{ 0, 0, 0, 0 } };
    const int J[][4] = { { -1, 0, 1, 1 },{ 0, 0, 0,-1 } };
    const int L[][4] = { { 1, 0,-1,-1 },{ 0, 0, 0,-1 } };
    const int S[][4] = { { 1, 0, 0,-1 },{ 0, 0,-1,-1 } };
    const int Z[][4] = { { -1, 0, 0, 1 },{ 0, 0,-1,-1 } };
    const int T[][4] = { { -1, 0, 1, 0 },{ 0, 0, 0,-1 } };
    const int O[][4] = { { -1, 0,-1, 0 },{ 0, 0,-1,-1 } };
}

namespace fnt
{
    const int I[][8] = 
    {
        { 1,-1, 0, 0,-1, 1,-2, 2 },
        {-1, 1, 0, 0, 1,-1, 2,-2 }
    };

    const int J[][8] = 
    {
        {-1,-1, 0, 0, 1, 1, 2, 0 },
        { 1,-1, 0, 0,-1, 1, 0, 2 },
        { 1, 1, 0, 0,-1,-1,-2, 0 },
        {-1, 1, 0, 0, 1,-1, 0,-2 }
    };
    
    const int L[][8] = 
    {
        { 1, 1, 0, 0,-1,-1, 0,-2 },
        {-1, 1, 0, 0, 1,-1, 2, 0 },
        {-1,-1, 0, 0, 1, 1, 0, 2 },
        { 1,-1, 0, 0,-1, 1,-2, 0 }
    };

    const int S[][8] = 
    {
        { 1,-1, 0, 0,-1,-1,-2, 0 },
        {-1, 1, 0, 0, 1, 1, 2, 0 }
    };

    const int T[][8] = 
    {
        {-1,-1, 0, 0, 1, 1, 1,-1 },
        { 1,-1, 0, 0,-1, 1, 1, 1 },
        { 1, 1, 0, 0,-1,-1,-1, 1 },
        {-1, 1, 0, 0, 1,-1,-1,-1 }
    };

    const int Z[][8] = 
    {
        {-1, 1, 0, 0,-1,-1, 0,-2 },
        { 1,-1, 0, 0, 1, 1, 0, 2 }
    };

    const int O[][8] = 
    {
        { 0, 0, 0, 0, 0, 0, 0, 0 }
    };
}