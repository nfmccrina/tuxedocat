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

#include "EngineTest.hpp"
#include "MessageQueue.hpp"
#include "gtest/gtest.h"

using namespace TuxedoCat;

EngineTest::EngineTest()
    : startpos({"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"}, mq),
        kiwipete({"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"}, mq),
        wikiPosition3({"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"}, mq),
        wikiPosition4({"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"}, mq),
        wikiPosition4_mirrored({"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1"},
            mq),
        wikiPosition5({"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"}, mq)
{
}

TEST_F(EngineTest, perft_startpos_perft1)
{
    uint64_t result {startpos.perft(1)};

    EXPECT_EQ(20ULL, result);
}

TEST_F(EngineTest, perft_startpos_perft2)
{
    uint64_t result {startpos.perft(2)};

    EXPECT_EQ(400ULL, result);
}

TEST_F(EngineTest, perft_startpos_perft3)
{
    uint64_t result {startpos.perft(3)};

    EXPECT_EQ(8902ULL, result);
}

TEST_F(EngineTest, perft_startpos_perft4)
{
    uint64_t result {startpos.perft(4)};

    EXPECT_EQ(197281ULL, result);
}

TEST_F(EngineTest, perft_startpos_perft5)
{
    uint64_t result {startpos.perft(5)};

    EXPECT_EQ(4865609ULL, result);
}

TEST_F(EngineTest, perft_startpos_perft6)
{
    uint64_t result {startpos.perft(6)};

    EXPECT_EQ(119060324ULL, result);
}

TEST_F(EngineTest, perft_kiwipete_perft1)
{
    uint64_t result {kiwipete.perft(1)};

    EXPECT_EQ(48ULL, result);
}

TEST_F(EngineTest, perft_kiwipete_perft2)
{
    uint64_t result {kiwipete.perft(2)};

    EXPECT_EQ(2039ULL, result);
}

TEST_F(EngineTest, perft_kiwipete_perft3)
{
    uint64_t result {kiwipete.perft(3)};

    EXPECT_EQ(97862ULL, result);
}

TEST_F(EngineTest, perft_kiwipete_perft4)
{
    uint64_t result {kiwipete.perft(4)};

    EXPECT_EQ(4085603ULL, result);
}

TEST_F(EngineTest, perft_kiwipete_perft5)
{
    uint64_t result {kiwipete.perft(5)};

    EXPECT_EQ(193690690ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition3_perft1)
{
    uint64_t result {wikiPosition3.perft(1)};

    EXPECT_EQ(14ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition3_perft2)
{
    uint64_t result {wikiPosition3.perft(2)};

    EXPECT_EQ(191ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition3_perft3)
{
    uint64_t result {wikiPosition3.perft(3)};

    EXPECT_EQ(2812ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition3_perft4)
{
    uint64_t result {wikiPosition3.perft(4)};

    EXPECT_EQ(43238ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition3_perft5)
{
    uint64_t result {wikiPosition3.perft(5)};

    EXPECT_EQ(674624ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition3_perft6)
{
    uint64_t result {wikiPosition3.perft(6)};

    EXPECT_EQ(11030083ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition3_perft7)
{
    uint64_t result {wikiPosition3.perft(7)};

    EXPECT_EQ(178633661ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition4_perft1)
{
    uint64_t result {wikiPosition4.perft(1)};

    EXPECT_EQ(6ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition4_perft2)
{
    uint64_t result {wikiPosition4.perft(2)};

    EXPECT_EQ(264ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition4_perft3)
{
    uint64_t result {wikiPosition4.perft(3)};

    EXPECT_EQ(9467ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition4_perft4)
{
    uint64_t result {wikiPosition4.perft(4)};

    EXPECT_EQ(422333ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition4_perft5)
{
    uint64_t result {wikiPosition4.perft(5)};

    EXPECT_EQ(15833292ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition4_perft6)
{
    uint64_t result {wikiPosition4.perft(6)};

    EXPECT_EQ(706045033ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition4_mirrored_perft1)
{
    uint64_t result {wikiPosition4_mirrored.perft(1)};

    EXPECT_EQ(6ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition4_mirrored_perft2)
{
    uint64_t result {wikiPosition4_mirrored.perft(2)};

    EXPECT_EQ(264ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition4_mirrored_perft3)
{
    uint64_t result {wikiPosition4_mirrored.perft(3)};

    EXPECT_EQ(9467ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition4_mirrored_perft4)
{
    uint64_t result {wikiPosition4_mirrored.perft(4)};

    EXPECT_EQ(422333ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition4_mirrored_perft5)
{
    uint64_t result {wikiPosition4_mirrored.perft(5)};

    EXPECT_EQ(15833292ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition4_mirrored_perft6)
{
    uint64_t result {wikiPosition4_mirrored.perft(6)};

    EXPECT_EQ(706045033ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition5_perft1)
{
    uint64_t result {wikiPosition5.perft(1)};

    EXPECT_EQ(44ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition5_perft2)
{
    uint64_t result {wikiPosition5.perft(2)};

    EXPECT_EQ(1486ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition5_perft3)
{
    uint64_t result {wikiPosition5.perft(3)};

    EXPECT_EQ(62379ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition5_perft4)
{
    uint64_t result {wikiPosition5.perft(4)};

    EXPECT_EQ(2103487ULL, result);
}

TEST_F(EngineTest, perft_wikiPosition5_perft5)
{
    uint64_t result {wikiPosition5.perft(5)};

    EXPECT_EQ(89941194ULL, result);
}