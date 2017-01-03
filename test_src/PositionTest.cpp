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
#include "../include/MoveList.hpp"
#include "../include/gtest/gtest.h"
#include <vector>

using namespace TuxedoCat;

TEST_F(PositionTest,
    generateMoves_PawnCaptures_IfExistLeft_ShouldGenerateCaptures)
{
    Position p("8/8/2nB4/3P4/8/8/8/8 w - - 0 1");

    MoveList ml;
    p.generateMoves(Rank::PAWN, ml);

    EXPECT_EQ(1, ml.size());

    if (ml.size() > 0)
    {
        EXPECT_EQ(0x0000040000000000ULL,
            ml[0].getTargetSquare().toBitboard());
        EXPECT_EQ(true, Piece(Color::WHITE, Rank::PAWN, Square("d5")) ==
            ml[0].getMovingPiece());
        EXPECT_EQ(Rank::NONE, ml[0].getPromotedRank());
    }
}

TEST_F(PositionTest,
    generateMoves_PawnCaptures_IfExistRight_ShouldGenerateCaptures)
{
    Position p("8/8/3rb3/3P4/8/8/8/8 w - - 0 1");

    MoveList captures;
    p.generateMoves(Rank::PAWN, captures);

    EXPECT_EQ(1, captures.size());

    if (captures.size() > 0)
    {
        EXPECT_EQ(0x0000100000000000ULL,
            captures[0].getTargetSquare().toBitboard());
        EXPECT_EQ(true, Piece(Color::WHITE, Rank::PAWN, Square("d5")) ==
            captures[0].getMovingPiece());
        EXPECT_EQ(Rank::NONE, captures[0].getPromotedRank());
    }
}

TEST_F(PositionTest,
    generateMoves_PawnCaptures_ShouldNotIncludeEPIfNotValid)
{
    Position p("8/p7/8/8/4P3/8/8/8 b - e3 0 1");

    MoveList captures;
    p.generateMoves(Rank::PAWN, captures);

    EXPECT_EQ(2, captures.size());
}

TEST_F(PositionTest,
    generateMoves_PawnCaptures_IfNotExist_ShouldGenerateCaptures)
{
    Position p("8/8/3K4/3P4/8/8/8/8 w - - 0 1");

    MoveList captures;
    p.generateMoves(Rank::PAWN, captures);

    EXPECT_EQ(0, captures.size());
}

TEST_F(PositionTest,
    generateMoves_PawnCaptures_IfInCheck_ShouldOnlyGenerateCapture)
{
    Position p("8/8/8/2b5/1P6/4K3/8/8 w - - 0 1");

    MoveList captures;
    p.generateMoves(Rank::PAWN, captures);

    EXPECT_EQ(1, captures.size());
    
    if (captures.size() > 0)
    {
        EXPECT_EQ(0x0000000400000000ULL, captures[0].getTargetSquare()
            .toBitboard());
    }
}

TEST_F(PositionTest,
    generateMoves_PawnCaptures_IfPromotion_ShouldGenerateCapturePromotions)
{
    Position p("8/8/8/8/8/8/1p6/QN6 b - - 0 1");

    MoveList captures;
    p.generateMoves(Rank::PAWN, captures);

    Piece mp(Color::BLACK, Rank::PAWN, Square("b2"));
    Square s("a1");
    bool hasQueen = false;
    bool hasRook = false;
    bool hasBishop = false;
    bool hasKnight = false;

    EXPECT_EQ(4, captures.size());

    if (captures.size() > 0)
    {
        for (size_t count = 0; count < captures.size(); count++)
        {
            if (captures[count].getPromotedRank() == Rank::QUEEN)
            {
                hasQueen = true;
            }
            else if (captures[count].getPromotedRank() == Rank::ROOK)
            {
                hasRook = true;
            }
            else if (captures[count].getPromotedRank() == Rank::BISHOP)
            {
                hasBishop = true;
            }
            else if (captures[count].getPromotedRank() == Rank::KNIGHT)
            {
                hasKnight = true;
            }

            EXPECT_EQ(captures[count].getTargetSquare(), s);
            EXPECT_EQ(captures[count].getMovingPiece(), mp);
        }

        EXPECT_EQ(true, hasQueen);
        EXPECT_EQ(true, hasRook);
        EXPECT_EQ(true, hasBishop);
        EXPECT_EQ(true, hasKnight);
    }
}

TEST_F(PositionTest,
    generateMoves_PawnCaptures_IfExistEnPassant_ShouldGenerateCaptures)
{
    Position p("8/8/8/8/2pP4/2R5/8/8 b - d3 0 1");

    MoveList captures;
    p.generateMoves(Rank::PAWN, captures);

    EXPECT_EQ(1, captures.size());

    if (captures.size() > 0)
    {
        EXPECT_EQ(0x0000000000080000ULL,
            captures[0].getTargetSquare().toBitboard());
        EXPECT_EQ(true, Piece(Color::BLACK, Rank::PAWN, Square("c4")) ==
            captures[0].getMovingPiece());
        EXPECT_EQ(Rank::NONE, captures[0].getPromotedRank());
    }
}

TEST_F(PositionTest,
    generateMoves_PawnAdvances_IfNotBlocked_ShouldGenerateAdvance)
{
    Position p("8/8/8/8/2p5/8/8/8 b - - 0 1");

    MoveList advances;
    p.generateMoves(Rank::PAWN, advances);

    EXPECT_EQ(1, advances.size());

    if (advances.size() > 0)
    {
        EXPECT_EQ(0x0000000000040000ULL,
            advances[0].getTargetSquare().toBitboard());
        EXPECT_EQ(true, Piece(Color::BLACK, Rank::PAWN, Square("c4")) ==
            advances[0].getMovingPiece());
        EXPECT_EQ(Rank::NONE, advances[0].getPromotedRank());
    }
}

TEST_F(PositionTest,
    generateMoves_PawnAdvances_IfInCheck_ShouldNotGetUnhelpfulMoves)
{
    Position p("8/8/8/2B4p/8/4k3/8/8 b - - 0 1");

    MoveList advances;
    p.generateMoves(Rank::PAWN, advances);

    EXPECT_EQ(0, advances.size());
}

TEST_F(PositionTest,
    generateMoves_PawnAdvances_IfInCheck_ShouldGetHelpfulMoves)
{
    Position p("8/8/8/2Bp3p/8/4k3/8/8 b - - 0 1");

    MoveList advances;
    p.generateMoves(Rank::PAWN, advances);

    EXPECT_EQ(1, advances.size());

    if (advances.size() > 0)
    {
        EXPECT_EQ(0x8000000ULL, advances[0].getTargetSquare()
            .toBitboard());
    }
}

TEST_F(PositionTest,
    generateMoves_PawnAdvances_IfPromotion_ShouldGeneratePromotions)
{
    Position p("8/3P4/8/8/8/8/8/8 w - - 0 1");

    MoveList captures;
    p.generateMoves(Rank::PAWN, captures);

    Piece mp(Color::WHITE, Rank::PAWN, Square("d7"));
    Square s("d8");
    bool hasQueen = false;
    bool hasRook = false;
    bool hasBishop = false;
    bool hasKnight = false;

    EXPECT_EQ(4, captures.size());

    if (captures.size() > 0)
    {
        for (size_t count = 0; count < captures.size(); count++)
        {
            if (captures[count].getPromotedRank() == Rank::QUEEN)
            {
                hasQueen = true;
            }
            else if (captures[count].getPromotedRank() == Rank::ROOK)
            {
                hasRook = true;
            }
            else if (captures[count].getPromotedRank() == Rank::BISHOP)
            {
                hasBishop = true;
            }
            else if (captures[count].getPromotedRank() == Rank::KNIGHT)
            {
                hasKnight = true;
            }

            EXPECT_EQ(captures[count].getTargetSquare(), s);
            EXPECT_EQ(captures[count].getMovingPiece(), mp);
        }

        EXPECT_EQ(true, hasQueen);
        EXPECT_EQ(true, hasRook);
        EXPECT_EQ(true, hasBishop);
        EXPECT_EQ(true, hasKnight);
    }
}

TEST_F(PositionTest,
    generateMoves_PawnAdvances_IfPinned_ShouldNotGenerateAdvance)
{
    Position p("8/8/8/8/k1p2R2/8/8/8 b - - 0 1");

    MoveList advances;
    p.generateMoves(Rank::PAWN, advances);

    EXPECT_EQ(0, advances.size());
}

TEST_F(PositionTest,
    generateMoves_PawnAdvances_IfPinnedDiag_ShouldNotGenerateAdvance)
{
    Position p("8/8/8/8/k7/1p6/8/3B4 b - - 0 1");

    MoveList advances;
    p.generateMoves(Rank::PAWN, advances);

    EXPECT_EQ(0, advances.size());
}

TEST_F(PositionTest,
    generateMoves_PawnDoubleAdv_ShouldAdvanceTwoIfFirstMove)
{
    Position p("8/4p3/8/8/8/8/8/8 b - - 0 1");

    MoveList advances;
    p.generateMoves(Rank::PAWN, advances);

    EXPECT_EQ(2, advances.size());

    Piece mp(Color::BLACK, Rank::PAWN, Square("e7"));
    bool normalAdvanceExists = false;
    bool doubleAdvanceExists = false;

    for (size_t count = 0; count < advances.size(); count++)
    {
        if (advances[count].getTargetSquare() == Square("e6"))
        {
            normalAdvanceExists = true;
        }

        if (advances[count].getTargetSquare() == Square("e5"))
        {
            doubleAdvanceExists = true;
        }
    }

    EXPECT_EQ(true, normalAdvanceExists);
    EXPECT_EQ(true, doubleAdvanceExists);
}

TEST_F(PositionTest,
    generateMoves_PawnDblAdvances_IfBlocked_ShouldNotGenerateAdvance)
{
    Position p("8/8/8/8/1B6/8/1P6/8 w - - 0 1");

    MoveList advances;
    p.generateMoves(Rank::PAWN, advances);

    EXPECT_EQ(1, advances.size());
}

TEST_F(PositionTest,
    generateMoves_KnightMoves_ShouldCalculateMoves)
{
    Position p("1N6/8/8/8/1B6/3N4/1r3Q2/8 w - - 0 1");

    MoveList advances;
    p.generateMoves(Rank::KNIGHT, advances);

    EXPECT_EQ(9, advances.size());
}

TEST_F(PositionTest,
    generateMoves_KnightMoves_IfInCheck_ShouldOnlyFindCapture)
{
    Position p("1N6/8/8/8/1B6/3N4/1r3K2/8 w - - 0 1");

    MoveList advances;
    p.generateMoves(Rank::KNIGHT, advances);

    EXPECT_EQ(1, advances.size());
}

TEST_F(PositionTest,
    generateMoves_makeMove_ShouldRestorePositionCorrectly)
{
    Position p {"8/8/8/8/1B6/3N4/1r3K2/8 w - - 0 1"};

    Move m {Piece {Color::WHITE, Rank::KNIGHT, Square {"d3"}},
        Square {"e1"}, Rank::NONE};

    std::stringstream ss;

    ss << "- - - - - - - -" << std::endl;
    ss << "- - - - - - - -" << std::endl;
    ss << "- - - - - - - -" << std::endl;
    ss << "- - - - - - - -" << std::endl;
    ss << "- B - - - - - -" << std::endl;
    ss << "- - - N - - - -" << std::endl;
    ss << "- r - - - K - -" << std::endl;
    ss << "- - - - - - - -" << std::endl;
    ss << "To Move: White" << std::endl;

    p.makeMove(m);
    p.unmakeMove();

    EXPECT_EQ(ss.str(), p.toString());
}
