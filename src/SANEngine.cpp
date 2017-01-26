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

#include "SANEngine.hpp"
#include "Rank.hpp"
#include "LookupData.hpp"
#include "Utility.hpp"
#include <sstream>

using namespace TuxedoCat;

std::string SANEngine::calculateNotation(Position& pos, const Move& move)
{
    std::stringstream san;
    Rank movingRank = move.movingPiece.rank;
    MoveList availableMoves;

    pos.generateMoves(movingRank, availableMoves);

    if (movingRank != Rank::PAWN)
    {
        std::string rank {rankToString(movingRank)};
        rank[0] = std::toupper(rank[0]);

        if (!move.isCastle())
        {
            san << rank;

            bool ambiguity = false;
            bool ranksDiffer = false;
            bool filesDiffer = false;

            for (int i = 0; i < availableMoves.size(); i++)
            {
                Move tmpMove {availableMoves[i]};

                if (tmpMove.movingPiece.square != move.movingPiece.square &&
                    tmpMove.targetSquare == move.targetSquare)
                {
                    ambiguity = true;
                    
                    int squareIndex {Utility::lsb(move.movingPiece.square)};
                    uint64_t rankMask {LookupData::rankMask[squareIndex]};
                    uint64_t fileMask {LookupData::fileMask[squareIndex]};

                    if ((tmpMove.movingPiece.square & rankMask) == 0x00ULL)
                    {
                        ranksDiffer = true;
                    }

                    if ((tmpMove.movingPiece.square & fileMask) == 0x00ULL)
                    {
                        filesDiffer = true;
                    }

                    break;
                }
            }

            if (ambiguity)
            {
                if (filesDiffer && !ranksDiffer)
                {
                    san << Utility::toAlgebraicCoordinate(
                        move.movingPiece.square)[0];
                }
                else if (ranksDiffer && !filesDiffer)
                {
                    san << Utility::toAlgebraicCoordinate(
                        move.movingPiece.square)[1];
                }
                else
                {
                    san << Utility::toAlgebraicCoordinate(
                        move.movingPiece.square);
                }
            }

            if (pos.isSquareOccupied(Utility::lsb(move.targetSquare)) !=
                0x00ULL)
            {
                san << "x";
            }

            san << Utility::toAlgebraicCoordinate(
                move.targetSquare);
        }
        else
        {
            if (Utility::inMask(move.targetSquare,
                0x4000000000000040ULL))
            {
                san << "0-0";
            }
            else
            {
                san << "0-0-0";
            }
        }
    }
    else
    {
        bool ep = false;

        if (pos.isSquareOccupied(Utility::lsb(move.targetSquare)) !=
            0x00ULL)
        {
            san << Utility::toAlgebraicCoordinate(
                move.movingPiece.square)[0];
            san << "x";

            if (move.targetSquare == (0x01ULL << pos.getEnPassantIndex()))
            {
                ep = true;
            }
        }

        san << Utility::toAlgebraicCoordinate(
            move.targetSquare);

        if (ep)
        {
            san << "e.p.";
        }

        if (move.promotedRank != Rank::NONE)
        {
            san << "=";

            std::string pr {rankToString(move.promotedRank)};
            san << static_cast<char>(std::toupper(pr[0]));
        }
    }

    pos.makeMove(move);

    if (pos.isInCheck())
    {
        san << "+";
    }

    pos.unmakeMove();

    return san.str();
}
