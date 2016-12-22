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

#include "Rank.hpp"
#include <iostream>
#include <cctype>

TuxedoCat::Rank TuxedoCat::getRankFromChar(char ch)
{
    Rank rank;

    switch (std::toupper(ch))
    {
        case 'P':
        {
            rank = Rank::PAWN;
        }
        break;
        case 'N':
        {
            rank = Rank::KNIGHT;
        }
        break;
        case 'B':
        {
            rank = Rank::BISHOP;
        }
        break;
        case 'R':
        {
            rank = Rank::ROOK;
        }
        break;
        case 'Q':
        {
            rank = Rank::QUEEN;
        }
        break;
        case 'K':
        {
            rank = Rank::KING;
        }
        break;
        default:
        {
            rank = Rank::PAWN;
        }
        break;
    }

    return rank;
}

std::string TuxedoCat::rankToString(Rank r)
{
    if (r == Rank::PAWN)
    {
        return "p";
    }
    else if (r == Rank::KNIGHT)
    {
        return "n";
    }
    else if (r == Rank::BISHOP)
    {
        return "b";
    }
    else if (r == Rank::ROOK)
    {
        return "r";
    }
    else if (r == Rank::QUEEN)
    {
        return "q";
    }
    else
    {
        return "k";
    }
}