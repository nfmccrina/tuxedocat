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

#include "CastlingStatus.hpp"

TuxedoCat::CastlingStatus::CastlingStatus(bool wks, bool wqs, bool bks,
    bool bqs)
{
    setWhiteKingSide(wks);
    setWhiteQueenSide(wqs);
    setBlackKingSide(bks);
    setBlackQueenSide(bqs);
}

bool TuxedoCat::CastlingStatus::getWhiteKingSide() const
{
    return this->whiteKingSide;
}

bool TuxedoCat::CastlingStatus::getWhiteQueenSide() const
{
    return this->whiteQueenSide;
}

bool TuxedoCat::CastlingStatus::getBlackKingSide() const
{
    return this->blackKingSide;
}

bool TuxedoCat::CastlingStatus::getBlackQueenSide() const
{
    return this->blackQueenSide;
}

void TuxedoCat::CastlingStatus::setWhiteKingSide(bool value)
{
    this->whiteKingSide = value;
}

void TuxedoCat::CastlingStatus::setWhiteQueenSide(bool value)
{
    this->whiteQueenSide = value;
}

void TuxedoCat::CastlingStatus::setBlackKingSide(bool value)
{
    this->blackKingSide = value;
}

void TuxedoCat::CastlingStatus::setBlackQueenSide(bool value)
{
    this->blackQueenSide = value;
}
