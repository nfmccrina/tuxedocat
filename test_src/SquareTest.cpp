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
#include "../include/InvalidSquareException.hpp"

TEST_F(SquareTest, WhenUsingStringCtor_ShouldThrowExceptionIfInvalid)
{
    EXPECT_THROW (TuxedoCat::Square("b9"),
        TuxedoCat::InvalidSquareException);
}

TEST_F(SquareTest, WhenUsingStringCtor_ShouldThrowExceptionIfEmpty)
{
    EXPECT_THROW (TuxedoCat::Square(""),
        TuxedoCat::InvalidSquareException);
}

TEST_F(SquareTest, WhenUsingPairCtor_ShouldThrowExceptionIfInvalid)
{
    EXPECT_THROW (TuxedoCat::Square(std::pair<int, int>(-1, 5)),
        TuxedoCat::InvalidSquareException);
}

TEST_F(SquareTest, WhenUsingBitboardCtor_ShouldThrowExceptionIfInvalid)
{
    EXPECT_THROW(TuxedoCat::Square(TuxedoCat::Bitboard(0x00ULL)),
        TuxedoCat::InvalidSquareException);
}

TEST_F(SquareTest, toBitboard_ShouldConvertToBitboardCorrectly)
{
    TuxedoCat::Bitboard b {0x0000000020000000ULL};
    TuxedoCat::Square s {b};

    EXPECT_EQ(b, s.toBitboard());
}

TEST_F(SquareTest, toCoordinates_ShouldConvertToCoordinatesCorrectly)
{
    TuxedoCat::Bitboard b {0x0000000020000000ULL};
    TuxedoCat::Square s {b};

    std::pair<int, int> result = s.toCoordinates();

    EXPECT_EQ(3, result.first);
    EXPECT_EQ(5, result.second);
}

TEST_F(SquareTest, toString_ShouldConvertToAlgebraicNotationCorrectly)
{
    TuxedoCat::Bitboard b {0x0000000020000000ULL};
    TuxedoCat::Square s {b};

    EXPECT_EQ("f4", s.toString());
}
