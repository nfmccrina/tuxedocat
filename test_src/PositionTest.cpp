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

#include "../test_include/PositionTest.hpp"
#include "../include/Position.hpp"
#include "../include/Move.hpp"
#include "../include/gtest/gtest.h"
#include "boost/optional.hpp"
#include <vector>

using namespace TuxedoCat;

TEST_F(PositionTest,
    generateMoves_PawnCaptures_IfExist_ShouldGenerateCaptures)
{
    Position p("8/8/2n5/3P4/8/8/8/8 w - - 0 1");

    std::vector<Move> captures = p.generateMoves();

    EXPECT_EQ(1, captures.size());

    if (captures.size() > 0)
    {
        EXPECT_EQ(0x0000040000000000ULL,
            captures[0].getTargetSquare().toBitboard());
        EXPECT_EQ(true, Piece(Color::WHITE, Rank::PAWN, Square("d5")) ==
            captures[0].getMovingPiece());
        EXPECT_EQ(boost::none, captures[0].getPromotedRank());
    }
}
