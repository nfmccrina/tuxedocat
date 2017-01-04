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

#include "../test_include/PieceTest.hpp"
#include "../include/Piece.hpp"
#include "../include/Color.hpp"
#include "../include/Rank.hpp"
#include "../include/Square.hpp"
#include "../include/gtest/gtest.h"

using namespace TuxedoCat;

TEST_F(PieceTest, toString_ShouldReturnCorrectRepresentationForWhitePawn)
{
    Piece p(Color::WHITE, Rank::PAWN, Square("b1"));

    EXPECT_EQ("P", p.toString());
}

TEST_F(PieceTest, toString_ShouldReturnCorrectRepresentationForBlackPawn)
{
    Piece p(Color::BLACK, Rank::PAWN, Square("b1"));

    EXPECT_EQ("p", p.toString());
}

TEST_F(PieceTest, toString_ShouldReturnCorrectRepresentationForWhiteKnight)
{
    Piece p(Color::WHITE, Rank::KNIGHT, Square("b1"));

    EXPECT_EQ("N", p.toString());
}

TEST_F(PieceTest, toString_ShouldReturnCorrectRepresentationForBlackKnight)
{
    Piece p(Color::BLACK, Rank::KNIGHT, Square("b1"));

    EXPECT_EQ("n", p.toString());
}

TEST_F(PieceTest, toString_ShouldReturnCorrectRepresentationForWhiteBishop)
{
    Piece p(Color::WHITE, Rank::BISHOP, Square("b1"));

    EXPECT_EQ("B", p.toString());
}

TEST_F(PieceTest, toString_ShouldReturnCorrectRepresentationForBlackBishop)
{
    Piece p(Color::BLACK, Rank::BISHOP, Square("b1"));

    EXPECT_EQ("b", p.toString());
}

TEST_F(PieceTest, toString_ShouldReturnCorrectRepresentationForWhiteRook)
{
    Piece p(Color::WHITE, Rank::ROOK, Square("b1"));

    EXPECT_EQ("R", p.toString());
}

TEST_F(PieceTest, toString_ShouldReturnCorrectRepresentationForBlackRook)
{
    Piece p(Color::BLACK, Rank::ROOK, Square("b1"));

    EXPECT_EQ("r", p.toString());
}

TEST_F(PieceTest, toString_ShouldReturnCorrectRepresentationForWhiteQueen)
{
    Piece p(Color::WHITE, Rank::QUEEN, Square("b1"));

    EXPECT_EQ("Q", p.toString());
}

TEST_F(PieceTest, toString_ShouldReturnCorrectRepresentationForBlackQueen)
{
    Piece p(Color::BLACK, Rank::QUEEN, Square("b1"));

    EXPECT_EQ("q", p.toString());
}

TEST_F(PieceTest, toString_ShouldReturnCorrectRepresentationForWhiteKing)
{
    Piece p(Color::WHITE, Rank::KING, Square("b1"));

    EXPECT_EQ("K", p.toString());
}

TEST_F(PieceTest, toString_ShouldReturnCorrectRepresentationForBlackKing)
{
    Piece p(Color::BLACK, Rank::KING, Square("b1"));

    EXPECT_EQ("k", p.toString());
}

TEST_F(PieceTest, getSquare_ShouldReturnTheCorrectSquare)
{
    Piece p(Color::BLACK, Rank::QUEEN, Square("d6"));

    EXPECT_EQ(Bitboard(0x0000080000000000ULL), p.getSquare().toBitboard());
}

TEST_F(PieceTest, getColor_ShouldReturnTheCorrectColor)
{
    Piece p(Color::BLACK, Rank::QUEEN, Square("d6"));

    EXPECT_EQ(Color::BLACK, p.getColor());
}

TEST_F(PieceTest, getRank_ShouldReturnTheCorrectRank)
{
    Piece p(Color::BLACK, Rank::QUEEN, Square("d6"));

    EXPECT_EQ(Rank::QUEEN, p.getRank());
}

TEST_F(PieceTest, operatorEQ_ShouldReturnTrueForIdenticalPieces)
{
    Piece p1(Color::BLACK, Rank::KNIGHT, Square("e4"));
    Piece p2(Color::BLACK, Rank::KNIGHT, Square("e4"));

    bool expected = true;

    EXPECT_EQ(expected, p1 == p2);
}

TEST_F(PieceTest, operatorNEQ_ShouldReturnTrueForIdenticalPieces)
{
    Piece p1(Color::BLACK, Rank::KNIGHT, Square("e4"));
    Piece p2(Color::BLACK, Rank::PAWN, Square("e4"));

    bool expected = true;

    EXPECT_EQ(expected, p1 != p2);
}

TEST_F(PieceTest, isValid_ShouldReturnFalseIfMemberIsNull)
{
    Square s1;
    Square s2 {"e3"};

    Piece p1 {Color::BLACK, Rank::PAWN, s1};
    Piece p2 {Color::WHITE, Rank::NONE, s2};
    Piece p3 {Color::NONE, Rank::KING, s2};

    bool expected = false;

    EXPECT_EQ(expected, p1.isValid());
    EXPECT_EQ(expected, p2.isValid());
    EXPECT_EQ(expected, p3.isValid());
}
