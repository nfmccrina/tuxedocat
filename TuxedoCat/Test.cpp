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

void Test::RunTests()
{
	TestPerft();
	GetDoubledPawnCountTest();

	std::cout << "Tests complete!" << std::endl;
}

void Test::TestPerft()
{
	Board board;

	Position::SetPosition(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	
	PrintTestResult("Perft: position 1, depth 1", static_cast<uint64_t>(20), Engine::Perft(board, 1));
	PrintTestResult("Perft: position 1, depth 2", static_cast<uint64_t>(400), Engine::Perft(board, 2));
	PrintTestResult("Perft: position 1, depth 3", static_cast<uint64_t>(8902), Engine::Perft(board, 3));
	PrintTestResult("Perft: position 1, depth 4", static_cast<uint64_t>(197281), Engine::Perft(board, 4));
	PrintTestResult("Perft: position 1, depth 5", static_cast<uint64_t>(4865609), Engine::Perft(board, 5));
	PrintTestResult("Perft: position 1, depth 6", static_cast<uint64_t>(119060324), Engine::Perft(board, 6));

	Position::SetPosition(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

	PrintTestResult("Perft: position 2, depth 1", static_cast<uint64_t>(48), Engine::Perft(board, 1));
	PrintTestResult("Perft: position 2, depth 2", static_cast<uint64_t>(2039), Engine::Perft(board, 2));
	PrintTestResult("Perft: position 2, depth 3", static_cast<uint64_t>(97862), Engine::Perft(board, 3));
	PrintTestResult("Perft: position 2, depth 4", static_cast<uint64_t>(4085603), Engine::Perft(board, 4));
	PrintTestResult("Perft: position 2, depth 5", static_cast<uint64_t>(193690690), Engine::Perft(board, 5));

	Position::SetPosition(board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

	PrintTestResult("Perft: position 3, depth 1", static_cast<uint64_t>(14), Engine::Perft(board, 1));
	PrintTestResult("Perft: position 3, depth 2", static_cast<uint64_t>(191), Engine::Perft(board, 2));
	PrintTestResult("Perft: position 3, depth 3", static_cast<uint64_t>(2812), Engine::Perft(board, 3));
	PrintTestResult("Perft: position 3, depth 4", static_cast<uint64_t>(43238), Engine::Perft(board, 4));
	PrintTestResult("Perft: position 3, depth 5", static_cast<uint64_t>(674624), Engine::Perft(board, 5));
	PrintTestResult("Perft: position 3, depth 6", static_cast<uint64_t>(11030083), Engine::Perft(board, 6));

	Position::SetPosition(board, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

	PrintTestResult("Perft: position 4, depth 1", static_cast<uint64_t>(6), Engine::Perft(board, 1));
	PrintTestResult("Perft: position 4, depth 2", static_cast<uint64_t>(264), Engine::Perft(board, 2));
	PrintTestResult("Perft: position 4, depth 3", static_cast<uint64_t>(9467), Engine::Perft(board, 3));
	PrintTestResult("Perft: position 4, depth 4", static_cast<uint64_t>(422333), Engine::Perft(board, 4));
	PrintTestResult("Perft: position 4, depth 5", static_cast<uint64_t>(15833292), Engine::Perft(board, 5));

	Position::SetPosition(board, "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");

	PrintTestResult("Perft: position 4 mirrored, depth 1", static_cast<uint64_t>(6), Engine::Perft(board, 1));
	PrintTestResult("Perft: position 4 mirrored, depth 2", static_cast<uint64_t>(264), Engine::Perft(board, 2));
	PrintTestResult("Perft: position 4 mirrored, depth 3", static_cast<uint64_t>(9467), Engine::Perft(board, 3));
	PrintTestResult("Perft: position 4 mirrored, depth 4", static_cast<uint64_t>(422333), Engine::Perft(board, 4));
	PrintTestResult("Perft: position 4 mirrored, depth 5", static_cast<uint64_t>(15833292), Engine::Perft(board, 5));

	Position::SetPosition(board, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

	PrintTestResult("Perft: position 5, depth 1", static_cast<uint64_t>(44), Engine::Perft(board, 1));
	PrintTestResult("Perft: position 5, depth 2", static_cast<uint64_t>(1486), Engine::Perft(board, 2));
	PrintTestResult("Perft: position 5, depth 3", static_cast<uint64_t>(62379), Engine::Perft(board, 3));
	PrintTestResult("Perft: position 5, depth 4", static_cast<uint64_t>(2103487), Engine::Perft(board, 4));
	PrintTestResult("Perft: position 5, depth 5", static_cast<uint64_t>(89941194), Engine::Perft(board, 5));

	Position::SetPosition(board, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

	PrintTestResult("Perft: position 6, depth 1", static_cast<uint64_t>(46), Engine::Perft(board, 1));
	PrintTestResult("Perft: position 6, depth 2", static_cast<uint64_t>(2079), Engine::Perft(board, 2));
	PrintTestResult("Perft: position 6, depth 3", static_cast<uint64_t>(89890), Engine::Perft(board, 3));
	PrintTestResult("Perft: position 6, depth 4", static_cast<uint64_t>(3894594), Engine::Perft(board, 4));
	PrintTestResult("Perft: position 6, depth 5", static_cast<uint64_t>(164075551), Engine::Perft(board, 5));
}

void Test::GetDoubledPawnCountTest()
{
	Board position;

	position.WhitePawns = 0x0000000064920900ULL;

	PrintTestResult("GetDoubledPawnCount, scenario 1", 0, Position::GetDoubledPawnCount(position, PieceColor::WHITE));

	position.BlackPawns = 0x00A5520800000000ULL;

	PrintTestResult("GetDoubledPawnCount, scenario 2", 0, Position::GetDoubledPawnCount(position, PieceColor::BLACK));

	position.WhitePawns = 0x000000002020CF00ULL;

	PrintTestResult("GetDoubledPawnCount, scenario 3", 1, Position::GetDoubledPawnCount(position, PieceColor::WHITE));

	position.BlackPawns = 0x0093484800000000ULL;

	PrintTestResult("GetDoubledPawnCount, scenario 4", 2, Position::GetDoubledPawnCount(position, PieceColor::BLACK));
}