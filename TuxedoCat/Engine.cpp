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
#include <chrono>

using namespace TuxedoCat;

Board currentPosition;
TimeControl currentClock;

static uint64_t nodeCount;

void Engine::InitializeEngine()
{
	currentClock.movesPerControl = 40;
	currentClock.remainingTime = 30000;
	currentClock.timeIncrement = 0;
	currentClock.type = TimeControlType::CONVENTIONAL;

	maxSearchDepth = 10000;

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

std::string Engine::GetMove(Board& position, TimeControl& clock)
{
	Move move = SearchRoot(position, clock);
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
	uint64_t whitePassedPawns;
	uint64_t blackPassedPawns;

	if (position.ColorToMove == PieceColor::WHITE)
	{
		sideToMoveFactor = 1;
	}
	else
	{
		sideToMoveFactor = -1;
	}

	// material count
	score += (100 * (Utility::PopCount(position.WhitePawns) - Utility::PopCount(position.BlackPawns)));
	score += (300 * (Utility::PopCount(position.WhiteKnights) - Utility::PopCount(position.BlackKnights)));
	score += (300 * (Utility::PopCount(position.WhiteBishops) - Utility::PopCount(position.BlackBishops)));
	score += (500 * (Utility::PopCount(position.WhiteRooks) - Utility::PopCount(position.BlackRooks)));
	score += (900 * (Utility::PopCount(position.WhiteQueens) - Utility::PopCount(position.BlackQueens)));
	score += (10000 * (Utility::PopCount(position.WhiteKing) - Utility::PopCount(position.BlackKing)));

	// passed pawns
	whitePassedPawns = Position::GetPassedPawns(position, PieceColor::WHITE);
	blackPassedPawns = Position::GetPassedPawns(position, PieceColor::BLACK);

	score += (20 * (Utility::PopCount(whitePassedPawns) - Utility::PopCount(blackPassedPawns)));

	score += (10 * (Utility::PopCount(whitePassedPawns & 0x00FF000000000000ULL) - Utility::PopCount(blackPassedPawns & 0x000000000000FF00ULL)));

	score += (5 * (Utility::PopCount(whitePassedPawns & 0x0000FF0000000000ULL) - Utility::PopCount(blackPassedPawns & 0x0000000000FF0000ULL)));

	score += (2 * (Utility::PopCount(whitePassedPawns & 0x000000FF00000000ULL) - Utility::PopCount(blackPassedPawns & 0x00000000FF000000ULL)));

	// doubled pawns

	score -= (3 * (Position::GetDoubledPawnCount(position, PieceColor::WHITE) - Position::GetDoubledPawnCount(position, PieceColor::BLACK)));

	return (score * sideToMoveFactor);
}

uint64_t Engine::GetAvailableSearchTime(TimeControl& clock, Board& position)
{
	uint32_t movesRemainingUntilNextTimeControl;
	uint64_t availableTime;

	if (clock.type == TimeControlType::CONVENTIONAL)
	{
		if (clock.movesPerControl == 0)
		{
			movesRemainingUntilNextTimeControl = 30;
		}
		else
		{
			movesRemainingUntilNextTimeControl = clock.movesPerControl - ((position.FullMoveCounter - 1) % clock.movesPerControl);

			if (movesRemainingUntilNextTimeControl == 0)
			{
				movesRemainingUntilNextTimeControl = 1;
			}
		}

		if (clock.remainingTime <= 50)
		{
			availableTime = 0;
		}
		else
		{
			availableTime = ((clock.remainingTime - 50) * 4) / movesRemainingUntilNextTimeControl;
		}
	}
	else if (clock.type == TimeControlType::INCREMENTAL)
	{
		if (clock.remainingTime <= 50)
		{
			availableTime = 0;
		}
		else
		{
			availableTime = ((clock.remainingTime - 50) / 30) + (clock.timeIncrement - 50);
		}
	}
	else if (clock.type == TimeControlType::TIME_PER_MOVE)
	{
		availableTime = clock.remainingTime - 10;
	}

	return availableTime;
}

Move Engine::SearchRoot(Board& position, TimeControl& clock)
{
	int max = 0;
	int depth = 1;
	int currentScore = 0;
	std::vector<Move> availableMoves;
	Move bestMove;
	std::default_random_engine generator(static_cast<unsigned int>(std::time(0)));
	std::uniform_int_distribution<> dist(-10, 10);
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point currentTime;
	std::chrono::milliseconds elapsedTime;
	uint64_t availableSearchTime;
	uint64_t predictedSearchTime;
	uint64_t nodeCountAtBeginningOfIteration;
	uint64_t nodeCountOfPreviousIteration;
	uint64_t nodeCountOfCurrentIteration;
	uint64_t predictedNodesOfNextIteration;
	uint64_t effectiveBranchingFactor;
	uint64_t branchingFactorSum;
	long long msecs;
	uint64_t nodesPerMillisecond;
	std::stringstream logText;

	availableSearchTime = GetAvailableSearchTime(clock, position);
	predictedSearchTime = 0;
	nodeCount = 0;
	nodeCountOfPreviousIteration = 0;

	start = std::chrono::high_resolution_clock::now();
	
	while (predictedSearchTime < availableSearchTime)
	{
		max = -3000000;
		bestMove.TargetLocation = 0;
		nodeCountAtBeginningOfIteration = nodeCount;

		availableMoves = MoveGenerator::GenerateMoves(position);
	
		for (auto it = availableMoves.begin(); it != availableMoves.end(); it++)
		{
			Position::Make(position, *it);
			nodeCount++;

			currentScore = -Search(position, depth - 1);

			Position::Unmake(position, *it);

			if (randomMode)
			{
				currentScore += dist(generator);
			}

			if (currentScore > max)
			{
				max = currentScore;
				bestMove = *it;
			}
		}

		// calculate predicted time of next iteration

		nodeCountOfCurrentIteration = nodeCount - nodeCountAtBeginningOfIteration;

		if (nodeCountOfPreviousIteration == 0)
		{
			branchingFactorSum = nodeCountOfCurrentIteration;
		}
		else
		{
			branchingFactorSum += (nodeCountOfCurrentIteration / nodeCountOfPreviousIteration);
		}

		effectiveBranchingFactor = (branchingFactorSum / depth);

		predictedNodesOfNextIteration = (nodeCountOfCurrentIteration * effectiveBranchingFactor);
		nodeCountOfPreviousIteration = nodeCountOfCurrentIteration;

		currentTime = std::chrono::high_resolution_clock::now();
		elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - start);
		msecs = elapsedTime.count();

		if (msecs == 0)
		{
			nodesPerMillisecond = 0x7FFFFFFFFFFFFFFFULL;
		}
		else
		{
			nodesPerMillisecond = nodeCount / msecs;
		}

		predictedSearchTime = (predictedNodesOfNextIteration / (nodesPerMillisecond * 10)) + (msecs / 10);

		depth++;
	}

	return bestMove;	
}

int Engine::Search(Board& position, int depth)
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
				nodeCount++;

				currentScore = -Search(position, depth - 1);

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
