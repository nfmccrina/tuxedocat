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

#include "Piece.hpp"
#include <boost/algorithm/string.hpp>
#include <cctype>

TuxedoCat::Piece::Piece(Color c, Rank r, Square s)
{
    setColor(c);
    setRank(r);
    setLocation(s);
}

TuxedoCat::Color TuxedoCat::Piece::getColor() const
{
    return this->color;
}

TuxedoCat::Rank TuxedoCat::Piece::getRank() const
{
    return this->rank;
}

TuxedoCat::Square TuxedoCat::Piece::getLocation() const
{
    return this->location;
}

void TuxedoCat::Piece::setColor(Color value)
{
    this->color = value;
}

void TuxedoCat::Piece::setRank(Rank value)
{
    this->rank = value;
}

void TuxedoCat::Piece::setLocation(Square value)
{
    this->location = value;
}

std::string TuxedoCat::Piece::toString() const
{
    std::string chRank = rankToString(rank);

    if (color == Color::WHITE)
    {
        boost::to_upper(chRank);
    }

    return chRank;
}
