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

#include "MoveList.hpp"
#include "Utility.hpp"
#include "MoveSearchCriteria.hpp"

using namespace TuxedoCat;

MoveList::MoveList()
    : currentIndex(-1)
{
}

void MoveList::addMove(Move m)
{
    if (currentIndex < (MoveList::MAX_SIZE - 1))
    {
        moves.push_back(m);
        currentIndex++;
    }
}

int MoveList::contains(Move m, int criteria, bool matchAny) const
{
    int total = 0;

    for (int count = 0; count <= currentIndex; count++)
    {
        if (matchAny)
        {
            if ((criteria & MoveSearchCriteria::MOVING_PIECE_COLOR) != 0)
            {
                if (m.movingPiece.color ==
                    moves[count].movingPiece.color)
                {
                    total++;
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_RANK) != 0)
            {
                if (m.movingPiece.rank ==
                    moves[count].movingPiece.rank)
                {
                    total++;
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_SQUARE) != 0)
            {
                if (m.movingPiece.square ==
                    moves[count].movingPiece.square)
                {
                    total++;
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_SQUARE_RANK)
                != 0)
            {
                if (Utility::toString(m.movingPiece.square)[1] ==
                    Utility::toString(moves[count].movingPiece.square)[1])
                {
                    total++;
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_SQUARE_FILE)
                != 0)
            {
                if (Utility::toAlgebraicCoordinate(
                        m.movingPiece.square)[0] ==
                    Utility::toAlgebraicCoordinate(
                        moves[count].movingPiece.square)[0])
                {
                    total++;
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::TARGET_SQUARE) != 0)
            {
                if (m.targetSquare ==
                    moves[count].targetSquare)
                {
                    total++;
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::PROMOTED_RANK) != 0)
            {
                if (m.promotedRank ==
                    moves[count].promotedRank)
                {
                    total++;
                    continue;
                }
            }
        }
        else
        {
            if ((criteria & MoveSearchCriteria::MOVING_PIECE_COLOR) != 0)
            {
                if (m.movingPiece.color !=
                    moves[count].movingPiece.color)
                {
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_RANK) != 0)
            {
                if (m.movingPiece.rank !=
                    moves[count].movingPiece.rank)
                {
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_SQUARE) != 0)
            {
                if (m.movingPiece.square !=
                    moves[count].movingPiece.square)
                {
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_SQUARE_RANK)
                != 0)
            {
                if (Utility::toAlgebraicCoordinate(
                        m.movingPiece.square)[1] !=
                    Utility::toAlgebraicCoordinate(
                        moves[count].movingPiece.square)[1])
                {
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_SQUARE_FILE)
                != 0)
            {
                if (Utility::toAlgebraicCoordinate(
                    m.movingPiece.square)[0] !=
                    Utility::toAlgebraicCoordinate(
                        moves[count].movingPiece.square)[0])
                {
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::TARGET_SQUARE) != 0)
            {
                if (m.targetSquare !=
                    moves[count].targetSquare)
                {
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::PROMOTED_RANK) != 0)
            {
                if (m.promotedRank !=
                    moves[count].promotedRank)
                {
                    continue;
                }
            }

            total++;
        }
    }

    return total;
}

int MoveList::size()
{
    return currentIndex + 1;
}

bool MoveList::isEmpty()
{
    return currentIndex == -1;
}

Move& MoveList::operator[](int pos)
{
    return moves[pos];
}
