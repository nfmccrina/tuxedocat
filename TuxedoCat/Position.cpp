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
#include <vector>

using namespace TuxedoCat;
using namespace TuxedoCat::Utility;

void TuxedoCat::Position::SetPosition(Board& position, std::string fen)
{
	std::vector<std::string> fen_parts = split(fen, " ");

	std::vector<std::string> rankInfo = split(fen_parts[0], "/");
	uint64_t currentSquare = 0x0000000000000000ULL;

	position.WhitePawns = 0x0000000000000000ULL;
	position.WhiteKnights = 0x0000000000000000ULL;
	position.WhiteBishops = 0x0000000000000000ULL;
	position.WhiteRooks = 0x0000000000000000ULL;
	position.WhiteQueens = 0x0000000000000000ULL;
	position.WhiteKing = 0x0000000000000000ULL;
	position.WhitePieces = 0x0000000000000000ULL;
	position.BlackPawns = 0x0000000000000000ULL;
	position.BlackKnights = 0x0000000000000000ULL;
	position.BlackBishops = 0x0000000000000000ULL;
	position.BlackRooks = 0x0000000000000000ULL;
	position.BlackQueens = 0x0000000000000000ULL;
	position.BlackKing = 0x0000000000000000ULL;
	position.BlackPieces = 0x0000000000000000ULL;

	for (int i = 0; i < 8; i++)
	{
		currentSquare = 0x0000000000000001ULL << (8 * (7 - i));

		for (size_t j = 0; j < rankInfo[i].length(); j++)
		{
			char currentCharacter = rankInfo[i][j];
			if (currentCharacter > 47 && currentCharacter < 58)
			{
				currentSquare = currentSquare << (currentCharacter - 48);
			}
			else
			{
				AddPieceAt(position, currentSquare,
					GetPieceColorFromChar(currentCharacter), GetPieceRankFromChar(currentCharacter));
					
				currentSquare = currentSquare << 1;
			}
		}
	}

	if (fen_parts[1] == "b")
	{
		position.ColorToMove = PieceColor::BLACK;
	}
	else if (fen_parts[1] == "w")
	{
		position.ColorToMove = PieceColor::WHITE;
	}

	position.CastlingStatus = CastlingFlags::NONE;

	if (fen_parts[2].find("K") != std::string::npos)
	{
		position.CastlingStatus = position.CastlingStatus | CastlingFlags::WHITE_SHORT;
	}

	if (fen_parts[2].find("Q") != std::string::npos)
	{
		position.CastlingStatus = position.CastlingStatus | CastlingFlags::WHITE_LONG;
	}

	if (fen_parts[2].find("k") != std::string::npos)
	{
		position.CastlingStatus = position.CastlingStatus | CastlingFlags::BLACK_SHORT;
	}

	if (fen_parts[2].find("q") != std::string::npos)
	{
		position.CastlingStatus = position.CastlingStatus | CastlingFlags::BLACK_LONG;
	}

	if (fen_parts[3] == "-")
	{
		position.EnPassantTarget = 0x0000000000000000ULL;
	}
	else
	{
		position.EnPassantTarget = GetSquareFromAlgebraic(fen);
	}

	position.HalfMoveCounter = std::stoi(fen_parts[4]);

	position.FullMoveCounter = std::stoi(fen_parts[5]);
}

void TuxedoCat::Position::Make(Board& position, Move mv)
{
	if (mv.SourceLocation == 0x0000000000000000ULL)
	{
		if (position.ColorToMove == PieceColor::WHITE)
		{
			position.ColorToMove = PieceColor::BLACK;
		}
		else
		{
			position.ColorToMove = PieceColor::WHITE;
			position.FullMoveCounter++;
		}
	}
	else
	{
		position.EnPassantTarget = 0x00000000000000ULL;
		//position.HalfMoveCounter = mv.CurrentHalfMoves + 1;

		if (((mv.SourceLocation & position.WhiteKing) != 0x00000000000000ULL
			|| (mv.SourceLocation & position.BlackKing) != 0x00000000000000ULL)
			&& ((mv.SourceLocation == 0x0000000000000010ULL && mv.TargetLocation == 0x0000000000000004ULL)
				|| (mv.SourceLocation == 0x0000000000000010ULL && mv.TargetLocation == 0x0000000000000040ULL)
				|| (mv.SourceLocation == 0x1000000000000000ULL && mv.TargetLocation == 0x0400000000000000ULL)
				|| (mv.SourceLocation == 0x1000000000000000ULL && mv.TargetLocation == 0x4000000000000000ULL)))
		{
			// castle

			position.HalfMoveCounter = 0;

			if (mv.SourceLocation == 0x0000000000000010ULL && mv.TargetLocation == 0x0000000000000004ULL)
			{
				// white queen-side
				AddPieceAt(position, 0x0000000000000004ULL, PieceColor::WHITE, PieceRank::KING);
				AddPieceAt(position, 0x0000000000000008ULL, PieceColor::WHITE, PieceRank::ROOK);
				RemovePieceAt(position, 0x0000000000000010ULL);
				RemovePieceAt(position, 0x0000000000000001ULL);

				position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::WHITE_SHORT);
				position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::WHITE_LONG);
			}
			else if (mv.SourceLocation == 0x0000000000000010ULL && mv.TargetLocation == 0x0000000000000040ULL)
			{
				// white king-side
				AddPieceAt(position, 0x0000000000000040ULL, PieceColor::WHITE, PieceRank::KING);
				AddPieceAt(position, 0x0000000000000020ULL, PieceColor::WHITE, PieceRank::ROOK);
				RemovePieceAt(position, 0x0000000000000010ULL);
				RemovePieceAt(position, 0x0000000000000080ULL);

				position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::WHITE_SHORT);
				position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::WHITE_LONG);
			}
			else if (mv.SourceLocation == 0x1000000000000000ULL && mv.TargetLocation == 0x0400000000000000ULL)
			{
				// black queen-side
				AddPieceAt(position, 0x0400000000000000ULL, PieceColor::BLACK, PieceRank::KING);
				AddPieceAt(position, 0x0800000000000000ULL, PieceColor::BLACK, PieceRank::ROOK);
				RemovePieceAt(position, 0x1000000000000000ULL);
				RemovePieceAt(position, 0x0100000000000000ULL);

				position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::BLACK_SHORT);
				position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::BLACK_LONG);
			}
			else if (mv.SourceLocation == 0x1000000000000000ULL && mv.TargetLocation == 0x4000000000000000ULL)
			{
				// black king-side
				AddPieceAt(position, 0x4000000000000000ULL, PieceColor::BLACK, PieceRank::KING);
				AddPieceAt(position, 0x2000000000000000ULL, PieceColor::BLACK, PieceRank::ROOK);
				RemovePieceAt(position, 0x1000000000000000ULL);
				RemovePieceAt(position, 0x8000000000000000ULL);

				position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::BLACK_SHORT);
				position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::BLACK_LONG);
			}
		}
		else
		{
			// not a castle

			if (mv.MovingPiece == PieceRank::PAWN)
			{
				// pawn move

				position.HalfMoveCounter = 0;

				if (mv.CapturedPiece != PieceRank::NONE)
				{
					if (mv.TargetLocation == mv.CurrentEnPassant)
					{
						if (mv.MoveColor == PieceColor::WHITE)
						{
							RemovePieceAt(position, mv.TargetLocation >> 8);
						}
						else
						{
							RemovePieceAt(position, mv.TargetLocation << 8);
						}
					}
					else
					{
						if (mv.CapturedPiece == PieceRank::ROOK)
						{
							if (mv.TargetLocation == 0x0000000000000001ULL)
							{
								position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::WHITE_LONG);
							}
							else if (mv.TargetLocation == 0x0000000000000080ULL)
							{
								position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::WHITE_SHORT);
							}
							else if (mv.TargetLocation == 0x0100000000000000ULL)
							{
								position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::BLACK_LONG);
							}
							else if (mv.TargetLocation == 0x8000000000000000ULL)
							{
								position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::BLACK_SHORT);
							}
						}

						RemovePieceAt(position, mv.TargetLocation);
					}
				}
				else
				{
					if (mv.TargetLocation == (mv.SourceLocation << 16))
					{
						position.EnPassantTarget = mv.SourceLocation << 8;
					}
					else if (mv.TargetLocation == (mv.SourceLocation >> 16))
					{
						position.EnPassantTarget = mv.SourceLocation >> 8;
					}
				}

				if (mv.PromotedRank != PieceRank::NONE)
				{
					AddPieceAt(position, mv.TargetLocation, mv.MoveColor, mv.PromotedRank);
				}
				else
				{
					AddPieceAt(position, mv.TargetLocation, mv.MoveColor, PieceRank::PAWN);
				}

				RemovePieceAt(position, mv.SourceLocation);
			}
			else
			{
				if (mv.MovingPiece == PieceRank::ROOK && mv.MoveColor == PieceColor::WHITE
					&& mv.SourceLocation == 0x0000000000000001ULL)
				{
					position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::WHITE_LONG);
				}
				else if (mv.MovingPiece == PieceRank::ROOK && mv.MoveColor == PieceColor::WHITE
					&& mv.SourceLocation == 0x0000000000000080ULL)
				{
					position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::WHITE_SHORT);
				}
				else if (mv.MovingPiece == PieceRank::ROOK && mv.MoveColor == PieceColor::BLACK
					&& mv.SourceLocation == 0x0100000000000000ULL)
				{
					position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::BLACK_LONG);
				}
				else if (mv.MovingPiece == PieceRank::ROOK && mv.MoveColor == PieceColor::BLACK
					&& mv.SourceLocation == 0x8000000000000000ULL)
				{
					position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::BLACK_SHORT);
				}

				if (mv.MovingPiece == PieceRank::KING && mv.MoveColor == PieceColor::WHITE)
				{
					position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::WHITE_LONG);
					position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::WHITE_SHORT);
				}
				else if (mv.MovingPiece == PieceRank::KING && mv.MoveColor == PieceColor::BLACK)
				{
					position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::BLACK_LONG);
					position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::BLACK_SHORT);
				}

				if (mv.CapturedPiece != PieceRank::NONE)
				{
					if (mv.CapturedPiece == PieceRank::ROOK)
					{
						if (mv.TargetLocation == 0x0000000000000001ULL)
						{
							position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::WHITE_LONG);
						}
						else if (mv.TargetLocation == 0x0000000000000080ULL)
						{
							position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::WHITE_SHORT);
						}
						else if (mv.TargetLocation == 0x0100000000000000ULL)
						{
							position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::BLACK_LONG);
						}
						else if (mv.TargetLocation == 0x8000000000000000ULL)
						{
							position.CastlingStatus = position.CastlingStatus & (~CastlingFlags::BLACK_SHORT);
						}
					}
					// a capture!
					RemovePieceAt(position, mv.TargetLocation); // he gone
					position.HalfMoveCounter = 0;
				}

				AddPieceAt(position, mv.TargetLocation, mv.MoveColor, mv.MovingPiece);
				RemovePieceAt(position, mv.SourceLocation);
			}
		}

		if (position.ColorToMove == PieceColor::WHITE)
		{
			position.ColorToMove = PieceColor::BLACK;
		}
		else
		{
			position.ColorToMove = PieceColor::WHITE;
			position.FullMoveCounter++;
		}
	}
}

void TuxedoCat::Position::Unmake(Board& position, Move mv)
{
	if (mv.SourceLocation == 0x0000000000000000ULL)
	{
		if (position.ColorToMove == PieceColor::WHITE)
		{
			position.ColorToMove = PieceColor::BLACK;
			position.FullMoveCounter--;
		}
		else
		{
			position.ColorToMove = PieceColor::WHITE;
		}
	}
	else
	{
		AddPieceAt(position, mv.SourceLocation, mv.MoveColor, mv.MovingPiece);
		RemovePieceAt(position, mv.TargetLocation);

		if (((mv.SourceLocation & position.WhiteKing) != 0x0000000000000000ULL
			|| (mv.SourceLocation & position.BlackKing) != 0x0000000000000000ULL)
			&& ((mv.SourceLocation == 0x0000000000000010ULL && mv.TargetLocation == 0x0000000000000004ULL)
				|| (mv.SourceLocation == 0x0000000000000010ULL && mv.TargetLocation == 0x0000000000000040ULL)
				|| (mv.SourceLocation == 0x1000000000000000ULL && mv.TargetLocation == 0x0400000000000000ULL)
				|| (mv.SourceLocation == 0x1000000000000000ULL && mv.TargetLocation == 0x4000000000000000ULL)))
		{
			uint64_t oldRookLocation = 0x0000000000000000ULL;
			uint64_t newRookLocation = 0x0000000000000000ULL;

			if (mv.SourceLocation == 0x0000000000000010ULL && mv.TargetLocation == 0x0000000000000040ULL)
			{
				oldRookLocation = 0x0000000000000020ULL;
				newRookLocation = 0x0000000000000080ULL;
			}
			else if (mv.SourceLocation == 0x0000000000000010ULL && mv.TargetLocation == 0x0000000000000004ULL)
			{
				oldRookLocation = 0x0000000000000008ULL;
				newRookLocation = 0x0000000000000001ULL;
			}
			else if (mv.SourceLocation == 0x1000000000000000ULL && mv.TargetLocation == 0x4000000000000000ULL)
			{
				oldRookLocation = 0x2000000000000000ULL;
				newRookLocation = 0x8000000000000000ULL;
			}
			else if (mv.SourceLocation == 0x1000000000000000ULL && mv.TargetLocation == 0x0400000000000000ULL)
			{
				oldRookLocation = 0x0800000000000000ULL;
				newRookLocation = 0x0100000000000000ULL;
			}

			RemovePieceAt(position, mv.TargetLocation);
			RemovePieceAt(position, oldRookLocation);
			AddPieceAt(position, mv.SourceLocation, mv.MoveColor, PieceRank::KING);
			AddPieceAt(position, newRookLocation, mv.MoveColor, PieceRank::ROOK);
		}

		if (mv.CapturedPiece != PieceRank::NONE)
		{
			if (mv.CurrentEnPassant != 0x0000000000000000ULL && mv.MovingPiece == PieceRank::PAWN
				&& mv.TargetLocation == mv.CurrentEnPassant)
			{
				if (mv.MoveColor == PieceColor::WHITE)
				{
					AddPieceAt(position, mv.TargetLocation >> 8, PieceColor::BLACK, mv.CapturedPiece);
				}
				else
				{
					AddPieceAt(position, mv.TargetLocation << 8, PieceColor::WHITE, mv.CapturedPiece);
				}
			}
			else
			{
				AddPieceAt(position, mv.TargetLocation,
					mv.MoveColor == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE,
					mv.CapturedPiece);
			}
		}

		position.ColorToMove = mv.MoveColor;
		position.CastlingStatus = mv.CastlingStatus;
		position.EnPassantTarget = mv.CurrentEnPassant;
		position.HalfMoveCounter = mv.CurrentHalfMoves;

		if (mv.MoveColor == PieceColor::BLACK)
		{
			position.FullMoveCounter--;
		}
	}
}

void TuxedoCat::Position::AddPieceAt(Board& position, uint64_t loc, PieceColor color, PieceRank rank)
{
	if (rank == PieceRank::QUEEN)
	{
		if (color == PieceColor::WHITE)
		{
			position.WhiteQueens = position.WhiteQueens | loc;
		}
		else
		{
			position.BlackQueens = position.BlackQueens | loc;
		}
	}
	else if (rank == PieceRank::ROOK)
	{
		if (color == PieceColor::WHITE)
		{
			position.WhiteRooks = position.WhiteRooks | loc;
		}
		else
		{
			position.BlackRooks = position.BlackRooks | loc;
		}
	}
	else if (rank == PieceRank::BISHOP)
	{
		if (color == PieceColor::WHITE)
		{
			position.WhiteBishops = position.WhiteBishops | loc;
		}
		else
		{
			position.BlackBishops = position.BlackBishops | loc;
		}
	}
	else if (rank == PieceRank::KNIGHT)
	{
		if (color == PieceColor::WHITE)
		{
			position.WhiteKnights = position.WhiteKnights | loc;
		}
		else
		{
			position.BlackKnights = position.BlackKnights | loc;
		}
	}
	else if (rank == PieceRank::PAWN)
	{
		if (color == PieceColor::WHITE)
		{
			position.WhitePawns = position.WhitePawns | loc;
		}
		else
		{
			position.BlackPawns = position.BlackPawns | loc;
		}
	}
	else if (rank == PieceRank::KING)
	{
		if (color == PieceColor::WHITE)
		{
			position.WhiteKing = loc;
		}
		else
		{
			position.BlackKing = loc;
		}
	}

	UpdatePieces(position);
}

void TuxedoCat::Position::RemovePieceAt(Board& position, uint64_t location)
{
	position.WhitePawns = position.WhitePawns & (~location);
	position.WhiteKnights = position.WhiteKnights & (~location);
	position.WhiteBishops = position.WhiteBishops & (~location);
	position.WhiteRooks = position.WhiteRooks & (~location);
	position.WhiteQueens = position.WhiteQueens & (~location);
	position.WhiteKing = position.WhiteKing & (~location);

	position.BlackPawns = position.BlackPawns & (~location);
	position.BlackKnights = position.BlackKnights & (~location);
	position.BlackBishops = position.BlackBishops & (~location);
	position.BlackRooks = position.BlackRooks & (~location);
	position.BlackQueens = position.BlackQueens & (~location);
	position.BlackKing = position.BlackKing & (~location);

	UpdatePieces(position);
}

PieceRank TuxedoCat::Position::GetRankAt(Board& position, uint64_t loc)
{
	PieceRank rank = PieceRank::NONE;

	if ((position.WhitePawns & loc) != 0x0000000000000000ULL
		|| (position.BlackPawns & loc) != 0x0000000000000000ULL)
	{
		rank = PieceRank::PAWN;
	}

	if ((position.WhiteKnights & loc) != 0x0000000000000000ULL
		|| (position.BlackKnights & loc) != 0x0000000000000000ULL)
	{
		rank = PieceRank::KNIGHT;
	}

	if ((position.WhiteBishops & loc) != 0x0000000000000000ULL
		|| (position.BlackBishops & loc) != 0x0000000000000000ULL)
	{
		rank = PieceRank::BISHOP;
	}

	if ((position.WhiteRooks & loc) != 0x0000000000000000ULL
		|| (position.BlackRooks & loc) != 0x0000000000000000ULL)
	{
		rank = PieceRank::ROOK;
	}

	if ((position.WhiteQueens & loc) != 0x0000000000000000ULL
		|| (position.BlackQueens & loc) != 0x0000000000000000ULL)
	{
		rank = PieceRank::QUEEN;
	}

	if ((position.WhiteKing & loc) != 0x0000000000000000ULL
		|| (position.BlackKing & loc) != 0x0000000000000000ULL)
	{
		rank = PieceRank::KING;
	}

	return rank;
}

PieceColor TuxedoCat::Position::GetColorAt(Board& position, uint64_t loc)
{
	PieceColor color = PieceColor::NONE;

	if ((position.WhitePieces & loc) != 0x0000000000000000UL)
	{
		color = PieceColor::WHITE;
	}

	if ((position.BlackPieces & loc) != 0x0000000000000000UL)
	{
		color = PieceColor::BLACK;
	}

	return color;
}

void TuxedoCat::Position::UpdatePieces(Board& position)
{
	position.WhitePieces = (position.WhitePawns
		| position.WhiteKnights
		| position.WhiteBishops
		| position.WhiteRooks
		| position.WhiteQueens
		| position.WhiteKing);

	position.BlackPieces = (position.BlackPawns
		| position.BlackKnights
		| position.BlackBishops
		| position.BlackRooks
		| position.BlackQueens
		| position.BlackKing);
}
