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

#include "SANEngineTest.hpp"

using namespace TuxedoCat;

SANEngineTest::SANEngineTest()
    : kiwipete("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1")
{
}

TEST_F(SANEngineTest, calculateNotation_shouldCalculatePawnAdvance)
{
    Move m1 {{Color::WHITE, Rank::PAWN, 0x0200ULL}, 0x020000ULL, Rank::NONE};
    Move m2 {{Color::BLACK, Rank::PAWN, 0x0004000000000000ULL},
        0x0000000400000000ULL, Rank::NONE};

    std::string result1 {engine.calculateNotation(kiwipete, m1)};

    kiwipete.makeMove(m1);

    std::string result2 {engine.calculateNotation(kiwipete, m2)};

    EXPECT_EQ("b3", result1);
    EXPECT_EQ("c5", result2);
}
