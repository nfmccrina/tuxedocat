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

#include "../include/MoveList.hpp"
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
        moves[++currentIndex] = m;
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
                if (m.getMovingPiece().getColor() ==
                    moves[count].getMovingPiece().getColor())
                {
                    total++;
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_RANK) != 0)
            {
                if (m.getMovingPiece().getRank() ==
                    moves[count].getMovingPiece().getRank())
                {
                    total++;
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_SQUARE) != 0)
            {
                if (m.getMovingPiece().getSquare() ==
                    moves[count].getMovingPiece().getSquare())
                {
                    total++;
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_SQUARE_RANK)
                != 0)
            {
                if (m.getMovingPiece().getSquare().toString()[1] ==
                    moves[count].getMovingPiece().getSquare().toString()[1])
                {
                    total++;
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_SQUARE_FILE)
                != 0)
            {
                if (m.getMovingPiece().getSquare().toString()[0] ==
                    moves[count].getMovingPiece().getSquare().toString()[0])
                {
                    total++;
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::TARGET_SQUARE) != 0)
            {
                if (m.getTargetSquare() ==
                    moves[count].getTargetSquare())
                {
                    total++;
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::PROMOTED_RANK) != 0)
            {
                if (m.getPromotedRank() ==
                    moves[count].getPromotedRank())
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
                if (m.getMovingPiece().getColor() !=
                    moves[count].getMovingPiece().getColor())
                {
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_RANK) != 0)
            {
                if (m.getMovingPiece().getRank() !=
                    moves[count].getMovingPiece().getRank())
                {
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_SQUARE) != 0)
            {
                if (m.getMovingPiece().getSquare() !=
                    moves[count].getMovingPiece().getSquare())
                {
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_SQUARE_RANK)
                != 0)
            {
                if (m.getMovingPiece().getSquare().toString()[1] !=
                    moves[count].getMovingPiece().getSquare().toString()[1])
                {
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::MOVING_PIECE_SQUARE_FILE)
                != 0)
            {
                if (m.getMovingPiece().getSquare().toString()[0] !=
                    moves[count].getMovingPiece().getSquare().toString()[0])
                {
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::TARGET_SQUARE) != 0)
            {
                if (m.getTargetSquare() !=
                    moves[count].getTargetSquare())
                {
                    continue;
                }
            }

            if ((criteria & MoveSearchCriteria::PROMOTED_RANK) != 0)
            {
                if (m.getPromotedRank() !=
                    moves[count].getPromotedRank())
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
