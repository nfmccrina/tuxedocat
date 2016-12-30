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

#include "../test_include/SquareTest.hpp"
#include "../include/Square.hpp"
#include "../include/gtest/gtest.h"

using namespace TuxedoCat;

TEST_F(SquareTest, Constructor_ShouldInitializeFromStringCorrectly)
{
    Square s("b1");

    EXPECT_EQ(Bitboard(0x0000000000000002ULL), s.toBitboard());
}

TEST_F(SquareTest, WhenUsingStringCtor_ShouldBeZeroIfInvalid)
{
    Square s {"b9"};

    EXPECT_EQ(false, s.isValid());
}

TEST_F(SquareTest, WhenUsingStringCtor_ShouldBeZeroIfEmpty)
{
    Square s {""};

    EXPECT_EQ(false, s.isValid());
}

TEST_F(SquareTest, WhenUsingPairCtor_ShouldBeZeroIfInvalid)
{
    Square s {std::pair<int, int>(-1, 5)};

    EXPECT_EQ(false, s.isValid());
}

TEST_F(SquareTest, WhenUsingBitboardCtor_ShouldThrowExceptionIfInvalid)
{
    Square s {Bitboard {0x00ULL}};

    EXPECT_EQ(false, s.isValid());
}

TEST_F(SquareTest, toBitboard_ShouldConvertToBitboardCorrectly)
{
    Bitboard b {0x0000000020000000ULL};
    Square s {b};

    EXPECT_EQ(b, s.toBitboard());
}

TEST_F(SquareTest, toCoordinates_ShouldConvertToCoordinatesCorrectly)
{
    Bitboard b {0x0000000020000000ULL};
    Square s {b};

    std::pair<int, int> result = s.toCoordinates();

    EXPECT_EQ(3, result.first);
    EXPECT_EQ(5, result.second);
}

TEST_F(SquareTest, toString_ShouldConvertToAlgebraicNotationCorrectly)
{
    Bitboard b {0x0000000020000000ULL};
    Square s {b};

    EXPECT_EQ("f4", s.toString());
}

TEST_F(SquareTest, operatorEQ_ShouldCorrectlyCompareObjects)
{
    Square s1("a3");
    Square s2("a3");

    EXPECT_EQ(true, s1 == s2);
}

TEST_F(SquareTest, operatorNEQ_ShouldCorrectlyCompareObjects)
{
    Square s1("a3");
    Square s2("b7");

    EXPECT_EQ(true, s1 != s2);
}

TEST_F(SquareTest, isValid_ShouldReturnFalseIfZero)
{
    Square s;

    EXPECT_EQ(false, s.isValid());
}
