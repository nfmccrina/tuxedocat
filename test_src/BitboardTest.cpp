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
#include "../include/gtest/gtest.h"
#include <sstream>
#include <utility>

using namespace TuxedoCat;

void BitboardTest::SetUp()
{
    bitboard.setValue(0x00ULL);
}

TEST_F(BitboardTest,
    getValue_WhenBitboardInitializedNotEmpty_ReturnsTheCorrectValue)
{
    Bitboard b(0x02ULL);

    EXPECT_EQ (0x02ULL, b.getValue());
}

TEST_F(BitboardTest,
    getValue_WhenBitboardInitializedEmpty_ReturnsTheCorrectValue)
{
    Bitboard b;

    EXPECT_EQ (0x00ULL, b.getValue());
}

TEST_F(BitboardTest, setValue_UpdatesTheBitboardCorrectly)
{
    Bitboard b;
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
    Bitboard b1(0x8F729262B65A6DC3ULL);
    Bitboard b2(0x8F729262B65A6DC3ULL);

    EXPECT_EQ(true, b1 == b2);
}

TEST_F(BitboardTest, NotEqualsOp_ComparesBitboardsCorrectly)
{
    Bitboard b1(0x8F729262B65A6DC3ULL);
    Bitboard b2(0x8F729162B65A6DC3ULL);

    EXPECT_EQ(true, b1 != b2);
}

TEST_F(BitboardTest, AmpersandOp_ReturnsTheIntersectionOfTwoBitboards)
{
    Bitboard b1(0x010040F00F010263ULL);
    Bitboard b2(0x0080420010410280ULL);

    EXPECT_EQ(0x0000400000010200ULL, (b1 & b2).getValue());
}

TEST_F(BitboardTest,
    AmpersandEqualsOp_SetsTheBitboardToTheIntersectionOfTwoBitboards)
{
    Bitboard b1(0x010040F00F010263ULL);
    Bitboard b2(0x0080420010410280ULL);

    EXPECT_EQ(0x0000400000010200ULL, (b1 &= b2).getValue());
}

TEST_F(BitboardTest, PipeOp_ReturnsTheUnionOfTwoBitboards)
{
    Bitboard b1(0x010040F00F010263ULL);
    Bitboard b2(0x0080420010410280ULL);

    EXPECT_EQ(0x018042F01F4102E3ULL, (b1 | b2).getValue());
}

TEST_F(BitboardTest, PipeEqualsOp_SetsTheBitboardToTheUnionOfTwoBitboards)
{
    Bitboard b1(0x010040F00F010263ULL);
    Bitboard b2(0x0080420010410280ULL);

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

TEST_F(BitboardTest,
    toAlgebraicCoordinate_ReturnsAnAlgebraicCoordinateIfOneBitSet)
{
    bitboard.setValue(0x0000080000000000ULL);

    EXPECT_EQ("d6", bitboard.toAlgebraicCoordinate());
}

TEST_F(BitboardTest,
    toAlgebraicCoordinate_ReturnsEmptyStringIfMultipleBitsSet)
{
    bitboard.setValue(0x0000080000000F00ULL);

    EXPECT_EQ("", bitboard.toAlgebraicCoordinate());
}

TEST_F(BitboardTest, toAlgebraicCoordinate_ReturnsEmptyStringIfEmpty)
{
    bitboard.setValue(0x0000000000000000ULL);

    EXPECT_EQ("", bitboard.toAlgebraicCoordinate());
}

TEST_F(BitboardTest,
    toCoordinates_ReturnsACoordinatePairIfOneBitSet)
{
    bitboard.setValue(0x0000080000000000ULL);
    
    std::pair<int, int> result { bitboard.toCoordinates() };

    EXPECT_EQ(5, result.first);
    EXPECT_EQ(3, result.second);
}

TEST_F(BitboardTest,
    toCoordinates_ReturnsZeroIfMultipleBitsSet)
{
    bitboard.setValue(0x0000080000000F00ULL);

    std::pair<int, int> result = bitboard.toCoordinates();

    EXPECT_EQ(0, result.first);
    EXPECT_EQ(0, result.second);
}

TEST_F(BitboardTest, toCoordinates_ReturnsZeroIfEmpty)
{
    bitboard.setValue(0x0000000000000000ULL);

    std::pair<int, int> result = bitboard.toCoordinates();

    EXPECT_EQ(0, result.first);
    EXPECT_EQ(0, result.second);
}

TEST_F(BitboardTest, uint64_t_IsConvertedToBitboardImplicitly)
{
    Bitboard b(0x0250492803434000ULL);
    Bitboard b2(0x0000000040000000ULL);

    EXPECT_EQ(true, b == 0x0250492803434000ULL);
    EXPECT_EQ(0x0000000044000000ULL, b2 | 0x0000000004000000ULL);
}

TEST_F(BitboardTest, uint64_t_CanBeAssignedToBitboardVariable)
{
    Bitboard b;

    b = 0x0000000F00000000ULL;

    EXPECT_EQ(true, b.getValue() == 0x0000000F00000000ULL);
}

TEST_F(BitboardTest, flipBit_WhenValidIndex_ShouldFlipTheCorrectBit)
{
    Bitboard b(0x40F2012900B3CC24ULL);

    b.flipBit(40);

    EXPECT_EQ(true, b.getValue() == 0x40F2002900B3CC24ULL);
}

TEST_F(BitboardTest, flipBit_WhenInvalidIndex_ShouldNotModifyBitboard)
{
    Bitboard b(0x40F2012900B3CC24ULL);

    b.flipBit(74);

    EXPECT_EQ(true, b.getValue() == 0x40F2012900B3CC24ULL);
}

TEST_F(BitboardTest, inMask_ReturnsTrueIfMatchesMask)
{
    Bitboard b(0x0000000000000010ULL);

    EXPECT_EQ(true, b.inMask(0x00000000000000FFULL));
}

TEST_F(BitboardTest, inMask_ReturnsFalseIfNotMatchesMask)
{
    Bitboard b(0x0000000007000010ULL);

    EXPECT_EQ(false, b.inMask(0x00000000000000FFULL));
}

TEST_F(BitboardTest, isEmpty_ReturnsTrueIfNoBitsSet)
{
    Bitboard b {0x0000000000000000ULL};

    EXPECT_EQ(true, b.isEmpty());
}

TEST_F(BitboardTest, isEmpty_ReturnsFalseIfBitsSet)
{
    Bitboard b {0x0000008000000000ULL};

    EXPECT_EQ(false, b.isEmpty());
}
