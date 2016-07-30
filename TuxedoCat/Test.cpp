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

void TuxedoCat::Test::TestPerft()
{
	Board board;
	uint64_t result;

	Position::SetPosition(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	
	result = Engine::Perft(board, 1);
	PrintTestResult("Perft: position 1, depth 1", result == 20);

	result = Engine::Perft(board, 2);
	PrintTestResult("Perft: position 1, depth 2", result == 400);

	result = Engine::Perft(board, 3);
	PrintTestResult("Perft: position 1, depth 3", result == 8902);

	result = Engine::Perft(board, 4);
	PrintTestResult("Perft: position 1, depth 4", result == 197281);

	result = Engine::Perft(board, 5);
	PrintTestResult("Perft: position 1, depth 5", result == 4865609);

	result = Engine::Perft(board, 6);
	PrintTestResult("Perft: position 1, depth 6", result == 119060324);

	Position::SetPosition(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

	result = Engine::Perft(board, 1);
	PrintTestResult("Perft: position 2, depth 1", result == 48);

	result = Engine::Perft(board, 2);
	PrintTestResult("Perft: position 2, depth 2", result == 2039);

	result = Engine::Perft(board, 3);
	PrintTestResult("Perft: position 2, depth 3", result == 97862);

	result = Engine::Perft(board, 4);
	PrintTestResult("Perft: position 2, depth 4", result == 4085603);

	result = Engine::Perft(board, 5);
	PrintTestResult("Perft: position 2, depth 5", result == 193690690);

	Position::SetPosition(board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

	result = Engine::Perft(board, 1);
	PrintTestResult("Perft: position 3, depth 1", result == 14);

	result = Engine::Perft(board, 2);
	PrintTestResult("Perft: position 3, depth 2", result == 191);

	result = Engine::Perft(board, 3);
	PrintTestResult("Perft: position 3, depth 3", result == 2812);

	result = Engine::Perft(board, 4);
	PrintTestResult("Perft: position 3, depth 4", result == 43238);

	result = Engine::Perft(board, 5);
	PrintTestResult("Perft: position 3, depth 5", result == 674624);

	result = Engine::Perft(board, 6);
	PrintTestResult("Perft: position 3, depth 6", result == 11030083);

	Position::SetPosition(board, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

	result = Engine::Perft(board, 1);
	PrintTestResult("Perft: position 4, depth 1", result == 6);

	result = Engine::Perft(board, 2);
	PrintTestResult("Perft: position 4, depth 2", result == 264);

	result = Engine::Perft(board, 3);
	PrintTestResult("Perft: position 4, depth 3", result == 9467);

	result = Engine::Perft(board, 4);
	PrintTestResult("Perft: position 4, depth 4", result == 422333);

	result = Engine::Perft(board, 5);
	PrintTestResult("Perft: position 4, depth 5", result == 15833292);

	Position::SetPosition(board, "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");

	result = Engine::Perft(board, 1);
	PrintTestResult("Perft: position 4 mirrored, depth 1", result == 6);

	result = Engine::Perft(board, 2);
	PrintTestResult("Perft: position 4 mirrored, depth 2", result == 264);

	result = Engine::Perft(board, 3);
	PrintTestResult("Perft: position 4 mirrored, depth 3", result == 9467);

	result = Engine::Perft(board, 4);
	PrintTestResult("Perft: position 4 mirrored, depth 4", result == 422333);

	result = Engine::Perft(board, 5);
	PrintTestResult("Perft: position 4 mirrored, depth 5", result == 15833292);

	Position::SetPosition(board, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

	result = Engine::Perft(board, 1);
	PrintTestResult("Perft: position 5, depth 1", result == 44);

	result = Engine::Perft(board, 2);
	PrintTestResult("Perft: position 5, depth 2", result == 1486);

	result = Engine::Perft(board, 3);
	PrintTestResult("Perft: position 5, depth 3", result == 62379);

	result = Engine::Perft(board, 4);
	PrintTestResult("Perft: position 5, depth 4", result == 2103487);

	result = Engine::Perft(board, 5);
	PrintTestResult("Perft: position 5, depth 5", result == 89941194);

	Position::SetPosition(board, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

	result = Engine::Perft(board, 1);
	PrintTestResult("Perft: position 6, depth 1", result == 46);

	result = Engine::Perft(board, 2);
	PrintTestResult("Perft: position 6, depth 2", result == 2079);

	result = Engine::Perft(board, 3);
	PrintTestResult("Perft: position 6, depth 3", result == 89890);

	result = Engine::Perft(board, 4);
	PrintTestResult("Perft: position 6, depth 4", result == 3894594);

	result = Engine::Perft(board, 5);
	PrintTestResult("Perft: position 6, depth 5", result == 164075551);

	std::cout << "test complete!" << std::endl << std::endl;
}

void TuxedoCat::Test::PrintTestResult(std::string name, bool result)
{
	std::cout << name << "...";

	if (result)
	{
		std::cout << "passed";
	}
	else
	{
		std::cout << "failed";
	}

	std::cout << std::endl;
}