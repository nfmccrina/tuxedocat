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
#include <iostream>
#include <random>
#include <functional>
#include <ctime>
#include <sstream>
#include <stack>

using namespace TuxedoCat;

Board currentPosition;
uint32_t engineTime;
uint32_t opponentTime;

void Engine::InitializeEngine()
{
	Position::SetPosition(currentPosition, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

std::string Engine::GetRandomMove(Board& position)
{
	std::vector<Move> availableMoves = MoveGenerator::GenerateMoves(position);
	std::string result;

	if (availableMoves.size() > 0)
	{
		std::default_random_engine generator(static_cast<unsigned int>(std::time(0)));
		std::uniform_int_distribution<> dist(0, static_cast<int>(availableMoves.size() - 1));

		Move move = availableMoves[dist(generator)];

		Position::Make(position, move);
		result = Utility::GenerateXBoardNotation(move);
	}
	else
	{
		result = "";
	}

	return result;
}

std::string Engine::GetMove(Board& position)
{
	Move move = NegaMaxRoot(position);
	std::string result = "";

	if (move.TargetLocation != 0)
	{
		Position::Make(position, move);
		result = Utility::GenerateXBoardNotation(move);
	}
	
	return result;
}

int Engine::EvaluatePosition(Board& position)
{
	int score = 0;
	int sideToMoveFactor = 0;

	if (position.ColorToMove == PieceColor::WHITE)
	{
		sideToMoveFactor = 1;
	}
	else
	{
		sideToMoveFactor = -1;
	}

	// material count
	score += (100 * Utility::PopCount(position.WhitePawns));
	score += (300 * Utility::PopCount(position.WhiteKnights));
	score += (300 * Utility::PopCount(position.WhiteBishops));
	score += (500 * Utility::PopCount(position.WhiteRooks));
	score += (900 * Utility::PopCount(position.WhiteQueens));
	score += (100000 * Utility::PopCount(position.WhiteKing));

	score -= (100 * Utility::PopCount(position.BlackPawns));
	score -= (300 * Utility::PopCount(position.BlackKnights));
	score -= (300 * Utility::PopCount(position.BlackBishops));
	score -= (500 * Utility::PopCount(position.BlackRooks));
	score -= (900 * Utility::PopCount(position.BlackQueens));
	score -= (100000 * Utility::PopCount(position.BlackKing));

	return (score * sideToMoveFactor);
}

Move Engine::NegaMaxRoot(Board& position)
{
	int max = 0;
	int currentScore = 0;
	std::vector<Move> availableMoves;
	Move bestMove;

	for (int depth = 1; depth < 5; depth++) // max depth of 4 for now
	{
		max = -3000000;
		bestMove.TargetLocation = 0;

		availableMoves = MoveGenerator::GenerateMoves(position);

		for (auto it = availableMoves.begin(); it != availableMoves.end(); it++)
		{
			Position::Make(position, *it);

			currentScore = -NegaMax(position, depth - 1);

			Position::Unmake(position, *it);

			if (currentScore > max)
			{
				max = currentScore;
				bestMove = *it;
			}
		}
	}

	return bestMove;	
}

int Engine::NegaMax(Board& position, int depth)
{
	int max = 0;
	int currentScore = 0;
	std::vector<Move> availableMoves;

	if (depth == 0)
	{
		return EvaluatePosition(position);
	}
	else
	{
		max = -1999999;

		availableMoves = MoveGenerator::GenerateMoves(position);

		if (availableMoves.size() > 0)
		{
			for (auto it = availableMoves.begin(); it != availableMoves.end(); it++)
			{
				Position::Make(position, *it);

				currentScore = -NegaMax(position, depth - 1);

				Position::Unmake(position, *it);

				if (currentScore > max)
				{
					max = currentScore;
				}
			}
		}
		else
		{			
			if (position.ColorToMove == PieceColor::WHITE && !MoveGenerator::IsSquareAttacked(position.WhiteKing, position))
			{
				max = 0;
			}
			else if (position.ColorToMove == PieceColor::BLACK && !MoveGenerator::IsSquareAttacked(position.BlackKing, position))
			{
				max = 0;
			}
			else
			{
				max = max - depth;
			}
		}

		return max;
	}
}

bool Engine::IsGameOver(Board& position)
{
	if (MoveGenerator::GenerateMoves(position).size() == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::string Engine::GetGameResult(Board& position)
{
	std::string result = "";

	if (IsGameOver(position))
	{
		if (position.ColorToMove == PieceColor::BLACK && MoveGenerator::IsSquareAttacked(currentPosition.BlackKing, currentPosition))
		{
			result = "1-0 {White mates}";
		}
		else if (position.ColorToMove == PieceColor::WHITE && MoveGenerator::IsSquareAttacked(currentPosition.WhiteKing, currentPosition))
		{
			result = "0-1 {Black mates}";
		}
		else
		{
			result = "1/2-1/2 {Stalemate}";
		}
	}

	return result;
}

uint64_t Engine::Perft(Board& position, int depth)
{
	if (depth <= 1)
	{
		return static_cast<uint64_t>(MoveGenerator::GenerateMoves(position).size());
	}
	else
	{
		uint64_t count = 0;
		std::vector<Move> availableMoves = MoveGenerator::GenerateMoves(position);

		for (auto it = availableMoves.cbegin(); it != availableMoves.cend(); it++)
		{
			Position::Make(position, *it);
			count += Perft(position, depth - 1);
			Position::Unmake(position, *it);
		}

		return count;
	}
}

void Engine::Divide(Board& position, int depth)
{
	uint64_t totalCount = 0;
	int moveCount = 0;
	std::vector<Move> availableMoves = MoveGenerator::GenerateMoves(position);
	std::stringstream output;

	if (depth <= 1)
	{
		for (auto it = availableMoves.cbegin(); it != availableMoves.cend(); it++)
		{
			moveCount++;
			totalCount++;

			output << Utility::GenerateSAN(position, *it, availableMoves) << ": 1";
			std::cout << output.str() << std::endl;

			Utility::WriteLog("engine -> interface: " + output.str());
		}
	}
	else
	{
		for (auto it = availableMoves.cbegin(); it != availableMoves.cend(); it++)
		{
			moveCount++;

			Position::Make(position, *it);

			uint64_t count = Perft(position, depth - 1);

			totalCount += count;

			std::cout << Utility::GenerateSAN(position, *it, availableMoves) << ": " << std::to_string(count) << std::endl;

			Position::Unmake(position, *it);
		}
	}

	std::cout << std::endl << "Moves: " << moveCount << std::endl << "Total leaf nodes: " << totalCount << std::endl;
}
