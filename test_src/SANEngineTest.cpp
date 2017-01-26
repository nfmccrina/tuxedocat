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

    std::string result1 {SANEngine::calculateNotation(kiwipete, m1)};

    kiwipete.makeMove(m1);

    std::string result2 {SANEngine::calculateNotation(kiwipete, m2)};

    EXPECT_EQ("b3", result1);
    EXPECT_EQ("c5", result2);
}

TEST_F(SANEngineTest, calculateNotation_shouldCalculatePawnCapture)
{
    Move m1 {{Color::WHITE, Rank::PAWN, 0x4000ULL}, 0x800000ULL, Rank::NONE};
    Move m2 {{Color::BLACK, Rank::PAWN, 0x100000000000ULL},
        0x0800000000ULL, Rank::NONE};

    std::string result1 {SANEngine::calculateNotation(kiwipete, m1)};

    kiwipete.makeMove(m1);

    std::string result2 {SANEngine::calculateNotation(kiwipete, m2)};

    EXPECT_EQ("gxh3", result1);
    EXPECT_EQ("exd5", result2);
}

TEST_F(SANEngineTest, calculateNotation_shouldCalculateCastle)
{
    Move m1 {{Color::WHITE, Rank::KING, 0x10ULL}, 0x40ULL, Rank::NONE};
    Move m2 {{Color::BLACK, Rank::KING, 0x1000000000000000ULL},
        0x0400000000000000ULL, Rank::NONE};

    std::string result1 {SANEngine::calculateNotation(kiwipete, m1)};

    kiwipete.makeMove(m1);

    std::string result2 {SANEngine::calculateNotation(kiwipete, m2)};

    EXPECT_EQ("0-0", result1);
    EXPECT_EQ("0-0-0", result2);
}

TEST_F(SANEngineTest, calculateNotation_shouldCalculateKnightCapture)
{
    Move m1 {{Color::BLACK, Rank::KNIGHT, 0x200000000000ULL},
        0x0800000000ULL, Rank::NONE};

    kiwipete.makeMove({{}, 0x00ULL, Rank::NONE});
    std::string result1 {SANEngine::calculateNotation(kiwipete, m1)};

    EXPECT_EQ("Nfxd5", result1);
}

TEST_F(SANEngineTest, calculateNotation_shouldCalculateRookMove)
{
    Move m1 {{Color::WHITE, Rank::KING, 0x10ULL}, 0x40ULL, Rank::NONE};
    Move m2 {{Color::WHITE, Rank::ROOK, 0x01ULL}, 0x04ULL, Rank::NONE};

    kiwipete.makeMove(m1);
    kiwipete.makeMove({{}, 0x00ULL, Rank::NONE});

    std::string result1 {SANEngine::calculateNotation(kiwipete, m2)};

    EXPECT_EQ("Rac1", result1);
}

TEST_F(SANEngineTest, calculateNotation_shouldCalculatePawnPromotion)
{
    Move m1 {{Color::BLACK, Rank::PAWN, 0x800000ULL}, 0x4000ULL, Rank::NONE};
    Move m2 {{Color::WHITE, Rank::QUEEN, 0x200000ULL},
        0x20000000ULL, Rank::NONE};
    Move m3 {{Color::BLACK, Rank::PAWN, 0x4000ULL}, 0x40ULL, Rank::ROOK};

    std::string result1 {SANEngine::calculateNotation(kiwipete, m1)};

    kiwipete.makeMove(m1);

    std::string result2 {SANEngine::calculateNotation(kiwipete, m2)};

    kiwipete.makeMove(m2);

    std::string result3 {SANEngine::calculateNotation(kiwipete, m3)};

    EXPECT_EQ("hxg2", result1);
    EXPECT_EQ("Qf4", result2);
    EXPECT_EQ("g1=R", result3);
}
