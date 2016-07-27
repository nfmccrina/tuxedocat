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

 /*
  * LSB, MSB and PopCount code is from the chess programming wiki (chessprogramming.wikispaces.com)
  */

#include "TuxedoCat.h"
#include <sstream>
#include <cctype>

using namespace TuxedoCat;

static const uint64_t k1 = 0x5555555555555555ULL;
static const uint64_t k2 = 0x3333333333333333ULL;
static const uint64_t k4 = 0x0f0f0f0f0f0f0f0fULL;
static const uint64_t kf = 0x0101010101010101ULL;

static int index64[64] = {
	0,  1, 48,  2, 57, 49, 28,  3,
	61, 58, 50, 42, 38, 29, 17,  4,
	62, 55, 59, 36, 53, 51, 43, 22,
	45, 39, 33, 30, 24, 18, 12,  5,
	63, 47, 56, 27, 60, 41, 37, 16,
	54, 35, 52, 21, 44, 32, 23, 11,
	46, 26, 40, 15, 34, 20, 31, 10,
	25, 14, 19,  9, 13,  8,  7,  6
};

static int index64Reverse[64] = {
	0, 47,  1, 56, 48, 27,  2, 60,
	57, 49, 41, 37, 28, 16,  3, 61,
	54, 58, 35, 52, 50, 42, 21, 44,
	38, 32, 29, 23, 17, 11,  4, 62,
	46, 55, 26, 59, 40, 36, 15, 53,
	34, 51, 20, 43, 31, 22, 10, 45,
	25, 39, 14, 33, 19, 30,  9, 24,
	13, 18,  8, 12,  7,  6,  5, 63
};

std::vector<std::string> Utility::split(std::string str, std::string delimiter)
{
	size_t tmpIndex;
	std::string tmpStr;
	std::vector<std::string> result;

	tmpStr = str.substr();

	do
	{
		tmpIndex = tmpStr.find(delimiter);

		result.push_back(tmpStr.substr(0, tmpIndex));

		if (tmpIndex != std::string::npos)
		{
			tmpIndex++;
			tmpStr = tmpStr.substr(tmpIndex);
		}
		else
		{
			tmpStr.clear();
		}

	} while (!tmpStr.empty());

	return result;
}

uint64_t Utility::GetSquareFromAlgebraic(std::string algebraic)
{
	int rank = 0;
	int file = static_cast<int>(algebraic[0]);

	file -= 97;

	if (std::isdigit(algebraic[1]))
	{
		rank = std::stoi(algebraic.substr(1)) - 1;
	}

	return 0x0000000000000001ULL << ((rank * 8) + file);
}

int Utility::GetLSB(uint64_t bitmask)
{
	/*
	* begin wiki code
	*/

	if (bitmask == 0x0000000000000000ULL)
	{
		return -1;
	}

	uint64_t debruijn64 = 0x03F79D71B4CB0A89ULL;
	return index64[((bitmask & ((~bitmask) + 1)) * debruijn64) >> 58];

	/*
	* end wiki code
	*/
}

int Utility::GetMSB(uint64_t bitmask)
{
	/*
	* begin wiki code
	*/

	if (bitmask == 0x0000000000000000ULL)
	{
		return -1;
	}

	uint64_t debruijn64 = 0x03F79D71B4CB0A89ULL;
	bitmask |= bitmask >> 1;
	bitmask |= bitmask >> 2;
	bitmask |= bitmask >> 4;
	bitmask |= bitmask >> 8;
	bitmask |= bitmask >> 16;
	bitmask |= bitmask >> 32;
	return index64Reverse[(bitmask * debruijn64) >> 58];

	/*
	* end wiki code
	*/
}

int Utility::PopCount(uint64_t bitmask)
{
	/*
	* begin wiki code
	*/

	uint64_t x = bitmask;

	x = x - ((x >> 1) & k1);
	x = (x & k2) + ((x >> 2) & k2);
	x = (x + (x >> 4)) & k4;
	x = (x * kf) >> 56;

	return (int)x;

	/*
	* end wiki code
	*/
}

std::string Utility::GenerateSAN(Board& position, Move move, std::vector<Move> allMoves)
{
	std::stringstream san;
	bool isCastle = false;

	if (move.MovingPiece != PieceRank::PAWN)
	{
		if (move.MovingPiece == PieceRank::KNIGHT)
		{
			san << "N";
		}
		else if (move.MovingPiece == PieceRank::BISHOP)
		{
			san << "B";
		}
		else if (move.MovingPiece == PieceRank::ROOK)
		{
			san << "R";
		}
		else if (move.MovingPiece == PieceRank::QUEEN)
		{
			san << "Q";
		}
		else if (move.MovingPiece == PieceRank::KING)
		{
			if ((move.SourceLocation == 0x0000000000000010UL && move.TargetLocation == 0x0000000000000004UL)
				|| (move.SourceLocation == 0x0000000000000010UL && move.TargetLocation == 0x0000000000000040UL)
				|| (move.SourceLocation == 0x1000000000000000UL && move.TargetLocation == 0x0400000000000000UL)
				|| (move.SourceLocation == 0x1000000000000000UL && move.TargetLocation == 0x4000000000000000UL))
			{
				isCastle = true;
			}
			else
			{
				san << "K";
			}
		}

		if (!isCastle)
		{
			uint64_t pieces = 0x0000000000000000UL;

			if (move.MovingPiece == PieceRank::KNIGHT)
			{
				pieces =
					move.MoveColor == PieceColor::WHITE ? position.WhiteKnights : position.BlackKnights;
			}
			else if (move.MovingPiece == PieceRank::BISHOP)
			{
				pieces =
					move.MoveColor == PieceColor::WHITE ? position.WhiteBishops : position.BlackBishops;
			}
			else if (move.MovingPiece == PieceRank::ROOK)
			{
				pieces =
					move.MoveColor == PieceColor::WHITE ? position.WhiteRooks : position.BlackRooks;
			}
			else if (move.MovingPiece == PieceRank::QUEEN)
			{
				pieces =
					move.MoveColor == PieceColor::WHITE ? position.WhiteQueens : position.BlackQueens;
			}


			if (PopCount(pieces) > 1)
			{
				std::vector<Move> possibleMoves;
				
				for (auto it = allMoves.cbegin(); it != allMoves.cend(); it++)
				{
					if (it->MovingPiece == move.MovingPiece
						&& it->MoveColor == move.MoveColor
						&& it->TargetLocation == move.TargetLocation
						&& it->SourceLocation != move.SourceLocation)
					{
						possibleMoves.push_back(*it);
					}
				}

				if (possibleMoves.size() != 0)
				{
					bool conflictFile = false;
					bool conflictRank = false;

					for (auto it = possibleMoves.cbegin(); it != possibleMoves.cend(); it++)
					{
						if (GetFileFromLocation(it->SourceLocation) == GetFileFromLocation(move.SourceLocation))
						{
							conflictFile = true;
						}

						if (GetRankFromLocation(it->SourceLocation) == GetRankFromLocation(move.SourceLocation))
						{
							conflictRank = true;
						}
					}

					if (!conflictFile)
					{
						san << GetFileFromLocation(move.SourceLocation);
					}
					else if (!conflictRank)
					{
						san << GetRankFromLocation(move.SourceLocation);
					}
					else
					{
						san << GetFileFromLocation(move.SourceLocation);
						san << GetRankFromLocation(move.SourceLocation);
					}
				}
			}

			if (move.CapturedPiece != PieceRank::NONE)
			{
				san << "x";
			}

			san << GetFileFromLocation(move.TargetLocation);
			san << GetRankFromLocation(move.TargetLocation);
		}
		else
		{
			if (move.TargetLocation == 0x0000000000000040UL || move.TargetLocation == 0x4000000000000000UL)
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
		if (move.CapturedPiece != PieceRank::NONE)
		{
			san << GetFileFromLocation(move.SourceLocation);
			san << "x";
		}

		san << GetFileFromLocation(move.TargetLocation);
		san << GetRankFromLocation(move.TargetLocation);

		if (move.TargetLocation == position.EnPassantTarget
			&& move.CapturedPiece != PieceRank::NONE)
		{
			san << "e.p";
		}

		if (move.PromotedRank != PieceRank::NONE)
		{
			san << "=";

			if (move.PromotedRank == PieceRank::KNIGHT)
			{
				san << "N";
			}
			else if (move.PromotedRank == PieceRank::BISHOP)
			{
				san << "B";
			}
			else if (move.PromotedRank == PieceRank::ROOK)
			{
				san << "R";
			}
			else if (move.PromotedRank == PieceRank::QUEEN)
			{
				san << "Q";
			}
		}
	}

	return san.str();
}

std::string Utility::GetFileFromLocation(uint64_t location)
{
	uint64_t mask = 0x0101010101010101UL;
	char file = ' ';

	for (int index = 0; index < 8; index++)
	{
		if ((mask & location) != 0x0000000000000000UL)
		{
			file = (char)(index + 97);
			break;
		}

		mask = mask << 1;
	}

	return std::to_string(file);
}

int Utility::GetRankFromLocation(uint64_t location)
{
	uint64_t mask = 0x00000000000000FFUL;
	int index = 0;

	for (index = 1; index < 9; index++)
	{
		if ((mask & location) != 0x0000000000000000UL)
		{
			break;
		}

		mask = mask << 8;
	}

	return index;
}