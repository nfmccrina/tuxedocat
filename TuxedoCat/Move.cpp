/*
* The MIT License (MIT)
*
* Copyright (c) 2016 Nathan McCrina
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#include "TuxedoCat.h"

using namespace TuxedoCat;

void MoveUtil::InitializeMove(Move& move, uint64_t tgt, uint64_t src, uint64_t cep, PieceColor mc, PieceRank mp,
	PieceRank cp, PieceRank pr, int chm, int cs)
{
	move.TargetLocation = tgt;
	move.SourceLocation = src;
	move.CurrentEnPassant = cep;
	move.MoveColor = mc;
	move.MovingPiece = mp;
	move.CapturedPiece = cp;
	move.PromotedRank = pr;
	move.CurrentHalfMoves = chm;
	move.CastlingStatus = cs;
}

bool MoveUtil::compareMoves(Move& m1, Move& m2)
{
	bool result = false;

	if (m1.CapturedPiece != PieceRank::NONE)
	{
		if (m2.CapturedPiece == PieceRank::NONE)
		{
			result = true;
		}
		else
		{
			if (Utility::ComparePieces(m2.CapturedPiece, m1.CapturedPiece))
			{
				result = true;
			}
			else if (!Utility::ComparePieces(m1.CapturedPiece, m2.CapturedPiece))
			{
				if (Utility::ComparePieces(m1.MovingPiece, m2.MovingPiece))
				{
					result = true;
				}
			}
		}
	}

	return result;
}