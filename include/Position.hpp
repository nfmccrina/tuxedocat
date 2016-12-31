/*
* The MIT License (MIT)
*
* Copyright (c) 2016 Nathan McCrina
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation 
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "Square.hpp"
#include "CastlingStatus.hpp"
#include "Color.hpp"
#include "Piece.hpp"
#include "Move.hpp"
#include "Direction.hpp"
#include <string>
#include <vector>
#include <stack>
#include <array>

namespace TuxedoCat
{
    class Position
    {
    public:
        Position(std::string fen);

        std::vector<Move> generateMoves(Rank rank = Rank::NONE);
        Position(const Position& p);

        void makeMove(Move move);
        std::string toString() const;
        void unmakeMove();
    private:
        void addPieceAt(Bitboard location, Color c, Rank r);
        std::vector<Move> computeSlidingMoves(int index, Piece p,
            const std::array<uint64_t, 64>& rayMask);

        Bitboard computePinningPieceMask(Direction direction) const;
        std::vector<Square> findPiece(Color c, Rank r) const;
        std::vector<Move> generateCastles();
        std::vector<Move> generateKingMovesAt(Square s);
        std::vector<Move> generateKnightMovesAt(Square s);
        std::vector<Move> generatePawnAdvancesAt(Bitboard b);
        std::vector<Move> generatePawnCapturesAt(Bitboard b);
        std::vector<Move> generatePawnDblAdvancesAt(Bitboard b);
        std::vector<Move> generateSlidingMovesN(Bitboard b);
        std::vector<Move> generateSlidingMovesS(Bitboard b);
        std::vector<Move> generateSlidingMovesE(Bitboard b);
        std::vector<Move> generateSlidingMovesW(Bitboard b);
        std::vector<Move> generateSlidingMovesNE(Bitboard b);
        std::vector<Move> generateSlidingMovesNW(Bitboard b);
        std::vector<Move> generateSlidingMovesSE(Bitboard b);
        std::vector<Move> generateSlidingMovesSW(Bitboard b);
        std::vector<Move> getMovesFromMask(Bitboard mask, Piece p,
            bool inCheck);
        int getOffsetFromDirection(Direction direction) const;
        Bitboard getOpposingPieces(Color c) const;
        Bitboard getOwnPieces(Color c) const;
        Piece getPieceAt(Square s) const;
        bool isCastleLegal(Square s) const;
        bool isInCheck(Color c) const;
        bool isMoveValid(const Move& m);
        bool isPiecePinned(const Piece pinnedPiece,
            Direction direction) const;
        bool isSquareAttacked(Square s) const;
        bool isSquareEmpty(Square s) const;
        void removePieceAt(Bitboard location);
        void parseFEN(std::string fen);
        void updatePieces();

        Bitboard allPieces;
        Bitboard blackBishops;
        Bitboard blackKing;
        Bitboard blackKnights;
        Bitboard blackPawns;
        Bitboard blackPieces;
        Bitboard blackQueens;
        Bitboard blackRooks;
        CastlingStatus castlingStatus;
        Color colorToMove;
        Bitboard enPassantTarget;
        uint32_t fullMoveCounter;
        uint32_t halfMoveCounter;
        std::stack<Position> positionStack;
        Bitboard whiteBishops;
        Bitboard whiteKing;
        Bitboard whiteKnights;
        Bitboard whitePawns;
        Bitboard whitePieces;
        Bitboard whiteQueens;
        Bitboard whiteRooks;
    };
}
