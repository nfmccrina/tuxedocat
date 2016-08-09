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
std::stack<std::string> pvStrings;

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
	std::default_random_engine generator(static_cast<unsigned int>(std::time(0)));
	std::uniform_int_distribution<> dist(-10, 10);

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


	// advanced pawns
	score += (35 * Utility::PopCount(position.WhitePawns & 0x00FF000000000000ULL));
	score += (30 * Utility::PopCount(position.WhitePawns & 0x0000FF0000000000ULL));
	score += (20 * Utility::PopCount(position.WhitePawns & 0x000000FF00000000ULL));

	score -= (35 * Utility::PopCount(position.BlackPawns & 0x000000000000FF00ULL));
	score -= (30 * Utility::PopCount(position.BlackPawns & 0x0000000000FF0000ULL));
	score -= (20 * Utility::PopCount(position.BlackPawns & 0x00000000FF000000ULL));

	if (randomMode)
	{
		score += dist(generator);
	}

	return (score * sideToMoveFactor);
}

Move Engine::NegaMaxRoot(Board& position)
{
	int max = 0;
	int depth = 1;
	int currentScore = 0;
	std::vector<Move> availableMoves;
	Move bestMove;
	std::stringstream logText;
	std::stringstream thinkingOutput;
	std::string pvStr;
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point currentTime;
	std::chrono::milliseconds elapsedTime;
	uint32_t estimatedLeafNodesAtNextDepth = 0;
	uint32_t leafNodesAtCurrentDepth = 0;
	uint32_t leafNodesAtPreviousDepth = 0;
	uint32_t movesRemainingUntilNextTimeControl = 0;
	uint32_t availableTimeForThisMove = 0;
	double branchingFactorSum = 0;
	double effectiveBranchingFactor = 0;
	uint32_t timeRequiredForNextIteration = 0;
	uint64_t nodeCountAtPreviousDepth;

	nodeCount = 0;

	if (currentClock.type == TimeControlType::CONVENTIONAL)
	{
		if (currentClock.movesPerControl == 0)
		{
			movesRemainingUntilNextTimeControl = 30;
		}
		else
		{
			movesRemainingUntilNextTimeControl = currentClock.movesPerControl - ((currentPosition.FullMoveCounter - 1) % currentClock.movesPerControl);
			
			if (movesRemainingUntilNextTimeControl == 0)
			{
				movesRemainingUntilNextTimeControl = 1;
			}
		}
		
		availableTimeForThisMove = (currentClock.remainingTime - 100) / movesRemainingUntilNextTimeControl;
	}
	else if (currentClock.type == TimeControlType::INCREMENTAL)
	{
		availableTimeForThisMove = ((currentClock.remainingTime - 100) / 30) + (currentClock.timeIncrement - 50);
	}
	else if (currentClock.type == TimeControlType::TIME_PER_MOVE)
	{
		availableTimeForThisMove = currentClock.remainingTime - 10;
	}
	
	start = std::chrono::high_resolution_clock::now();

	while (true)
	{
		max = -3000000;
		bestMove.TargetLocation = 0;
		nodeCountAtPreviousDepth = nodeCount;
		leafNodesAtPreviousDepth = leafNodesAtCurrentDepth;

		availableMoves = MoveGenerator::GenerateMoves(position);

		for (auto it = availableMoves.begin(); it != availableMoves.end(); it++)
		{
			Position::Make(position, *it);
			nodeCount++;

			currentScore = -NegaMax(position, depth - 1);

			Position::Unmake(position, *it);

			if (currentScore > max)
			{
				pvStrings.push(Utility::GenerateXBoardNotation(*it));

				while (!pvStrings.empty())
				{
					thinkingOutput << pvStrings.top() << " ";
					pvStrings.pop();
				}

				pvStr = thinkingOutput.str().substr(0, thinkingOutput.str().size() - 1);
				thinkingOutput.clear();
				thinkingOutput.str("");

				max = currentScore;
				bestMove = *it;
			}
			else
			{
				while (!pvStrings.empty())
				{
					pvStrings.pop();
				}
			}
		}

		leafNodesAtCurrentDepth = static_cast<uint32_t>(nodeCount - nodeCountAtPreviousDepth);

		if (leafNodesAtPreviousDepth == 0)
		{
			branchingFactorSum += leafNodesAtCurrentDepth;
			effectiveBranchingFactor = branchingFactorSum;
		}
		else
		{
			branchingFactorSum += leafNodesAtCurrentDepth / (leafNodesAtPreviousDepth * 1.0);
			effectiveBranchingFactor = branchingFactorSum / depth;
		}

		estimatedLeafNodesAtNextDepth = static_cast<int>(leafNodesAtCurrentDepth * effectiveBranchingFactor);

		currentTime = std::chrono::high_resolution_clock::now();
		elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - start);
		long long msecs = elapsedTime.count();

		logText << "Search depth: " << depth << ", node count: " << nodeCount << ", elapsed time: " << msecs << "ms, nps: " << nodeCount / (msecs / 1000.0);
		Utility::WriteLog(logText.str());
		logText.clear();
		logText.str("");

		timeRequiredForNextIteration = static_cast<uint32_t>((nodeCount + estimatedLeafNodesAtNextDepth) / (nodeCount / (msecs / 10.0)));

		logText << "Leaf nodes at next depth: " << estimatedLeafNodesAtNextDepth << ", estimated time for search at next depth: " << timeRequiredForNextIteration
			<< ", allocated search time: " << availableTimeForThisMove << std::endl;

		Utility::WriteLog(logText.str());
		logText.clear();
		logText.str("");

		thinkingOutput << depth << " " << max << " " << msecs / 10 << " " << nodeCount << " " << pvStr;

		std::cout << thinkingOutput.str() << std::endl;
		Utility::WriteLog(thinkingOutput.str());
		thinkingOutput.clear();
		thinkingOutput.str("");

		if (((msecs / 10) + timeRequiredForNextIteration) >= availableTimeForThisMove)
		{
			break;
		}
		else
		{
			depth++;

			if (depth > maxSearchDepth)
			{
				break;
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
	Move localBestMove;
	std::stack<std::string> tmpStack;

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

				pvStrings.push(Utility::GenerateXBoardNotation(*it));

				currentScore = -NegaMax(position, depth - 1);

				Position::Unmake(position, *it);

				if (currentScore > max)
				{
					if (max != -1999999)
					{
						for (int count = 0; count < depth; count++)
						{
							tmpStack.push(pvStrings.top());
							pvStrings.pop();
						}

						for (int count = 0; count < depth; count++)
						{
							pvStrings.pop();
						}

						for (int count = 0; count < depth; count++)
						{
							pvStrings.push(tmpStack.top());
							tmpStack.pop();
						}
					}

					localBestMove = *it;
					max = currentScore;
				}
				else
				{
					for (int count = 0; count < depth; count++)
					{
						pvStrings.pop();
					}
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
