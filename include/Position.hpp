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

#include "CastlingStatus.hpp"
#include "Color.hpp"
#include "Piece.hpp"
#include "Move.hpp"
#include "Direction.hpp"
#include "MoveList.hpp"
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
        Position(const Position& p);

        void generateMoves(Rank rank, MoveList& moves);
        Move getMoveFromString(std::string s) const;
        void makeMove(const Move& move);
        std::string toString() const;
        void unmakeMove();
        bool isSquareOccupied(int squareIndex) const;
        bool isInCheck() const;
        int getEnPassantIndex() const;
    private:
        void addPieceAt(uint64_t location, Color c, Rank r);
        void calculateInCheck();
        void calculatePinnedPieces();
        void computeSlidingMoves(int index, Piece p, bool highBitBlock,
            const std::array<uint64_t, 64>& rayMask, MoveList& moves);
        void generateCastles(MoveList& moves);
        void generateKingMovesAt(uint64_t s, MoveList& moves);
        void generateKnightMoves(MoveList& moves);
        void generatePawnAdvances(MoveList& moves);
        void generatePawnCaptures(MoveList& moves);
        void generateSlidingMovesAt(Piece p, Direction d,
            MoveList& moves);
        int getBlockerIndex(uint64_t mask, bool highBitBlock);
        void getMovesFromMask(uint64_t mask, Piece p, MoveList& moves);
        bool getHighBitBlockerByDirection(Direction direction) const;
        int getOffsetFromDirection(Direction direction) const;
        uint64_t getOpposingPieces(Color c) const;
        uint64_t getOwnPieces(Color c) const;
        Piece getPieceAt(uint64_t s) const;
        uint64_t getPinnedPieceByDirection(int kingIndex, uint64_t ownPieces,
            Direction d) const;
        int getPotentialPinningPiece(int locationIndex, Direction dir) const;
        bool isCastleLegal(uint64_t s) const;
        bool isMoveLegal(const Move& m);
        bool isSlidingPiecePinned(const Piece p, Direction d) const;
        bool isSquareAttacked(uint64_t s) const;
        void removePieceAt(uint64_t location);
        void parseFEN(std::string fen);
        void updatePieces();

        uint64_t allPieces;
        uint64_t blackBishops;
        uint64_t blackKing;
        uint64_t blackKnights;
        uint64_t blackPawns;
        uint64_t blackPieces;
        uint64_t blackQueens;
        uint64_t blackRooks;
        CastlingStatus castlingStatus;
        Color colorToMove;
        uint64_t enPassantTarget;
        uint32_t fullMoveCounter;
        uint32_t halfMoveCounter;
        std::stack<Position> positionStack;
        uint64_t whiteBishops;
        uint64_t whiteKing;
        uint64_t whiteKnights;
        uint64_t whitePawns;
        uint64_t whitePieces;
        uint64_t whiteQueens;
        uint64_t whiteRooks;

        uint64_t currentPinnedPiecesN;
        uint64_t currentPinnedPiecesE;
        uint64_t currentPinnedPiecesW;
        uint64_t currentPinnedPiecesS;
        uint64_t currentPinnedPiecesNE;
        uint64_t currentPinnedPiecesNW;
        uint64_t currentPinnedPiecesSE;
        uint64_t currentPinnedPiecesSW;
        uint64_t currentPinnedPieces;

        bool inCheck;
    };
}
