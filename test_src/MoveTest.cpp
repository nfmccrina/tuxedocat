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

#include "../test_include/MoveTest.hpp"
#include "../include/Move.hpp"
#include "../include/Piece.hpp"
#include "../include/Color.hpp"
#include "../include/Rank.hpp"
#include "../include/Square.hpp"
#include "../include/gtest/gtest.h"

using namespace TuxedoCat;

TEST_F(MoveTest, getMovingPiece_ShouldReturnTheMovingPiece)
{
    Move m(Piece(Color::WHITE, Rank::KNIGHT, Square("b1")),
        Square("a3"), Rank::NONE);

    EXPECT_EQ(Color::WHITE, m.getMovingPiece().getColor());
    EXPECT_EQ(Rank::KNIGHT, m.getMovingPiece().getRank());
    EXPECT_EQ(Bitboard(0x0000000000000002ULL),
        m.getMovingPiece().getSquare().toBitboard());
}

TEST_F(MoveTest, getTargetSquare_ShouldReturnTheTargetSquare)
{
    Move m(Piece(Color::WHITE, Rank::KNIGHT, Square("b1")),
        Square("a3"), Rank::NONE);

    EXPECT_EQ(Bitboard(0x0000000000010000ULL),
        m.getTargetSquare().toBitboard());
}

TEST_F(MoveTest, getPromotedRank_ShouldReturnThePromotedRank)
{
    Move m(Piece(Color::WHITE, Rank::PAWN, Square("b7")),
        Square("b8"), Rank::QUEEN);

    EXPECT_EQ(Rank::QUEEN, m.getPromotedRank());
}

TEST_F(MoveTest, isValid_ShouldReturnFalseIfMemberIsInvalid)
{
    Move m {Piece(), Square {"e2"}, Rank::NONE};

    EXPECT_EQ(false, m.isValid());
}
