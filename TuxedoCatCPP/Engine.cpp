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

using namespace TuxedoCat;
using namespace TuxedoCat::MoveGenerator;
using namespace TuxedoCat::Position;
using namespace TuxedoCat::Utility;

Board currentPosition;

void Engine::InitializeEngine()
{
	SetPosition(currentPosition, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

uint64_t Engine::Perft(Board& position, int depth)
{
	if (depth <= 1)
	{
		return static_cast<uint64_t>(GenerateMoves(position).size());
	}
	else
	{
		uint64_t count = 0;
		std::vector<Move> availableMoves = GenerateMoves(position);

		for (auto it = availableMoves.cbegin(); it != availableMoves.cend(); it++)
		{
			Make(position, *it);
			count += Perft(position, depth - 1);
			Unmake(position, *it);
		}

		return count;
	}
}

void Engine::Divide(Board& position, int depth)
{
	uint64_t totalCount = 0;
	int moveCount = 0;
	std::vector<Move> availableMoves = GenerateMoves(position);

	if (depth <= 1)
	{
		for (auto it = availableMoves.cbegin(); it != availableMoves.cend(); it++)
		{
			moveCount++;
			totalCount++;
			std::cout << GenerateSAN(position, *it, availableMoves) << ": 1" << std::endl;
		}
	}
	else
	{
		for (auto it = availableMoves.cbegin(); it != availableMoves.cend(); it++)
		{
			moveCount++;

			Make(position, *it);

			uint64_t count = Perft(position, depth - 1);

			totalCount += count;

			std::cout << GenerateSAN(position, *it, availableMoves) << ": " << std::to_string(count) << std::endl;

			Unmake(position, *it);
		}
	}

	std::cout << std::endl << "Moves: " << moveCount << std::endl << "Total leaf nodes: " << totalCount << std::endl;
}