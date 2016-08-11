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

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

namespace TuxedoCat
{
	enum class PieceRank { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NONE };
	enum class PieceColor { WHITE, BLACK, NONE };
	enum class TimeControlType { CONVENTIONAL, INCREMENTAL, TIME_PER_MOVE };


	struct CastlingFlags
	{
		static const int NONE = 0;
		static const int WHITE_SHORT = 1;
		static const int WHITE_LONG = 2;
		static const int BLACK_SHORT = 4;
		static const int BLACK_LONG = 8;
	};

	struct TimeControl
	{
		TimeControlType type;
		uint32_t movesPerControl;
		uint32_t remainingTime;
		uint32_t timeIncrement;
	};

	struct Move
	{
		uint64_t TargetLocation;
		uint64_t SourceLocation;
		uint64_t CurrentEnPassant;
		PieceColor MoveColor;
		PieceRank MovingPiece;
		PieceRank CapturedPiece;
		PieceRank PromotedRank;
		int CurrentHalfMoves;
		int CastlingStatus;
	};

	struct Board
	{
		uint64_t WhitePawns;
		uint64_t WhiteKnights;
		uint64_t WhiteBishops;
		uint64_t WhiteRooks;
		uint64_t WhiteQueens;
		uint64_t WhiteKing;
		uint64_t WhitePieces;

		uint64_t BlackPawns;
		uint64_t BlackKnights;
		uint64_t BlackBishops;
		uint64_t BlackRooks;
		uint64_t BlackQueens;
		uint64_t BlackKing;
		uint64_t BlackPieces;

		uint64_t EnPassantTarget;

		int CastlingStatus;
		int HalfMoveCounter;
		int FullMoveCounter;

		PieceColor ColorToMove;
	};

	PieceColor GetPieceColorFromChar(char piece);
	PieceRank GetPieceRankFromChar(char piece);

	namespace MoveUtil
	{
		void InitializeMove(Move& move, uint64_t tgt, uint64_t src, uint64_t cep, PieceColor mc, PieceRank mp,
			PieceRank cp, PieceRank pr, int chm, int cs);
		bool AreEqual(const Move& move1, const Move& move2);
	}

	namespace Position
	{
		void SetPosition(Board& position, std::string fen);
		void AddPieceAt(Board& position, uint64_t loc, PieceColor color, PieceRank rank);
		void RemovePieceAt(Board& position, uint64_t location);
		PieceRank GetRankAt(Board& position, uint64_t loc);
		PieceColor GetColorAt(Board& position, uint64_t loc);
		void Make(Board& position, Move mv);
		void Unmake(Board& position, Move mv);
		void UpdatePieces(Board& position);
		uint64_t GetPassedPawns(Board& position, PieceColor color);
		int GetDoubledPawnCount(Board& position, PieceColor color);
	}

	namespace MoveGenerator
	{
		bool IsPiecePinned(Board& position, uint64_t location, int offset);
		bool IsPiecePinnedNWSE(Board& position, uint64_t location);
		bool IsPiecePinnedSWNE(Board& position, uint64_t location);
		bool IsPiecePinnedRank(Board& position, uint64_t location);
		bool IsPiecePinnedFile(Board& position, uint64_t location);
		void GenerateSlidingMovesAt(Board& position, uint64_t location,
			uint16_t direction, bool evade);
		void AddMove(uint64_t src, uint64_t tgt, PieceRank rank, PieceColor color, int hm, int cs, uint64_t ep,
			PieceRank capture, PieceRank promotion, bool evade, Board& position);
		void GeneratePawnMovesAt(uint64_t location, Board& position, bool evade);
		void GenerateKnightMovesAt(uint64_t location, Board& position, bool evade);
		void GenerateKingMovesAt(uint64_t location, Board& position, bool evade);
		bool IsSquareAttacked(uint64_t square, Board& position);
		std::vector<Move> GenerateMoves(Board& position);
	}

	namespace Utility
	{
		std::vector<std::string> split(std::string str, std::string delimiter);
		uint64_t GetSquareFromAlgebraic(std::string algebraic);
		int GetLSB(uint64_t bitmask);
		int GetMSB(uint64_t bitmask);
		int PopCount(uint64_t bitmask);
		uint64_t GetMaskFromRankOrFile(char rankorfile);
		std::string GetFileFromLocation(uint64_t location);
		int GetRankFromLocation(uint64_t location);
		TuxedoCat::Move GetMoveFromXBoardNotation(Board& position, std::string moveString);
		std::string GenerateXBoardNotation(Move move);
		std::string GenerateSAN(Board& position, Move move, std::vector<Move> allMoves);
		std::string PrintMove(Move move);
		std::string RankToString(PieceRank rank);
		std::string ColorToString(PieceColor color);
		std::string CastlingStatusToString(int flags);
		void WriteLog(std::string msg);
	}

	namespace Engine
	{
		uint64_t Perft(Board& position, int depth);
		void Divide(Board& position, int depth);
		std::string GetMove(Board& position, TimeControl& clock);
		std::string GetRandomMove(Board& position);
		bool IsGameOver(Board& position);
		std::string GetGameResult(Board& position);
		void InitializeEngine();
		int EvaluatePosition(Board& position);
		int Search(Board& position, int depth);
		Move SearchRoot(Board& position, TimeControl& clock);
		uint64_t GetAvailableSearchTime(TimeControl& clock, Board& position);
	}

	namespace Interface
	{
		void Run();
		void ReadInput();
		void OutputFeatures();
	}

	namespace Test
	{
		void TestPerft();
		void GetDoubledPawnCountTest();
		void RunTests();

		template <class T>
		void PrintTestResult(std::string name, T expected, T actual)
		{
			std::cout << name << "...";

			if (expected == actual)
			{
				std::cout << "passed";
			}
			else
			{
				std::cout << "failed" << std::endl;
				std::cout << "    expected " << actual << " to equal " << expected;
			}

			std::cout << std::endl;
		}
	}
}

extern struct TuxedoCat::Board currentPosition;
extern struct TuxedoCat::TimeControl currentClock;
extern int maxSearchDepth;
extern bool randomMode;
extern uint64_t KnightAttacks[64];
extern uint64_t KingAttacks[64];
extern uint64_t RayAttacksN[64];
extern uint64_t RayAttacksS[64];
extern uint64_t RayAttacksE[64];
extern uint64_t RayAttacksW[64];
extern uint64_t RayAttacksNW[64];
extern uint64_t RayAttacksSW[64];
extern uint64_t RayAttacksNE[64];
extern uint64_t RayAttacksSE[64];
