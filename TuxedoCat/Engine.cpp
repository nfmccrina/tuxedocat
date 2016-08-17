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
#include <algorithm>

using namespace TuxedoCat;

Board currentPosition;
TimeControl currentClock;

static uint64_t nodeCount;
static std::vector<std::vector<Move>> pvArrays;
static int currentMaxDepth;
static int currentBestScore;

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

	score += (8 * (Utility::PopCount(whitePassedPawns) - Utility::PopCount(blackPassedPawns)));

	score += (32 * (Utility::PopCount(whitePassedPawns & 0x00FF000000000000ULL) - Utility::PopCount(blackPassedPawns & 0x000000000000FF00ULL)));

	score += (24 * (Utility::PopCount(whitePassedPawns & 0x0000FF0000000000ULL) - Utility::PopCount(blackPassedPawns & 0x0000000000FF0000ULL)));

	score += (16 * (Utility::PopCount(whitePassedPawns & 0x000000FF00000000ULL) - Utility::PopCount(blackPassedPawns & 0x00000000FF000000ULL)));

	// doubled pawns

	score -= (5 * (Position::GetDoubledPawnCount(position, PieceColor::WHITE) - Position::GetDoubledPawnCount(position, PieceColor::BLACK)));

	// knight positioning
	score += (5 * (Utility::PopCount(position.WhiteKnights & 0x00003C3C3C3C0000ULL) - Utility::PopCount(position.BlackKnights & 0x00003C3C3C3C0000ULL)));

	// bishop mobility

	score += (static_cast<int>(MoveGenerator::GenerateMoves(position, PieceRank::BISHOP).size()));

	return (score * sideToMoveFactor);
}

uint64_t Engine::GetAvailableSearchTime(TimeControl& clock, Board& position)
{
	uint32_t movesRemainingUntilNextTimeControl;
	uint64_t availableTime = 0;

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
			availableTime = ((clock.remainingTime - 50) * 3) / movesRemainingUntilNextTimeControl;
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
		if (clock.remainingTime > 10)
		{
			availableTime = clock.remainingTime - 10;
		}
		else
		{
			availableTime = 0;
		}
	}

	return availableTime;
}

std::string Engine::BuildPVString()
{
	std::stringstream ss;

	if (pvArrays.size() > 0)
	{
		for (auto it = pvArrays[0].begin(); it != pvArrays[0].end(); it++)
		{
			ss << " " << Utility::GenerateXBoardNotation(*it);
		}
	}

	return ss.str();
}

Move Engine::SearchRoot(Board& position, TimeControl& clock)
{
	int max = 0;
	int depth = 1;
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
	int alpha;
	int beta;

	availableSearchTime = GetAvailableSearchTime(clock, position);
	predictedSearchTime = 0;
	nodeCount = 0;
	nodeCountOfPreviousIteration = 0;

	start = std::chrono::high_resolution_clock::now();

	logText << "available time for search: " << availableSearchTime;
	Utility::WriteLog(logText.str());
	logText.clear();
	logText.str("");

	bestMove.TargetLocation = 0;
	availableMoves = MoveGenerator::GenerateMoves(position);
	std::sort(availableMoves.begin(), availableMoves.end(), MoveUtil::compareMoves);

	do
	{
		nodeCountAtBeginningOfIteration = nodeCount;
		alpha = -3000000;
		beta = 3000000;
		currentMaxDepth = depth;

		std::vector<Move> newLevelVector;
		pvArrays.push_back(newLevelVector);

		if (pvArrays[0].size() > 0)
		{
			auto it = std::find(availableMoves.begin(), availableMoves.end(), pvArrays[0][0]);

			availableMoves.erase(it);
			availableMoves.insert(availableMoves.begin(), pvArrays[0][0]);
		}

		for (auto it = pvArrays.begin(); it != pvArrays.end(); it++)
		{
			it->clear();
		}

		for (auto it = availableMoves.begin(); it != availableMoves.end(); it++)
		{
			Position::Make(position, *it);
			nodeCount++;

			currentBestScore = -Search(position, depth - 1);

			Position::Unmake(position, *it);

			if (randomMode)
			{
				currentBestScore += dist(generator);
			}

			if (currentBestScore > alpha)
			{
				alpha = currentBestScore;
				bestMove = *it;

				pvArrays[0].clear();
				pvArrays[0].push_back(*it);

				if (pvArrays.size() > 1)
				{
					pvArrays[0].insert(pvArrays[0].end(), pvArrays[1].begin(), pvArrays[1].end());
				}

				currentTime = std::chrono::high_resolution_clock::now();
				elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - start);
				msecs = elapsedTime.count();

				if (showThinkingOutput)
				{
					logText << depth << " " << alpha << " " << msecs / 10 << " " << nodeCount << BuildPVString();
					std::cout << logText.str() << std::endl;
					Utility::WriteLog(logText.str());
					logText.clear();
					logText.str("");
				}
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
			nodesPerMillisecond = 0x00FFFFFFFFFFFFFFULL;
		}
		else
		{
			nodesPerMillisecond = nodeCount / msecs;

			if (availableSearchTime > static_cast<uint64_t>(msecs / 10))
			{
				availableSearchTime = availableSearchTime - (msecs / 10);
			}
			else
			{
				availableSearchTime = 0;
			}

			if (nodesPerMillisecond == 0)
			{
				nodesPerMillisecond = 0x00FFFFFFFFFFFFFFULL;
			}
		}

		predictedSearchTime = (predictedNodesOfNextIteration / (nodesPerMillisecond * 10)) + (msecs / 10);

		logText << "elapsed time: " << msecs / 10 << "cs, predicted time: " << predictedSearchTime << "cs, effective branching factor: " << effectiveBranchingFactor;
		Utility::WriteLog(logText.str());
		logText.clear();
		logText.str("");

		depth++;

		if (depth > maxSearchDepth)
		{
			break;
		}
	} while (predictedSearchTime < availableSearchTime);

	pvArrays.clear();

	return bestMove;	
}

int Engine::Search(Board& position, int depth)
{
	int currentScore = 0;
	std::vector<Move> availableMoves;
	int max = -3000000;

	if (depth == 0)
	{
		return EvaluatePosition(position);
	}
	else
	{
		availableMoves = MoveGenerator::GenerateMoves(position);
		std::sort(availableMoves.begin(), availableMoves.end(), MoveUtil::compareMoves);

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
					pvArrays[currentMaxDepth - depth].clear();
					pvArrays[currentMaxDepth - depth].push_back(*it);

					if (pvArrays.size() > ((currentMaxDepth - depth) + 1))
					{
						pvArrays[currentMaxDepth - depth].insert(pvArrays[currentMaxDepth - depth].end(),
							pvArrays[(currentMaxDepth - depth) + 1].begin(), pvArrays[(currentMaxDepth - depth) + 1].end());
					}

					max = currentScore;
				}
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
