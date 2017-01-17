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
#include "../include/Utility.hpp"
#include "../include/gtest/gtest.h"

using namespace TuxedoCat;

TEST_F(MoveTest, isValid_ShouldReturnFalseIfMemberIsInvalid)
{
    Move m {Piece(), Utility::algebraicToBitboard("e2"), Rank::NONE};
    bool expected = false;

    EXPECT_EQ(expected, m.isValid());
}

TEST_F(MoveTest, isCastle_ShouldReturnTrueIfMoveIsCastle)
{
    Move m1 {
        Piece {Color::WHITE, Rank::KING, Utility::algebraicToBitboard("e1")},
        Utility::algebraicToBitboard("g1"), Rank::NONE};
        
    Move m2 {
        Piece {Color::WHITE, Rank::KING, Utility::algebraicToBitboard("e1")},
        Utility::algebraicToBitboard("c1"), Rank::NONE};

    Move m3 {
        Piece {Color::BLACK, Rank::KING, Utility::algebraicToBitboard("e8")},
        Utility::algebraicToBitboard("g8"), Rank::NONE};

    Move m4 {
        Piece {Color::BLACK, Rank::KING, Utility::algebraicToBitboard("e8")},
        Utility::algebraicToBitboard("c8"), Rank::NONE};

    bool expected = true;

    EXPECT_EQ(expected, m1.isCastle());
    EXPECT_EQ(expected, m2.isCastle());
    EXPECT_EQ(expected, m3.isCastle());
    EXPECT_EQ(expected, m4.isCastle());
}

TEST_F(MoveTest, isCastle_ShouldReturnFalseIfMoveIsNotCastle)
{
    Move m1 {
        Piece {Color::BLACK, Rank::KING, Utility::algebraicToBitboard("e1")},
        Utility::algebraicToBitboard("g1"), Rank::NONE};
        
    Move m2 {
        Piece {Color::WHITE, Rank::ROOK, Utility::algebraicToBitboard("e1")},
        Utility::algebraicToBitboard("c1"), Rank::NONE};

    bool expected = false;

    EXPECT_EQ(expected, m1.isCastle());
    EXPECT_EQ(expected, m2.isCastle());
}
