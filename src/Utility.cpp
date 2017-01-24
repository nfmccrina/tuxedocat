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
*/

#include "Utility.hpp"
#include "LookupData.hpp"

using namespace TuxedoCat;

void Utility::split(const std::string &s, char delim,
    std::vector<std::string> &elems)
{
    std::stringstream ss;
    ss.str(s);
    std::string item;

    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
}


std::vector<std::string> Utility::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void Utility::flipBit(uint64_t& value, int bitIndex)
{
    if (bitIndex >= 0 && bitIndex < 64)
    {
        value ^= (0x01ULL << bitIndex);
    }
}

bool Utility::inMask(uint64_t value, uint64_t  mask)
{
    return (value & mask) == value;
}

int Utility::lsb(uint64_t value)
{
    /*
    * begin wiki code
    */

    if (value == 0x0000000000000000ULL)
    {
            return -1;
    }

    uint64_t debruijn64 = 0x03F79D71B4CB0A89ULL;
    return LookupData::index64[((value & ((~value) + 1)) * debruijn64) >> 58];

    /*
    * end wiki code
    */
}

int Utility::msb(uint64_t value)
{
    /*
    * begin wiki code
    */

    uint64_t x {value};
    if (x == 0x0000000000000000ULL)
    {
            return -1;
    }

    uint64_t debruijn64 = 0x03F79D71B4CB0A89ULL;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    return LookupData::index64Reverse[(x * debruijn64) >> 58];

    /*
    * end wiki code
    */
}

int Utility::popcount(uint64_t value)
{
    /*
    * begin wiki code
    */

    uint64_t x = value; 

    x = x - ((x >> 1) & LookupData::k1);
    x = (x & LookupData::k2) + ((x >> 2) & LookupData::k2);
    x = (x + (x >> 4)) & LookupData::k4;
    x = (x * LookupData::kf) >> 56;

    return (int)x;

    /*
    * end wiki code
    */
}

std::string Utility::toAlgebraicCoordinate(uint64_t value)
{
    uint64_t mask {0x00000000000000FFULL};
    int rank {0};
    int file {0};

    if (value == 0x00ULL)
    {
        return "";
    }

    while ((value & mask) == 0x00ULL)
    {
        rank++;
        mask <<= 8;
    }

    mask = 0x0101010101010101ULL;

    while ((value & mask) == 0x00ULL)
    {
        file++;
        mask <<= 1;
    }

    return std::string(1, static_cast<char>(file + 97)) +
        std::to_string(rank + 1);
}

std::pair<int, int> Utility::toCoordinates(uint64_t value)
{
    std::string msg {"Bitboard::toCoordinates: Bitboard must only have one bit set to generate a coordinate"};
    uint64_t mask {0x00000000000000FFULL};
    int rank {1};
    int file {1};

    if (value == 0x00ULL)
    {
        return std::pair<int, int> {-1, -1};
    }

    while ((value & mask) == 0x00ULL)
    {
        rank++;
        mask <<= 8;
    }

    mask = 0x0101010101010101ULL;

    while ((value & mask) == 0x00ULL)
    {
        file++;
        mask <<= 1;
    }

    return std::pair<int, int>(rank, file);
}

std::string Utility::toString(uint64_t value)
{
    std::stringstream ss;
    uint64_t currentValue {0x0100000000000000ULL};

    for (int row = 7; row >= 0; row--)
    {
        for (int col = 0; col < 8; col++)
        {
            if ((currentValue & value) != 0x00ULL)
            {
                ss << "*";
            }
            else
            {
                ss << "-";
            }

            if (col != 7)
            {
                currentValue = currentValue << 1;
                ss << " ";
            }
        }

        if (row != 0)
        {
            ss << std::endl;
        }

        currentValue = currentValue >> 15;
    }

    return ss.str();
}

uint64_t Utility::algebraicToBitboard(std::string s)
{
    int rank {0};
    int file {0};

    if (!Utility::isAlgebraicDescValid(s))
    {
        return 0x00ULL;
    }

    file = static_cast<int>(s[0]);

    if (file > 96 && file < 105)
    {
        file -= 97;
    }
    else if (file > 64 && file < 73)
    {
        file -= 65;
    }
    else
    {
        return 0x00ULL;
    }

    rank = std::stoi(s.substr(1)) - 1;

    return (0x0000000000000001ULL << ((rank * 8) + file));
}

bool Utility::isAlgebraicDescValid(std::string s)
{
    return s.find_first_of("abcdefghABCDEFGH") == 0 &&
        s.find_first_of("12345678") == 1;
}

uint64_t Utility::coordinatesToBitboard(std::pair<int, int> coord)
{
    if (!Utility::areCoordinatesValid(coord))
    {
        return 0x00ULL;
    }
    else
    {
        return 0x01ULL << ((coord.first * 8) + coord.second);
    }
}

bool Utility::areCoordinatesValid(std::pair<int, int> coord)
{
    return coord.first >= 0 && coord.first < 8 && coord.second >= 0
        && coord.second < 8;
}
