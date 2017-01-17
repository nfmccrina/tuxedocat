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
#include "../include/Utility.hpp"

using namespace TuxedoCat;

Move::Move()
    : promotedRank(Rank::NONE)
{
}

Move::Move(Piece mp, uint64_t tl, Rank pr)
    : movingPiece(mp), targetSquare(tl), promotedRank(pr)
{
}

bool Move::isCastle() const
{
    const Piece& mp {movingPiece};

    return isValid() &&
        mp.rank == Rank::KING &&
        (
        (
        mp.color == Color::WHITE && mp.square == 0x10ULL &&
        Utility::inMask(targetSquare, 0x0000000000000044ULL)
        ) ||
        (
        mp.color == Color::BLACK && mp.square == 0x1000000000000000ULL &&
        Utility::inMask(targetSquare, 0x4400000000000000ULL)
        )
        );
}

bool Move::isValid() const
{
    if (!movingPiece.isValid())
    {
        return false;
    }

    if (targetSquare == 0x00)
    {
        return false;
    }

    if (movingPiece.rank == Rank::PAWN &&
        movingPiece.color == Color::WHITE &&
        Utility::inMask(targetSquare, 0xFF00000000000000ULL) &&
        promotedRank == Rank::NONE)
    {
        return false;
    }

    if (movingPiece.rank == Rank::PAWN &&
        movingPiece.color == Color::BLACK &&
        Utility::inMask(targetSquare, 0x00000000000000FFULL) &&
        promotedRank == Rank::NONE)
    {
        return false;
    }

    return true;
}
