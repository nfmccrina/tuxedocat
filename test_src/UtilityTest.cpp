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

#include "Utility.hpp"
#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <utility>
#include <sstream>

using namespace TuxedoCat;

TEST(UtilityTest, msb_ShouldCorrectlyCalculateMSB)
{
    EXPECT_EQ(48, Utility::msb(0x0001000000010101ULL));
}

TEST(UtilityTest, msb_ShouldReturnNegativeOneIfInputZero)
{
    EXPECT_EQ(-1, Utility::msb(0x00ULL));
}

TEST(UtilityTest, split_ShouldSplitStringCorrectly)
{
    std::vector<std::string> parts {Utility::split("ab cd ef", ' ')};

    EXPECT_EQ(3, static_cast<int>(parts.size()));
    EXPECT_EQ("ab", parts[0]);
    EXPECT_EQ("cd", parts[1]);
    EXPECT_EQ("ef", parts[2]);
}

TEST(UtilityTest, split_ShouldSplitStringWithOnePartCorrectly)
{
    std::vector<std::string> parts {Utility::split("abcdef", ' ')};

    EXPECT_EQ(1, static_cast<int>(parts.size()));
    EXPECT_EQ("abcdef", parts[0]);
}

TEST(UtilityTest, split_ShouldSplitEmptyStringCorrectly)
{
    std::vector<std::string> parts {Utility::split("", ' ')};

    EXPECT_EQ(0, static_cast<int>(parts.size()));
}

TEST(UtilityTest, flipBit_shouldToggleSpecifiedBit)
{
    uint64_t b {0x0000000800000000ULL};

    Utility::flipBit(b, 35);

    EXPECT_EQ(b, 0x00ULL);
}

TEST(UtilityTest, flipBit_shouldLeaveArgUnchangedIfIndexOutOfRange)
{
    uint64_t b {0x0000000800000000ULL};

    Utility::flipBit(b, -7);

    EXPECT_EQ(b, 0x0000000800000000ULL);
}

TEST(UtilityTest, inMask_shouldReturnTrueIfValueIsCompletelyCoveredByMask)
{
    bool result {Utility::inMask(0x0000040000002000ULL, 0x0000FF000000FF00ULL)};

    EXPECT_EQ(true, result);
}

TEST(UtilityTest, inMask_shouldReturnFalseIfValueIsNotCompletelyCoveredByMask)
{
    bool result {Utility::inMask(0x0000040000002000ULL, 0x0000FF0000000000ULL)};

    EXPECT_EQ(false, result);
}

TEST(UtilityTest, lsb_shouldReturnLeastSetBitIndex)
{
    int result = Utility::lsb(0x0007003000000000ULL);

    EXPECT_EQ(36, result);
}

TEST(UtilityTest, lsb_shouldReturnNegativeOneIfNoBitsSet)
{
    int result = Utility::lsb(0x0000000000000000ULL);

    EXPECT_EQ(-1, result);
}

TEST(UtilityTest, msb_shouldReturnHighestSetBitIndex)
{
    int result = Utility::msb(0x0007003000000000ULL);

    EXPECT_EQ(50, result);
}

TEST(UtilityTest, msb_shouldReturnNegativeOneIfNoBitsSet)
{
    int result = Utility::msb(0x0000000000000000ULL);

    EXPECT_EQ(-1, result);
}

TEST(UtilityTest, popcount_shouldReturnCountOfSetBits)
{
    int result = Utility::popcount(0x0007003000000000ULL);

    EXPECT_EQ(5, result);
}

TEST(UtilityTest, popcount_shouldReturnZeroIfNoBitsSet)
{
    int result = Utility::popcount(0x0000000000000000ULL);

    EXPECT_EQ(0, result);
}

TEST(UtilityTest, toAlgebraicCoordinate_shouldReturnStringVersionOfSquare)
{
    std::string a1 = Utility::toAlgebraicCoordinate(0x0000000000000001ULL);
    std::string e4 = Utility::toAlgebraicCoordinate(0x0000000010000000ULL);
    std::string h8 = Utility::toAlgebraicCoordinate(0x8000000000000000ULL);

    EXPECT_EQ("a1", a1);
    EXPECT_EQ("e4", e4);
    EXPECT_EQ("h8", h8);
}

TEST(UtilityTest, toAlgebraicCoordinate_shouldReturnEmptyStringIfZero)
{
    std::string result = Utility::toAlgebraicCoordinate(0x0000000000000000ULL);
    
    EXPECT_EQ("", result);
}

TEST(UtilityTest, toCoordinates_shouldReturnStringVersionOfSquare)
{
    std::pair<int, int> a1 = Utility::toCoordinates(0x0000000000000001ULL);
    std::pair<int, int> e4 = Utility::toCoordinates(0x0000000010000000ULL);
    std::pair<int, int> h8 = Utility::toCoordinates(0x8000000000000000ULL);

    EXPECT_EQ(1, a1.first);
    EXPECT_EQ(4, e4.first);
    EXPECT_EQ(8, h8.first);
    EXPECT_EQ(1, a1.second);
    EXPECT_EQ(5, e4.second);
    EXPECT_EQ(8, h8.second);
}

TEST(UtilityTest, toCoordinates_shouldReturnNegativeOneIfZero)
{
    std::pair<int, int> result = Utility::toCoordinates(0x0000000000000000ULL);
    
    EXPECT_EQ(-1, result.first);
    EXPECT_EQ(-1, result.second);
}

TEST(UtilityTest, toString_shouldPrintTheBitboard)
{
    std::stringstream ss;

    ss << "- - - * - - - -" << std::endl;
    ss << "- - - - - - - -" << std::endl;
    ss << "- - - - - - * *" << std::endl;
    ss << "- - - - - - - -" << std::endl;
    ss << "- * * - - - - -" << std::endl;
    ss << "- - - - * - - -" << std::endl;
    ss << "- - - - - - - -" << std::endl;
    ss << "- - * - - - - -";

    EXPECT_EQ(ss.str(), Utility::toString(0x0800C00006100004ULL));
}

TEST(UtilityTest, algebraicToBitboard_shouldGenerateABitboardCorrectly)
{
    uint64_t a1 = Utility::algebraicToBitboard("a1");
    uint64_t e4 = Utility::algebraicToBitboard("e4");
    uint64_t h8 = Utility::algebraicToBitboard("h8");
    
    EXPECT_EQ(0x0000000000000001ULL, a1);
    EXPECT_EQ(0x0000000010000000ULL, e4);
    EXPECT_EQ(0x8000000000000000ULL, h8);
}

TEST(UtilityTest, algebraicToBitboard_shouldGenerateAnEmptyBitboardIfInvalid)
{
    uint64_t result = Utility::algebraicToBitboard("j5");
    
    EXPECT_EQ(0x0000000000000000ULL, result);
}

TEST(UtilityTest, coordinatesToBitboard_shouldGenerateABitboardCorrectly)
{
    uint64_t a1 = Utility::coordinatesToBitboard({0, 0});
    uint64_t e4 = Utility::coordinatesToBitboard({3, 4});
    uint64_t h8 = Utility::coordinatesToBitboard({7, 7});
    
    EXPECT_EQ(0x0000000000000001ULL, a1);
    EXPECT_EQ(0x0000000010000000ULL, e4);
    EXPECT_EQ(0x8000000000000000ULL, h8);
}

TEST(UtilityTest, coordinatesToBitboard_shouldGenerateAnEmptyBitboardIfInvalid)
{
    uint64_t result = Utility::coordinatesToBitboard({4, 9});
    
    EXPECT_EQ(0x0000000000000000ULL, result);
}
