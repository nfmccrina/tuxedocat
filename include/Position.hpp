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
#include <boost/optional.hpp>
#include <string>
#include <vector>
#include <stack>

namespace TuxedoCat
{
    class Position
    {
    public:
        Position(std::string fen);

        std::vector<Move> generateMoves() const;
        /*Position(const Position& p);

        bool isMoveValid(const Move& m);
        void makeMove(boost::optional<const Move&> move);
        std::string toString() const;
        void unmakeMove();*/
    private:
        void addPieceAt(Bitboard location, Color c, Rank r);
        Bitboard computePinningPieceMask(Direction direction) const;
        std::vector<Move> generatePawnAdvancesAt(Bitboard b) const;
        std::vector<Move> generatePawnCapturesAt(Bitboard b) const;
        std::vector<Move> generatePawnDblAdvancesAt(Bitboard b) const;
        /*std::vector<Square> findPiece(Color c, Rank r) const;
        std::vector<Move> generateCastles() const;
        std::vector<Move> generateKingMovesAt(const Square& s) const;
        std::vector<Move> generateKnightMovesAt(const Square& s,
            bool inCheck) const;
        std::vector<Move> generatePawnMovesAt(const Square& s) const;*/
        int getOffsetFromDirection(Direction direction) const;
        boost::optional<Piece> getPieceAt(Square s) const;
        bool isPiecePinned(const Piece pinnedPiece,
            Direction direction) const;
        /*bool isSquareAttacked(Square s) const;*/
        bool isSquareEmpty(Square s) const;
        /*void removePieceAt(Bitboard location);*/
        void parseFEN(std::string fen);
        void updatePieces();

        Bitboard blackBishops;
        Bitboard blackKing;
        Bitboard blackKnights;
        Bitboard blackPawns;
        Bitboard blackPieces;
        Bitboard blackQueens;
        Bitboard blackRooks;
        CastlingStatus castlingStatus;
        Color colorToMove;
        boost::optional<Bitboard> enPassantTarget;
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
