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

TuxedoCat::Square::Square()
{
    this->location = 0x00;
}

TuxedoCat::Square::Square(std::string s)
{
    int rank = 0;
    int file = static_cast<int>(s[0]);

    file -= 97;

    if (std::isdigit(s[1]))
    {
        rank = std::stoi(s.substr(1)) - 1;
    }

    this->location = 0x0000000000000001ULL << ((rank * 8) + file);
}

TuxedoCat::Square::Square(uint8_t row, uint8_t column)
{
    this->location = 0x00;

    if (row < 8 && column < 8)
    {
        this->location = 0x01;

        for (uint8_t count = 0; count < row; count++)
        {
            this->location = this->location << 8;
        }

        for (uint8_t count = 0; count < column; count++)
        {
            this->location = this->location << 1;
        }
    }
}

TuxedoCat::Square::Square(uint64_t bitboard)
{
    this->location = bitboard;
}

uint64_t TuxedoCat::Square::getLocation() const
{
    return this->location;
}
