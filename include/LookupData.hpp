/*
* The MIT License (MIT)
*
* Copyright (c) 2016 Nathan McCrina
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation 
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
*/

#pragma once

#include "Direction.hpp"
#include <cstdint>
#include <array>

namespace TuxedoCat
{
    class LookupData
    {
    public:
        static const std::array<uint64_t, 64>& getRayAttacksByDirection(
            Direction dir);
        static const std::array<uint64_t, 64> knightAttacks;
        static const std::array<uint64_t, 64> kingAttacks;
        static const std::array<uint64_t, 64> rayAttacksN;
        static const std::array<uint64_t, 64> rayAttacksS;
        static const std::array<uint64_t, 64> rayAttacksE;
        static const std::array<uint64_t, 64> rayAttacksW;
        static const std::array<uint64_t, 64> rayAttacksNE;
        static const std::array<uint64_t, 64> rayAttacksNW;
        static const std::array<uint64_t, 64> rayAttacksSE;
        static const std::array<uint64_t, 64> rayAttacksSW;
        static const std::array<uint64_t, 64> fileMask;
        static const std::array<uint64_t, 64> rankMask;
        static const std::array<uint64_t, 64> swneMask;
        static const std::array<uint64_t, 64> nwseMask;
        static const std::array<std::array<uint64_t, 64>, 64> betweenLookup;
        static const int index64[64];
        static const int index64Reverse[64];
        static const uint64_t k1;
        static const uint64_t k2;
        static const uint64_t k4;
        static const uint64_t kf;
    };
}
