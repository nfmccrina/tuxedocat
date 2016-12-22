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

#include "Square.hpp"
#include "../include/InvalidSquareException.hpp"

// begin constructors

TuxedoCat::Square::Square(std::string s)
{
    int rank {0};
    int file {0};
    std::string msg = "Square(std::string): could not convert " + s +
        " to a square";
    InvalidSquareException ex(msg);

    if (!isAlgebraicDescValid(s))
    {
        throw ex;
    }

    file = static_cast<int>(s[0]);

    if (file > 96 && file < 105)
    {
        file -= 96;
    }
    else if (file > 64 && file < 73)
    {
        file -= 64;
    }
    else
    {
        throw ex;
    }

    rank = std::stoi(s.substr(1)) - 1;

    this->location = Bitboard(0x0000000000000001ULL << ((rank * 8) + file));
}

TuxedoCat::Square::Square(std::pair<int, int> coord)
{
    std::string msg = "Square(std::pair<int, int>): could not convert (" +
        std::to_string(coord.first) + ", " +
        std::to_string(coord.second) + ") to a square";
    InvalidSquareException ex(msg);
    
    if (!areCoordinatesValid(coord))
    {
        throw ex;
    }

    this->location = 0x01;
    this->location <<= (coord.first * 8);
    this->location <<= coord.second;
}

TuxedoCat::Square::Square(Bitboard bitboard)
{
    std::string exMsg {"Square(Bitboard): could not convert bitboard to square"};

    if (!isBitboardValid(bitboard))
    {
        throw InvalidSquareException(exMsg);
    }

    location = bitboard;
}

// end constructors

// begin public methods

TuxedoCat::Bitboard TuxedoCat::Square::toBitboard() const
{
    return location;
}

std::pair<int, int> TuxedoCat::Square::toCoordinates() const
{
    return location.toCoordinates();
}

std::string TuxedoCat::Square::toString() const
{
    return location.toAlgebraicCoordinate();
}

// end public methods

// begin private methods

bool TuxedoCat::Square::areCoordinatesValid(std::pair<int, int> coord) const
{
    return coord.first >= 0 && coord.first < 8 &&
        coord.second >= 0 && coord.second < 8;
}

bool TuxedoCat::Square::isAlgebraicDescValid(std::string s) const
{
    return s.find_first_of("abcdefghABCDEFGH") == 0 &&
        s.find_first_of("12345678") == 1;
}

bool TuxedoCat::Square::isBitboardValid(Bitboard b) const
{
    return b.popcount() == 1;
}

// end private methods
