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

#include "../test_include/BitboardTest.hpp"
#include "../googletest/include/gtest/gtest.h"
#include <sstream>

void BitboardTest::SetUp()
{
    bitboard.setValue(0x00ULL);
}

TEST_F(BitboardTest,
    getValue_WhenBitboardInitializedNotEmpty_ReturnsTheCorrectValue)
{
    TuxedoCat::Bitboard b(0x02ULL);

    EXPECT_EQ (0x02ULL, b.getValue());
}

TEST_F(BitboardTest,
    getValue_WhenBitboardInitializedEmpty_ReturnsTheCorrectValue)
{
    TuxedoCat::Bitboard b;

    EXPECT_EQ (0x00ULL, b.getValue());
}

TEST_F(BitboardTest, setValue_UpdatesTheBitboardCorrectly)
{
    TuxedoCat::Bitboard b;
    b.setValue(0x0503ULL);

    EXPECT_EQ (0x0503ULL, b.getValue());
}

TEST_F(BitboardTest, lsb_ReturnsNegativeOneForEmptyBitboard)
{
    EXPECT_EQ(-1, bitboard.lsb());
}

TEST_F(BitboardTest, lsb_ReturnsCorrectValueForNonEmptyBitboard)
{
    bitboard.setValue(0x0007E50300400000ULL);

    EXPECT_EQ(22, bitboard.lsb());
}

TEST_F(BitboardTest, msb_ReturnsNegativeOneForEmptyBitboard)
{
    EXPECT_EQ(-1, bitboard.msb());
}

TEST_F(BitboardTest, msb_ReturnsCorrectValueForNonEmptyBitboard)
{
    bitboard.setValue(0x0007E50300400000ULL);

    EXPECT_EQ(50, bitboard.msb());
}

TEST_F(BitboardTest, popcount_ReturnsZeroForEmptyBitboard)
{
    EXPECT_EQ(0, bitboard.popcount());
}

TEST_F(BitboardTest, popcount_ReturnsCorrectValueForNonEmptyBitboard)
{
    bitboard.setValue(0x0007E50300400000ULL);

    EXPECT_EQ(11, bitboard.popcount());
}

TEST_F(BitboardTest, toString_RepresentsTheBitboardCorrectly)
{
    bitboard.setValue(0x0007E50300400000ULL);
    std::stringstream ss;

    ss << "- - - - - - - -";
    ss << std::endl;
    ss << "* * * - - - - -";
    ss << std::endl;
    ss << "* - * - - * * *";
    ss << std::endl;
    ss << "* * - - - - - -";
    ss << std::endl;
    ss << "- - - - - - - -";
    ss << std::endl;
    ss << "- - - - - - * -";
    ss << std::endl;
    ss << "- - - - - - - -";
    ss << std::endl;
    ss << "- - - - - - - -";

    EXPECT_EQ(ss.str(), bitboard.toString());
}

TEST_F(BitboardTest, tilde_ReturnsTheComplementOfTheBitboard)
{
    bitboard.setValue(0x830920F64080D401ULL);

    EXPECT_EQ(0x7CF6DF09BF7F2BFEULL, (~bitboard).getValue());
}

TEST_F(BitboardTest, EqualsOp_ComparesBitboardsCorrectly)
{
    TuxedoCat::Bitboard b1(0x8F729262B65A6DC3ULL);
    TuxedoCat::Bitboard b2(0x8F729262B65A6DC3ULL);

    EXPECT_EQ(true, b1 == b2);
}

TEST_F(BitboardTest, NotEqualsOp_ComparesBitboardsCorrectly)
{
    TuxedoCat::Bitboard b1(0x8F729262B65A6DC3ULL);
    TuxedoCat::Bitboard b2(0x8F729162B65A6DC3ULL);

    EXPECT_EQ(true, b1 != b2);
}

TEST_F(BitboardTest, AmpersandOp_ReturnsTheIntersectionOfTwoBitboards)
{
    TuxedoCat::Bitboard b1(0x010040F00F010263ULL);
    TuxedoCat::Bitboard b2(0x0080420010410280ULL);

    EXPECT_EQ(0x0000400000010200ULL, (b1 & b2).getValue());
}

TEST_F(BitboardTest,
    AmpersandEqualsOp_SetsTheBitboardToTheIntersectionOfTwoBitboards)
{
    TuxedoCat::Bitboard b1(0x010040F00F010263ULL);
    TuxedoCat::Bitboard b2(0x0080420010410280ULL);

    EXPECT_EQ(0x0000400000010200ULL, (b1 &= b2).getValue());
}

TEST_F(BitboardTest, PipeOp_ReturnsTheUnionOfTwoBitboards)
{
    TuxedoCat::Bitboard b1(0x010040F00F010263ULL);
    TuxedoCat::Bitboard b2(0x0080420010410280ULL);

    EXPECT_EQ(0x018042F01F4102E3ULL, (b1 | b2).getValue());
}

TEST_F(BitboardTest, PipeEqualsOp_SetsTheBitboardToTheUnionOfTwoBitboards)
{
    TuxedoCat::Bitboard b1(0x010040F00F010263ULL);
    TuxedoCat::Bitboard b2(0x0080420010410280ULL);

    EXPECT_EQ(0x018042F01F4102E3ULL, (b1 |= b2).getValue());
}

TEST_F(BitboardTest, GreaterThanOp_ReturnsTrueIfFirstOperandIsGreater)
{
    EXPECT_EQ(true, greaterThanValue > lessThanValue);
}

TEST_F(BitboardTest, GreaterThanOp_ReturnsFalseIfOperandsAreEqual)
{
    EXPECT_EQ(false, firstEqualValue > secondEqualValue);
}

TEST_F(BitboardTest, GreaterThanOp_ReturnsFalseIfFirstOperandIsLess)
{
    EXPECT_EQ(false, lessThanValue > greaterThanValue);
}

TEST_F(BitboardTest, LessThanOp_ReturnsTrueIfFirstOperandIsLess)
{
    EXPECT_EQ(true, lessThanValue < greaterThanValue);
}

TEST_F(BitboardTest, LessThanOp_ReturnsFalseIfOperandsAreEqual)
{
    EXPECT_EQ(false, firstEqualValue < secondEqualValue);
}

TEST_F(BitboardTest, LessThanOp_ReturnsFalseIfFirstOperandIsGreater)
{
    EXPECT_EQ(false, greaterThanValue < lessThanValue);
}

TEST_F(BitboardTest,
    GreaterThanOrEqualOp_ReturnsTrueIfFirstOperandIsGreater)
{
    EXPECT_EQ(true, greaterThanValue >= lessThanValue);
}

TEST_F(BitboardTest,
    GreaterThanOrEqualOp_ReturnsTrueIfOperandsAreEqual)
{
    EXPECT_EQ(true, firstEqualValue >= secondEqualValue);
}

TEST_F(BitboardTest,
    GreaterThanOrEqualOp_ReturnsFalseIfFirstOperandIsLess)
{
    EXPECT_EQ(false, lessThanValue >= greaterThanValue);
}

TEST_F(BitboardTest,
    LessThanOrEqualOp_ReturnsTrueIfFirstOperandIsLess)
{
    EXPECT_EQ(true, lessThanValue <= greaterThanValue);
}

TEST_F(BitboardTest,
    LessThanOrEqualOp_ReturnsTrueIfOperandsAreEqual)
{
    EXPECT_EQ(true, firstEqualValue <= secondEqualValue);
}

TEST_F(BitboardTest,
    LessThanOrEqualOp_ReturnsFalseIfFirstOperandIsGreater)
{
    EXPECT_EQ(false, greaterThanValue <= lessThanValue);
}

TEST_F(BitboardTest, ShiftLOp_ReturnsABitboardShiftedCorrectly)
{
    bitboard.setValue(0x0000000000000001ULL);

    EXPECT_EQ(0x0000000000010000ULL, (bitboard << 16).getValue());
}

TEST_F(BitboardTest, ShiftLOp_ReturnsUnchangedBitboardIfNegativeShift)
{
    bitboard.setValue(0x0000000000000001ULL);

    EXPECT_EQ(0x0000000000000001ULL, (bitboard << -4).getValue());
}

TEST_F(BitboardTest, ShiftROp_ReturnsABitboardShiftedCorrectly)
{
    bitboard.setValue(0x0000800000000000ULL);

    EXPECT_EQ(0x0000001000000000ULL, (bitboard >> 11).getValue());
}

TEST_F(BitboardTest, ShiftROp_ReturnsUnchangedBitboardIfNegativeShift)
{
    bitboard.setValue(0x0000000000000001ULL);

    EXPECT_EQ(0x0000000000000001ULL, (bitboard << -4).getValue());
}

TEST_F(BitboardTest, ShiftREqOp_SetsBitboardToShiftResult)
{
    bitboard.setValue(0x0000800000000000ULL);

    EXPECT_EQ(0x0000001000000000ULL, (bitboard >>= 11).getValue());
}

TEST_F(BitboardTest, ShiftREqOp_DoesNotModifyBitboardIfNegativeShift)
{
    bitboard.setValue(0x0000000000000001ULL);

    EXPECT_EQ(0x0000000000000001ULL, (bitboard <<= -4).getValue());
}

TEST_F(BitboardTest, ShiftLEqOp_SetsBitboardToShiftResult)
{
    bitboard.setValue(0x0000000000000001ULL);

    EXPECT_EQ(0x0000000000010000ULL, (bitboard <<= 16).getValue());
}

TEST_F(BitboardTest, ShiftLEqOp_DoesNotModifyBitboardIfNegativeShift)
{
    bitboard.setValue(0x0000000000000001ULL);

    EXPECT_EQ(0x0000000000000001ULL, (bitboard <<= -4).getValue());
}