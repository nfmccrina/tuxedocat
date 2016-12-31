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

#include "Move.hpp"

using namespace TuxedoCat;

Move::Move()
    : promotedRank(Rank::NONE)
{
}

Move::Move(Piece mp, Square tl, Rank pr)
    : movingPiece(mp), targetSquare(tl), promotedRank(pr)
{
}

Piece Move::getMovingPiece() const
{
    return this->movingPiece;
}

Square Move::getTargetSquare() const
{
    return this->targetSquare;
}

Rank Move::getPromotedRank() const
{
    return this->promotedRank;
}

bool Move::isCastle() const
{
    const Piece& mp {movingPiece};

    return isValid() &&
        mp.getRank() == Rank::KING &&
        (
        (
        mp.getColor() == Color::WHITE &&
        mp.getSquare().toBitboard().inMask(0x0000000000000010ULL) &&
        targetSquare.toBitboard().inMask(0x0000000000000044ULL)
        ) ||
        (
        mp.getColor() == Color::BLACK &&
        mp.getSquare().toBitboard().inMask(0x1000000000000000ULL) &&
        targetSquare.toBitboard().inMask(0x4400000000000000ULL)
        )
        );
}

bool Move::isValid() const
{
    return movingPiece.isValid() &&
        targetSquare.isValid();
}
