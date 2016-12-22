/*
* The MIT License (MIT)
*
* Copyright (c) 2016 Nathan McCrina
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the
* following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
* NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
* USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "../include/Bitboard.hpp"
#include "../include/LookupData.hpp"
#include "../include/BitboardConversionException.hpp"
#include <sstream>

// begin constructors

TuxedoCat::Bitboard::Bitboard()
    : bitboard(0x0000000000000000ULL)
{
}

TuxedoCat::Bitboard::Bitboard(uint64_t num)
    : bitboard(num)
{
}

// end constructors

// begin accessors/mutators

uint64_t TuxedoCat::Bitboard::getValue() const
{
    return bitboard;
}

void TuxedoCat::Bitboard::setValue(uint64_t value)
{
    bitboard = value;
}

// end accessors/mutators

// begin public methods

int TuxedoCat::Bitboard::lsb() const
{
    /*
    * begin wiki code
    */

    if (bitboard == 0x0000000000000000ULL)
    {
            return -1;
    }

    uint64_t debruijn64 = 0x03F79D71B4CB0A89ULL;
    return LookupData::index64[((bitboard & ((~bitboard) + 1)) * debruijn64) >> 58];

    /*
    * end wiki code
    */
}

int TuxedoCat::Bitboard::msb() const
{
    /*
    * begin wiki code
    */

    uint64_t x = bitboard;
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

int TuxedoCat::Bitboard::popcount() const
{
    /*
    * begin wiki code
    */

    uint64_t x = bitboard;

    x = x - ((x >> 1) & LookupData::k1);
    x = (x & LookupData::k2) + ((x >> 2) & LookupData::k2);
    x = (x + (x >> 4)) & LookupData::k4;
    x = (x * LookupData::kf) >> 56;

    return (int)x;

    /*
    * end wiki code
    */
}

std::string TuxedoCat::Bitboard::toAlgebraicCoordinate() const
{
    std::string msg {"Bitboard::toAlgebraicCoordinate: Bitboard must only have one bit set to generate an algebraic coordinate"};
    Bitboard mask {0x00000000000000FFULL};
    int rank {0};
    int file {0};

    if (popcount() != 1)
    {
        throw BitboardConversionException(msg);
    }

    while ((bitboard & mask) == 0x00ULL)
    {
        rank++;
        mask <<= 8;
    }

    mask.setValue(0x0101010101010101ULL);

    while ((bitboard & mask) == 0x00ULL)
    {
        file++;
        mask <<= 1;
    }

    return std::string(1, static_cast<char>(file + 97)) +
        std::to_string(rank + 1);
}

std::pair<int, int> TuxedoCat::Bitboard::toCoordinates() const
{
    std::string msg {"Bitboard::toCoordinates: Bitboard must only have one bit set to generate a coordinate"};
    Bitboard mask {0x00000000000000FFULL};
    int rank {0};
    int file {0};

    if (popcount() != 1)
    {
        throw BitboardConversionException(msg);
    }

    while ((bitboard & mask) == 0x00ULL)
    {
        rank++;
        mask <<= 8;
    }

    mask.setValue(0x0101010101010101ULL);

    while ((bitboard & mask) == 0x00ULL)
    {
        file++;
        mask <<= 1;
    }

    return std::pair<int, int>(rank, file);
}

std::string TuxedoCat::Bitboard::toString() const
{
    std::stringstream ss;
    uint64_t currentValue {0x0100000000000000ULL};

    for (int row = 7; row >= 0; row--)
    {
        for (int col = 0; col < 8; col++)
        {
            if ((currentValue & bitboard) != 0x00ULL)
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

// end public methods

// begin operator overloads

TuxedoCat::Bitboard TuxedoCat::Bitboard::operator~()
{
    return Bitboard(~bitboard);
}

TuxedoCat::Bitboard& TuxedoCat::Bitboard::operator&=(Bitboard other)
{
    bitboard = bitboard & other.getValue();
    return *this;
}

TuxedoCat::Bitboard& TuxedoCat::Bitboard::operator|=(Bitboard other)
{
    bitboard = bitboard | other.getValue();
    return *this;
}

TuxedoCat::Bitboard TuxedoCat::operator&(Bitboard a, Bitboard b)
{
    return Bitboard(a.getValue() & b.getValue());
}

TuxedoCat::Bitboard TuxedoCat::operator|(Bitboard a, Bitboard b)
{
    return Bitboard(a.getValue() | b.getValue());
}

bool TuxedoCat::operator==(Bitboard a, Bitboard b)
{
    return a.getValue() == b.getValue();
}

bool TuxedoCat::operator!=(Bitboard a, Bitboard b)
{
    return !(a.getValue() == b.getValue());
}

bool TuxedoCat::operator>(Bitboard a, Bitboard b)
{
    return a.getValue() > b.getValue();
}

bool TuxedoCat::operator<(Bitboard a, Bitboard b)
{
    return a.getValue() < b.getValue();
}

bool TuxedoCat::operator>=(Bitboard a, Bitboard b)
{
    return a.getValue() > b.getValue() || a.getValue() == b.getValue();
}

bool TuxedoCat::operator<=(Bitboard a, Bitboard b)
{
    return a.getValue() < b.getValue() || a.getValue() == b.getValue();
}

TuxedoCat::Bitboard TuxedoCat::operator<<(Bitboard a, int num)
{
    if (num < 0)
    {
        return a;
    }
    else
    {
        return Bitboard(a.getValue() << num);
    }
}

TuxedoCat::Bitboard TuxedoCat::operator>>(Bitboard a, int num)
{
    if (num < 0)
    {
        return a;
    }
    else
    {
        return Bitboard(a.getValue() >> num);
    }
}

TuxedoCat::Bitboard& TuxedoCat::Bitboard::operator<<=(int num)
{
    if (num >= 0)
    {
        bitboard <<= num;
    }

    return *this;
}

TuxedoCat::Bitboard& TuxedoCat::Bitboard::operator>>=(int num)
{
    if (num >= 0)
    {
        bitboard >>= num;
    }

    return *this;
}

// end operator overloads
