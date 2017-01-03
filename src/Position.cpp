/*
* The MIT License (MIT)
*
* Copyright (c) 2016 Nathan McCrina
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the
* following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
* NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
* USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "../include/Position.hpp"
#include "../include/LookupData.hpp"
#include "../include/Utility.hpp"
#include "../include/MoveSearchCriteria.hpp"
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>

using namespace TuxedoCat;

// begin constructors

Position::Position(std::string fen)
    : castlingStatus(CastlingStatus(false, false, false, false))
{
    parseFEN(fen);
}

Position::Position(const Position& p)
    : castlingStatus(p.castlingStatus)
{
    whitePawns = p.whitePawns;
    whiteKnights = p.whiteKnights;
    whiteBishops = p.whiteBishops;
    whiteRooks = p.whiteRooks;
    whiteQueens = p.whiteQueens;
    whiteKing = p.whiteKing;
    whitePieces = p.whitePieces;
    blackPawns = p.blackPawns;
    blackKnights = p.blackKnights;
    blackBishops = p.blackBishops;
    blackRooks = p.blackRooks;
    blackQueens = p.blackQueens;
    blackKing = p.blackKing;
    blackPieces = p.blackPieces;
    enPassantTarget = p.enPassantTarget;
    halfMoveCounter = p.halfMoveCounter;
    fullMoveCounter = p.fullMoveCounter;
    colorToMove = p.colorToMove;
    allPieces = p.allPieces;
}

// end constructors

// begin public methods

void Position::generateMoves(Rank rank, MoveList& moves)
{
    Bitboard pieces;
    Bitboard currentSquare;
    int currentIndex;

    if (colorToMove == Color::WHITE)
    {
        pieces = whitePieces;
    }
    else
    {
        pieces = blackPieces;
    }

    if (rank == Rank::NONE || rank == Rank::KING)
    {
        generateCastles(moves);
    }

    while (pieces != 0x00ULL)
    {
        currentIndex = pieces.lsb();
        currentSquare = 0x01ULL << currentIndex;

        if (rank == Rank::NONE || rank == Rank::PAWN)
        {
            generatePawnCapturesAt(currentSquare, moves);
            generatePawnAdvancesAt(currentSquare, moves);
            generatePawnDblAdvancesAt(currentSquare, moves);
        }

        if (rank == Rank::NONE || rank == Rank::KNIGHT)
        {
            generateKnightMovesAt(currentSquare, moves);
        }

        if (rank == Rank::NONE || rank == Rank::KING)
        {
            generateKingMovesAt(currentSquare, moves);
        }

        if ((rank == Rank::NONE || rank == Rank::BISHOP) &&
            getPieceAt(currentSquare).getRank() == Rank::BISHOP)
        {
            generateSlidingMovesAt(currentSquare,
                Direction::NE, moves);
            
            generateSlidingMovesAt(currentSquare,
                Direction::NW, moves);

            generateSlidingMovesAt(currentSquare,
                Direction::SE, moves);

            generateSlidingMovesAt(currentSquare,
                Direction::SW, moves);
        }

        if ((rank == Rank::NONE || rank == Rank::ROOK) &&
            getPieceAt(currentSquare).getRank() == Rank::ROOK)
        {
            generateSlidingMovesAt(currentSquare,
                Direction::N, moves);
            
            generateSlidingMovesAt(currentSquare,
                Direction::W, moves);

            generateSlidingMovesAt(currentSquare,
                Direction::E, moves);

            generateSlidingMovesAt(currentSquare,
                Direction::S, moves);
        }

        if ((rank == Rank::NONE || rank == Rank::QUEEN) &&
            getPieceAt(currentSquare).getRank() == Rank::QUEEN)
        {
            generateSlidingMovesAt(currentSquare,
                Direction::N, moves);
            
            generateSlidingMovesAt(currentSquare,
                Direction::W, moves);

            generateSlidingMovesAt(currentSquare,
                Direction::E, moves);

            generateSlidingMovesAt(currentSquare,
                Direction::S, moves);

            generateSlidingMovesAt(currentSquare,
                Direction::NE, moves);
            
            generateSlidingMovesAt(currentSquare,
                Direction::NW, moves);

            generateSlidingMovesAt(currentSquare,
                Direction::SE, moves);

            generateSlidingMovesAt(currentSquare,
                Direction::SW, moves);
        }

        pieces.flipBit(currentIndex);
    }

    computeMoveNotation(moves);
}

Move Position::getMoveFromString(std::string s) const
{
    if (s.size() < 4)
    {
        return Move(Piece(), Square(), Rank::NONE);
    }

    Square source {s.substr(0, 2)};
    Square target {s.substr(2, 2)};

    if (s.size() < 5)
    {
        return Move(getPieceAt(source), target, Rank::NONE);
    }
    else
    {
        return Move(getPieceAt(source), target,
            getRankFromString(s.substr(4, 1)));
    }
}

void Position::makeMove(const Move& move)
{
    Bitboard sourceLocation;
    Bitboard targetLocation;
    Bitboard tmpEnPassant;
    Color sourceColor;
    Rank sourceRank;
    Rank promotedRank;
    Piece capturedPiece;

    positionStack.push(*this);

    if (!move.isValid())
    {
        if (colorToMove == Color::WHITE)
        {
            colorToMove = Color::BLACK;
        }
        else
        {
            colorToMove = Color::WHITE;
            fullMoveCounter++;
        }

        return;
    }

    

    sourceLocation = move.getMovingPiece().getSquare().toBitboard();
    targetLocation = move.getTargetSquare().toBitboard();
    sourceColor = move.getMovingPiece().getColor();
    sourceRank = move.getMovingPiece().getRank();
    capturedPiece = getPieceAt(Square(targetLocation));
    promotedRank = move.getPromotedRank();

    if (!enPassantTarget.isEmpty())
    {
        tmpEnPassant = enPassantTarget;
    }

    if (
        (
            (sourceLocation & whiteKing) != 0x00000000000000ULL ||
            (sourceLocation & blackKing) != 0x00000000000000ULL
        ) &&
        (
            (
                sourceLocation == 0x0000000000000010ULL &&
                targetLocation == 0x0000000000000004ULL
            ) ||
            (
                sourceLocation == 0x0000000000000010ULL &&
                targetLocation == 0x0000000000000040ULL
            ) ||
            (
                sourceLocation == 0x1000000000000000ULL &&
                targetLocation == 0x0400000000000000ULL
            ) ||
            (
                sourceLocation == 0x1000000000000000ULL &&
                targetLocation == 0x4000000000000000ULL
            )
        ))
    {
        // castle
        halfMoveCounter = 0;

        if (sourceLocation == 0x0000000000000010ULL &&
            targetLocation == 0x0000000000000004ULL)
        {
            // white queen-side
            addPieceAt(0x0000000000000004ULL, Color::WHITE, Rank::KING);
            addPieceAt(0x0000000000000008ULL, Color::WHITE, Rank::ROOK);
            removePieceAt(0x0000000000000010ULL);
            removePieceAt(0x0000000000000001ULL);
        }
        else if (sourceLocation == 0x0000000000000010ULL &&
            targetLocation == 0x0000000000000040ULL)
        {
            // white king-side
            addPieceAt(0x0000000000000040ULL, Color::WHITE, Rank::KING);
            addPieceAt(0x0000000000000020ULL, Color::WHITE, Rank::ROOK);
            removePieceAt(0x0000000000000010ULL);
            removePieceAt(0x0000000000000080ULL);
        }
        else if (sourceLocation == 0x1000000000000000ULL &&
            targetLocation == 0x0400000000000000ULL)
        {
            // black queen-side
            addPieceAt(0x0400000000000000ULL, Color::BLACK, Rank::KING);
            addPieceAt(0x0800000000000000ULL, Color::BLACK, Rank::ROOK);
            removePieceAt(0x1000000000000000ULL);
            removePieceAt(0x0100000000000000ULL);
        }
        else if (sourceLocation == 0x1000000000000000ULL &&
            targetLocation == 0x4000000000000000ULL)
        {
            // black king-side
            addPieceAt(0x4000000000000000ULL, Color::BLACK, Rank::KING);
            addPieceAt(0x2000000000000000ULL, Color::BLACK, Rank::ROOK);
            removePieceAt(0x1000000000000000ULL);
            removePieceAt(0x8000000000000000ULL);
        }
    }
    else
    {
        // not a castle
        if (sourceRank == Rank::PAWN)
        {
            // pawn move
            halfMoveCounter = 0;

            if (capturedPiece.isValid())
            {
                if (!enPassantTarget.isEmpty())
                {
                    if (targetLocation == enPassantTarget)
                    {
                        if (sourceColor == Color::WHITE)
                        {
                            removePieceAt(targetLocation >> 8);
                        }
                        else
                        {
                            removePieceAt(targetLocation << 8);
                        }
                    }
                }
                else
                {
                    removePieceAt(targetLocation);
                }
            }
            else
            {
                if (targetLocation == (sourceLocation << 16))
                {
                    enPassantTarget = sourceLocation << 8;
                }
                else if (targetLocation == (sourceLocation >> 16))
                {
                    enPassantTarget = sourceLocation >> 8;
                }
            }

            if (promotedRank != Rank::NONE)
            {
                addPieceAt(targetLocation, sourceColor, promotedRank);
            }
            else
            {
                addPieceAt(targetLocation, sourceColor, Rank::PAWN);
            }

            removePieceAt(sourceLocation);
        }
        else
        {
            if (capturedPiece.isValid())
            {
                removePieceAt(targetLocation);
                halfMoveCounter = 0;
            }

            addPieceAt(targetLocation, sourceColor, sourceRank);
            removePieceAt(sourceLocation);
        }
    }

    if (colorToMove == Color::WHITE)
    {
        colorToMove = Color::BLACK;
    }
    else
    {
        colorToMove = Color::WHITE;
        fullMoveCounter++;
    }

    if (!enPassantTarget.isEmpty())
    {
        if (tmpEnPassant == enPassantTarget)
        {
            enPassantTarget = 0x00ULL;
        }
    }

    if ((whiteKing & 0x0000000000000010ULL) == 0x00)
    {
        castlingStatus.setWhiteKingSide(false);
        castlingStatus.setWhiteQueenSide(false);
    }

    if ((whiteRooks & 0x0000000000000001ULL) == 0x00)
    {
        castlingStatus.setWhiteQueenSide(false);
    }

    if ((whiteRooks & 0x0000000000000080ULL) == 0x00)
    {
        castlingStatus.setWhiteKingSide(false);
    }

    if ((blackKing & 0x1000000000000000ULL) == 0x00)
    {
        castlingStatus.setBlackKingSide(false);
        castlingStatus.setBlackQueenSide(false);
    }

    if ((blackRooks & 0x0100000000000000ULL) == 0x00)
    {
        castlingStatus.setBlackQueenSide(false);
    }

    if ((blackRooks & 0x8000000000000000ULL) == 0x00)
    {
        castlingStatus.setBlackKingSide(false);
    }
}

std::string Position::toString() const
{
    Piece currentPiece;
    std::stringstream ss;

    for (int row = 7; row >= 0; row--)
    {
        for (int col = 0; col < 8; col++)
        {
            currentPiece =
                getPieceAt(Square(std::pair<int, int>(row, col)));
            if (currentPiece.isValid())
            {
                ss << currentPiece.toString();
            }
            else
            {
                ss << "-";
            }

            if (col != 7)
            {
                ss << " ";
            }
        }

        ss << std::endl;
    }

    ss << "To Move: " << colorToString(colorToMove) << std::endl;    

    return ss.str();
}

void Position::unmakeMove()
{
    if (!positionStack.empty())
    {
        Position p { positionStack.top() };
        
        whitePawns = p.whitePawns;
        whiteKnights = p.whiteKnights;
        whiteBishops = p.whiteBishops;
        whiteRooks = p.whiteRooks;
        whiteQueens = p.whiteQueens;
        whiteKing = p.whiteKing;
        whitePieces = p.whitePieces;
        blackPawns = p.blackPawns;
        blackKnights = p.blackKnights;
        blackBishops = p.blackBishops;
        blackRooks = p.blackRooks;
        blackQueens = p.blackQueens;
        blackKing = p.blackKing;
        blackPieces = p.blackPieces;
        enPassantTarget = p.enPassantTarget;
        castlingStatus = p.castlingStatus;
        halfMoveCounter = p.halfMoveCounter;
        fullMoveCounter = p.fullMoveCounter;
        colorToMove = p.colorToMove;
        allPieces = p.allPieces;

        positionStack.pop();
    }
}

// end public methods

// begin private methods

void Position::addPieceAt(Bitboard loc, Color c, Rank r)
{
    if (r == Rank::QUEEN)
    {
        if (c == Color::WHITE)
        {
            whiteQueens = whiteQueens | loc;
        }
        else
        {
            blackQueens = blackQueens | loc;
        }
    }
    else if (r == Rank::ROOK)
    {
        if (c == Color::WHITE)
        {
            whiteRooks = whiteRooks | loc;
        }
        else
        {
            blackRooks = blackRooks | loc;
        }
    }
    else if (r == Rank::BISHOP)
    {
        if (c == Color::WHITE)
        {
            whiteBishops = whiteBishops | loc;
        }
        else
        {
            blackBishops = blackBishops | loc;
        }
    }
    else if (r == Rank::KNIGHT)
    {
        if (c == Color::WHITE)
        {
            whiteKnights = whiteKnights | loc;
        }
        else
        {
            blackKnights = blackKnights | loc;
        }
    }
    else if (r == Rank::PAWN)
    {
        if (c == Color::WHITE)
        {
            whitePawns = whitePawns | loc;
        }
        else
        {
            blackPawns = blackPawns | loc;
        }
    }
    else if (r == Rank::KING)
    {
        if (c == Color::WHITE)
        {
            whiteKing = loc;
        }
        else
        {
            blackKing = loc;
        }
    }

    updatePieces();
}

void Position::computeMoveNotation(MoveList& moves)
{
    std::stringstream san;

    for (int count = 0; count < moves.size(); count++)
    {
        san.str("");
        Move& move = moves[count];
        Rank movingRank = move.getMovingPiece().getRank();

        if (movingRank != Rank::PAWN)
        {
            std::string rank {rankToString(movingRank)};
            rank[0] = std::toupper(rank[0]);

            if (!move.isCastle())
            {
                san << rank;

                bool ambiguity = false;

                if (moves.contains(
                    move,
                    MoveSearchCriteria::TARGET_SQUARE |
                    MoveSearchCriteria::MOVING_PIECE_COLOR |
                    MoveSearchCriteria::MOVING_PIECE_RANK,
                    false) > 1)
                {
                    ambiguity = true;
                }

                if (ambiguity)
                {
                    bool ranksDiffer = true;
                    bool filesDiffer = true;

                    if (moves.contains(
                        move,
                        MoveSearchCriteria::TARGET_SQUARE |
                        MoveSearchCriteria::MOVING_PIECE_COLOR |
                        MoveSearchCriteria::MOVING_PIECE_RANK |
                        MoveSearchCriteria::MOVING_PIECE_SQUARE_RANK,
                        false) > 1)
                    {
                        ranksDiffer = false;
                    }

                    if (moves.contains(
                        move,
                        MoveSearchCriteria::TARGET_SQUARE |
                        MoveSearchCriteria::MOVING_PIECE_COLOR |
                        MoveSearchCriteria::MOVING_PIECE_RANK |
                        MoveSearchCriteria::MOVING_PIECE_SQUARE_FILE,
                        false) > 1)
                    {
                        filesDiffer = false;
                    }

                    if (filesDiffer)
                    {
                        san << move.getMovingPiece().getSquare()
                            .toString()[0];
                    }
                    else if (ranksDiffer)
                    {
                        san << move.getMovingPiece().getSquare()
                            .toString()[1];
                    }
                    else
                    {
                        san << move.getMovingPiece().getSquare()
                            .toString()[0];
                        san << move.getMovingPiece().getSquare()
                            .toString()[1];
                    }
                }

                if (!isSquareEmpty(move.getTargetSquare()))
                {
                    san << "x";
                }

                san << move.getTargetSquare().toString();
            }
            else
            {
                if (move.getTargetSquare().toBitboard()
                    .inMask(0x4000000000000040ULL))
                {
                    san << "0-0";
                }
                else
                {
                    san << "0-0-0";
                }
            }
        }
        else
        {
            bool ep = false;

            if (!isSquareEmpty(move.getTargetSquare()))
            {
                san << move.getMovingPiece().getSquare().toString()[0];
                san << "x";

                if (move.getTargetSquare().toBitboard() == enPassantTarget)
                {
                    ep = true;
                }
            }

            san << move.getTargetSquare().toString();

            if (ep)
            {
                san << "e.p.";
            }

            if (move.getPromotedRank() != Rank::NONE)
            {
                san << "=";

                std::string pr {rankToString(move.getPromotedRank())};
                san <<  std::toupper(pr[0]);
            }
        }

        makeMove(move);

        if (isInCheck(colorToMove))
        {
            san << "+";
        }

        unmakeMove();

        move.setNotation(san.str());
    }
}

void Position::computeSlidingMoves(int index, Piece p, bool highBitBlock,
    const std::array<uint64_t, 64>& rayMask, MoveList& moves)
{
    Bitboard moveMask {0x0000000000000000ULL};
    Bitboard opposingPieces;
    Bitboard ownPieces;
    int blockerIndex;
    bool inCheck {false};

    ownPieces = getOwnPieces(colorToMove);
    inCheck = isInCheck(colorToMove);
    moveMask = rayMask[index];

    blockerIndex = getBlockerIndex(moveMask, highBitBlock);

    if (blockerIndex != -1)
    {
        moveMask = moveMask & (~rayMask[blockerIndex]);
        moveMask = moveMask & (~ownPieces);
    }

    getMovesFromMask(moveMask, p, inCheck, moves);
}

Bitboard Position::computePinningPieceMask(Direction direction) const
{
    Bitboard mask;

    if (colorToMove == Color::BLACK)
    {
        if (direction == Direction::N || direction == Direction::S ||
            direction == Direction::E || direction == Direction::W)
        {
            mask = whiteQueens | whiteRooks;
        }
        else
        {
            mask = whiteQueens | whiteBishops;
        }
    }
    else
    {
        if (direction == Direction::N || direction == Direction::S ||
            direction == Direction::W || direction == Direction::E)
        {
            mask = blackQueens | blackRooks;
        }
        else
        {
            mask = blackQueens | blackBishops;
        }
    }

    return mask;
}

std::vector<Square> Position::findPiece(Color c, Rank r) const
{
    std::vector<Square> locations;
    Bitboard bitboard;
    Bitboard currentLocation;

    if (r == Rank::PAWN)
    {
        bitboard = whitePawns | blackPawns;
    }
    else if (r == Rank::KNIGHT)
    {
        bitboard = whiteKnights | blackKnights;
    }
    else if (r == Rank::BISHOP)
    {
        bitboard = whiteBishops | blackBishops;
    }
    else if (r == Rank::ROOK)
    {
        bitboard = whiteRooks | blackRooks;
    }
    else if (r == Rank::QUEEN)
    {
        bitboard = whiteQueens | blackQueens;
    }
    else if (r == Rank::KING)
    {
        bitboard = whiteKing | blackKing;
    }

    if (c == Color::WHITE)
    {
        bitboard = bitboard & whitePieces;
    }
    else
    {
        bitboard = bitboard & blackPieces;
    }

    while (bitboard != 0x00)
    {
        currentLocation = 0x01ULL << bitboard.lsb();

        locations.push_back(Square(currentLocation));

        bitboard = bitboard & (~currentLocation);
    }

    return locations;
}

void Position::generateCastles(MoveList& moves)
{
    Piece piece;

    if (colorToMove == Color::WHITE)
    {
        piece = getPieceAt(Square(std::pair<int, int>(0, 4)));

        if (piece.isValid() && piece.getRank() == Rank::KING &&
            piece.getColor() == Color::WHITE)
        {
            Move castleKingSide(piece, Square(std::pair<int, int>(0, 6)),
                Rank::NONE);
            Move castleQueenSide(piece, Square(std::pair<int, int>(0, 2)),
                Rank::NONE);

            if (isMoveLegal(castleKingSide))
            {
                moves.addMove(castleKingSide);
            }

            if (isMoveLegal(castleQueenSide))
            {
                moves.addMove(castleQueenSide);
            }
        }
    }
    else
    {
        piece = getPieceAt(Square(std::pair<int, int>(7, 4)));

        if (piece.isValid() && piece.getRank() == Rank::KING &&
            piece.getColor() == Color::BLACK)
        {
            Move castleKingSide(piece, Square(std::pair<int, int>(7, 6)),
                Rank::NONE);
            Move castleQueenSide(piece, Square(std::pair<int, int>(7, 2)),
                Rank::NONE);

            if (isMoveLegal(castleKingSide))
            {
                moves.addMove(castleKingSide);
            }

            if (isMoveLegal(castleQueenSide))
            {
                moves.addMove(castleQueenSide);
            }
        }
    }
}

void Position::generateKingMovesAt(Square s, MoveList& moves)
{
    int locationIndex;
    int currentIndex;
    Piece piece = getPieceAt(s);
    Bitboard location = s.toBitboard();
    Bitboard moveMask = 0x0000000000000000ULL;
    Bitboard currentMove;
    Bitboard ownPieces;

    if (!piece.isValid() || piece.getRank() != Rank::KING ||
        piece.getColor() != colorToMove)
    {
        return;
    }

    ownPieces = getOwnPieces(colorToMove);
    locationIndex = location.lsb();
    moveMask = LookupData::kingAttacks[locationIndex] & (~ownPieces);

    while (moveMask != 0x0000000000000000ULL)
    {
        currentIndex = moveMask.lsb();
        currentMove = 0x01ULL << currentIndex;
        Move m(piece, currentMove, Rank::NONE);

        if (isMoveLegal(m))
        {
            moves.addMove(m);
        }

        moveMask.flipBit(currentIndex);
    }
}

void Position::generateKnightMovesAt(Square s, MoveList& moves)
{
    Piece piece = getPieceAt(s);
    Bitboard location = s.toBitboard();
    int locationIndex;
    Bitboard moveMask = 0x0000000000000000ULL;
    Bitboard currentMove;
    Color color;
    Bitboard ownPieces;
    int currentIndex;
    bool inCheck {false};

    if (
        !piece.isValid() ||
        piece.getRank() != Rank::KNIGHT ||
        isPiecePinned(piece, Direction::N) ||
        isPiecePinned(piece, Direction::E) ||
        isPiecePinned(piece, Direction::S) ||
        isPiecePinned(piece, Direction::W) ||
        isPiecePinned(piece, Direction::SE) ||
        isPiecePinned(piece, Direction::NE) ||
        isPiecePinned(piece, Direction::SW) ||
        isPiecePinned(piece, Direction::NW)
        )
    {
        return;
    }

    color = piece.getColor();

    if (color == Color::WHITE)
    {
        ownPieces = whitePieces;

        if (whiteKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(Square(whiteKing));
        }
    }
    else
    {
        ownPieces = blackPieces;

        if (blackKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(Square(blackKing));
        }
    }

    locationIndex = location.lsb();

    moveMask = LookupData::knightAttacks[locationIndex] & (~ownPieces);

    while (moveMask != 0x0000000000000000ULL)
    {
        currentIndex = moveMask.lsb();
        currentMove = 0x01ULL << currentIndex;
        Move m(piece, Square(currentMove), Rank::NONE);

        if ((inCheck && isMoveLegal(m)) || !inCheck)
        {
            moves.addMove(m);
        }

        moveMask = moveMask.flipBit(currentIndex);
    }
}

void Position::generatePawnAdvancesAt(Bitboard b, MoveList& moves)
{
    Bitboard legalMask;
    Bitboard target;
    Piece movingPiece = getPieceAt(Square(b));
    bool inCheck {false};

    if (!movingPiece.isValid() || movingPiece.getRank() != Rank::PAWN ||
        isPiecePinned(movingPiece, Direction::E) ||
        isPiecePinned(movingPiece, Direction::W) ||
        isPiecePinned(movingPiece, Direction::NW) ||
        isPiecePinned(movingPiece, Direction::SE) ||
        isPiecePinned(movingPiece, Direction::NE) ||
        isPiecePinned(movingPiece, Direction::SW))
    {
        return;
    }

    if (colorToMove == Color::WHITE)
    {
        legalMask = 0x00FFFFFFFFFFFFFFULL;
        target = b << 8;

        if (whiteKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(Square(whiteKing));
        }
    }
    else
    {
        legalMask = 0xFFFFFFFFFFFFFF00ULL;
        target = b >> 8;

        if (blackKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(Square(blackKing));
        }
    }

    if (b.inMask(legalMask) && !target.inMask(allPieces))
    {
        if (target.inMask(0xFF000000000000FFULL))
        {
            Move m1(movingPiece, target, Rank::QUEEN);
            Move m2(movingPiece, target, Rank::ROOK);
            Move m3(movingPiece, target, Rank::BISHOP);
            Move m4(movingPiece, target, Rank::KNIGHT);

            if ((inCheck && isMoveLegal(m1)) || !inCheck)
            {
                moves.addMove(m1);
            }

            if ((inCheck && isMoveLegal(m2)) || !inCheck)
            {
                moves.addMove(m2);
            }

            if ((inCheck && isMoveLegal(m3)) || !inCheck)
            {
                moves.addMove(m3);
            }

            if ((inCheck && isMoveLegal(m4)) || !inCheck)
            {
                moves.addMove(m4);
            }
        }
        else
        {
            Move m1(movingPiece, target, Rank::NONE);

            if ((inCheck && isMoveLegal(m1)) || !inCheck)
            {
                moves.addMove(m1);
            }
        }
    }
}

void Position::generatePawnCapturesAt(Bitboard b, MoveList& moves)
{
    Bitboard validRankMask;
    Bitboard validCaptureLeftMask;
    Bitboard validCaptureRightMask;
    Bitboard captureLeftTarget {0x00ULL};
    Bitboard captureRightTarget {0x00ULL};
    Bitboard targets;
    Bitboard currentSquare;
    Bitboard opposingPieces;
    int currentIndex;
    Piece movePiece = getPieceAt(Square(b));
    bool inCheck {false};

    if (!movePiece.isValid() ||
        movePiece.getRank() != Rank::PAWN ||
        isPiecePinned(movePiece, Direction::N) ||
        isPiecePinned(movePiece, Direction::S) ||
        isPiecePinned(movePiece, Direction::E) ||
        isPiecePinned(movePiece, Direction::W))
    {
        return;
    }

    if (colorToMove == Color::WHITE)
    {
            
        validRankMask.setValue(0x00FFFFFFFFFFFFFFULL);
        validCaptureLeftMask.setValue(0xFEFEFEFEFEFEFEFEULL);
        validCaptureRightMask.setValue(0x7F7F7F7F7F7F7F7FULL);

        if (!isPiecePinned(movePiece, Direction::SW) &&
            !isPiecePinned(movePiece, Direction::NE))
        {
            captureLeftTarget = (
                (b & validRankMask) & validCaptureLeftMask) << 7;
        }

        if (!isPiecePinned(movePiece, Direction::SE) &&
            !isPiecePinned(movePiece, Direction::NW))
        {
            captureRightTarget = (
                (b & validRankMask) & validCaptureRightMask) << 9;
        }

        opposingPieces = blackPieces;

        if (whiteKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(Square(whiteKing));
        }
    }
    else
    {
        validRankMask.setValue(0xFFFFFFFFFFFFFF00ULL);
        validCaptureLeftMask.setValue(0x7F7F7F7F7F7F7F7FULL);
        validCaptureRightMask.setValue(0xFEFEFEFEFEFEFEFEULL);

        if (!isPiecePinned(movePiece, Direction::SW) &&
            !isPiecePinned(movePiece, Direction::NE))
        {
            captureLeftTarget = (
                (b & validRankMask) & validCaptureLeftMask) >> 7;
        }

        if (!isPiecePinned(movePiece, Direction::SE) &&
            !isPiecePinned(movePiece, Direction::NW))
        {
            captureRightTarget = (
                (b & validRankMask) & validCaptureRightMask) >> 9;
        }

        opposingPieces = whitePieces;

        if (blackKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(Square(blackKing));
        }
    }

    targets = (captureLeftTarget | captureRightTarget) &
        (opposingPieces | enPassantTarget);

    while (targets != 0x00ULL)
    {
        currentIndex = targets.lsb();
        currentSquare = 0x01ULL << currentIndex;

        if (currentSquare.inMask(0xFF000000000000FFULL))
        {
            Move m1(movePiece, currentSquare, Rank::QUEEN);
            Move m2(movePiece, currentSquare, Rank::ROOK);
            Move m3(movePiece, currentSquare, Rank::BISHOP);
            Move m4(movePiece, currentSquare, Rank::KNIGHT);

            if ((inCheck && isMoveLegal(m1)) || !inCheck)
            {
                moves.addMove(m1);
            }

            if ((inCheck && isMoveLegal(m2)) || !inCheck)
            {
                moves.addMove(m2);
            }

            if ((inCheck && isMoveLegal(m3)) || !inCheck)
            {
                moves.addMove(m3);
            }

            if ((inCheck && isMoveLegal(m4)) || !inCheck)
            {
                moves.addMove(m4);
            }
        }
        else
        {
            Move m1(movePiece, currentSquare, Rank::NONE);

            if ((inCheck && isMoveLegal(m1)) || !inCheck)
            {
                moves.addMove(m1);
            }
        }

        targets.flipBit(currentIndex);
    }
}

void Position::generatePawnDblAdvancesAt(Bitboard b, MoveList& moves)
{
    Bitboard legalMask;
    Bitboard target;
    Bitboard firstSquare;
    Piece movingPiece = getPieceAt(Square(b));
    bool inCheck {false};

    if (!movingPiece.isValid() || movingPiece.getRank() != Rank::PAWN ||
        isPiecePinned(movingPiece, Direction::E) ||
        isPiecePinned(movingPiece, Direction::W) ||
        isPiecePinned(movingPiece, Direction::SE) ||
        isPiecePinned(movingPiece, Direction::NW) ||
        isPiecePinned(movingPiece, Direction::SW) ||
        isPiecePinned(movingPiece, Direction::NE))
    {
        return;
    }

    if (colorToMove == Color::WHITE)
    {
        legalMask = 0x000000000000FF00ULL;
        target = b << 16;
        firstSquare = b << 8;

        if (whiteKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(Square(whiteKing));
        }
    }
    else
    {
        legalMask = 0x00FF000000000000ULL;
        target = b >> 16;
        firstSquare = b >> 8;

        if (blackKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(Square(blackKing));
        }
    }

    if (b.inMask(legalMask) &&
        !firstSquare.inMask(allPieces) &&
        !target.inMask(allPieces))
    {
        Move m(movingPiece, target, Rank::NONE);

        if ((inCheck && isMoveLegal(m)) || !inCheck)
        moves.addMove(m);
    }
}

void Position::generateSlidingMovesAt(Bitboard b, Direction d,
    MoveList& moves)
{
    Piece piece = getPieceAt({b});

    if (!piece.isValid() ||
        isSlidingPiecePinned(piece, d))
    {
        return;
    }

    return computeSlidingMoves(b.lsb(), piece,
        getHighBitBlockerByDirection(d),
        LookupData::getRayAttacksByDirection(d), moves);
}

int Position::getBlockerIndex(Bitboard mask, bool highBitBlock)
{
    if (highBitBlock)
    {
        return (mask & allPieces).msb();
    }
    else
    {
        return (mask & allPieces).lsb();
    }
}

void Position::getMovesFromMask(Bitboard mask, Piece p,
    bool inCheck, MoveList& moves)
{
    int currentIndex;
    Bitboard currentMove;

    while (mask != 0x0000000000000000ULL)
    {
        currentIndex = mask.lsb();
        currentMove = 0x001ULL << currentIndex;

        Move m {p, currentMove, Rank::NONE};

        if ((inCheck && isMoveLegal(m)) || !inCheck)
        {
            moves.addMove(m);
        }

        mask.flipBit(currentIndex);
    }
}

bool Position::getHighBitBlockerByDirection(Direction direction) const
{
    if (direction == Direction::N ||
        direction == Direction::NW ||
        direction == Direction::NE ||
        direction == Direction::W)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int Position::getOffsetFromDirection(Direction direction) const
{
    int offset = 0;
    
    if (direction == Direction::N || direction == Direction::S)
    {
        offset = 8;
    }
    else if (direction == Direction::E || direction == Direction::W)
    {
        offset = 1;
    }
    else if (direction == Direction::SW || direction == Direction::NE)
    {
        offset = 9;
    }
    else if (direction == Direction::NW || direction == Direction::SE)
    {
        offset = 7;
    }

    return offset;
}

Bitboard Position::getOpposingPieces(Color c) const
{
    if (c == Color::WHITE)
    {
        return blackPieces;
    }
    else
    {
        return whitePieces;
    }
}

Bitboard Position::getOwnPieces(Color c) const
{
    if (c == Color::WHITE)
    {
        return whitePieces;
    }
    else
    {
        return blackPieces;
    }
}

Piece Position::getPieceAt(Square s) const
{
    if (!isSquareEmpty(s))
    {
        if ((whitePawns & s.toBitboard()) != 0x00)
        {
            return Piece(Color::WHITE, Rank::PAWN, s);
        }
        else if ((whiteKnights & s.toBitboard()) != 0x00)
        {
            return Piece(Color::WHITE, Rank::KNIGHT, s);
        }
        else if ((whiteBishops & s.toBitboard()) != 0x00)
        {
            return Piece(Color::WHITE, Rank::BISHOP, s);
        }
        else if ((whiteRooks & s.toBitboard()) != 0x00)
        {
            return Piece(Color::WHITE, Rank::ROOK, s);
        }
        else if ((whiteQueens & s.toBitboard()) != 0x00)
        {
            return Piece(Color::WHITE, Rank::QUEEN, s);
        }
        else if ((whiteKing & s.toBitboard()) != 0x00)
        {
            return Piece(Color::WHITE, Rank::KING, s);
        }
        else if ((blackPawns & s.toBitboard()) != 0x00)
        {
            return Piece(Color::BLACK, Rank::PAWN, s);
        }
        else if ((blackKnights & s.toBitboard()) != 0x00)
        {
            return Piece(Color::BLACK, Rank::KNIGHT, s);
        }
        else if ((blackBishops & s.toBitboard()) != 0x00)
        {
            return Piece(Color::BLACK, Rank::BISHOP, s);
        }
        else if ((blackRooks & s.toBitboard()) != 0x00)
        {
            return Piece(Color::BLACK, Rank::ROOK, s);
        }
        else if ((blackQueens & s.toBitboard()) != 0x00)
        {
            return Piece(Color::BLACK, Rank::QUEEN, s);
        }
        else if ((blackKing & s.toBitboard()) != 0x00)
        {
            return Piece(Color::BLACK, Rank::KING, s);
        }
        else
        {
            return {};
        }
    }
    else
    {
        return {};
    }
}

bool Position::isPiecePinned(const Piece pinnedPiece,
    Direction direction) const
{
    Bitboard location = pinnedPiece.getSquare().toBitboard();
    bool result = false;
    Bitboard pinningPieceMask;
    Bitboard pinnedKingLocation;
    Bitboard tmp;
    Bitboard highMask = 0x00ULL;
    Bitboard lowMask = 0x00ULL;
    int offset;

    if (colorToMove == Color::WHITE)
    {
        pinnedKingLocation = whiteKing;
    }
    else
    {
        pinnedKingLocation = blackKing;
    }

    pinningPieceMask = computePinningPieceMask(direction);

    offset = getOffsetFromDirection(direction);

    highMask = 0x00ULL;
    lowMask = 0x00ULL;
    tmp = location << offset;

    while (tmp != 0x00ULL)
    {
        if (tmp.inMask(pinningPieceMask) ||
            tmp.inMask(pinnedKingLocation))
        {
            highMask = highMask | tmp;
            break;
        }
        else if (tmp.inMask(blackPieces | whitePieces))
        {
            break;
        }

        tmp = tmp << offset;
    }

    tmp = location >> offset;

    while (tmp != 0x00ULL)
    {
        if (tmp.inMask(pinningPieceMask) ||
            tmp.inMask(pinnedKingLocation))
        {
            lowMask = lowMask | tmp;
            break;
        }
        else if (tmp.inMask(blackPieces | whitePieces))
        {
            break;
        }

        tmp = tmp >> offset;
    }

    if (pinnedKingLocation.inMask(lowMask) &&
        !pinnedKingLocation.inMask(highMask) &&
        highMask != 0x00ULL)
    {
        result = true;
    }
    else if (pinnedKingLocation.inMask(highMask) &&
        !pinnedKingLocation.inMask(lowMask) &&
        lowMask != 0x00ULL)
    {
        result = true;
    }

    return result;
}

bool Position::isSlidingPiecePinned(const Piece p, Direction d) const
{
    bool result;

    if (d == Direction::N)
    {
        result = isPiecePinned(p, Direction::E) ||
            isPiecePinned(p, Direction::W) ||
            isPiecePinned(p, Direction::NW) ||
            isPiecePinned(p, Direction::NE) ||
            isPiecePinned(p, Direction::SW) ||
            isPiecePinned(p, Direction::SE); 
    }
    else if (d == Direction::S)
    {
        result = isPiecePinned(p, Direction::E) ||
            isPiecePinned(p, Direction::W) ||
            isPiecePinned(p, Direction::NW) ||
            isPiecePinned(p, Direction::NE) ||
            isPiecePinned(p, Direction::SW) ||
            isPiecePinned(p, Direction::SE); 
    }
    else if (d == Direction::W)
    {
        result = isPiecePinned(p, Direction::S) ||
            isPiecePinned(p, Direction::N) ||
            isPiecePinned(p, Direction::NW) ||
            isPiecePinned(p, Direction::NE) ||
            isPiecePinned(p, Direction::SW) ||
            isPiecePinned(p, Direction::SE); 
    }
    else if (d == Direction::E)
    {
        result = isPiecePinned(p, Direction::S) ||
            isPiecePinned(p, Direction::N) ||
            isPiecePinned(p, Direction::NW) ||
            isPiecePinned(p, Direction::NE) ||
            isPiecePinned(p, Direction::SW) ||
            isPiecePinned(p, Direction::SE); 
    }
    else if (d == Direction::NW)
    {
        result = isPiecePinned(p, Direction::E) ||
            isPiecePinned(p, Direction::W) ||
            isPiecePinned(p, Direction::N) ||
            isPiecePinned(p, Direction::NE) ||
            isPiecePinned(p, Direction::SW) ||
            isPiecePinned(p, Direction::S); 
    }
    else if (d == Direction::NE)
    {
        result = isPiecePinned(p, Direction::E) ||
            isPiecePinned(p, Direction::W) ||
            isPiecePinned(p, Direction::N) ||
            isPiecePinned(p, Direction::NE) ||
            isPiecePinned(p, Direction::SW) ||
            isPiecePinned(p, Direction::S); 
    }
    else if (d == Direction::SW)
    {
        result = isPiecePinned(p, Direction::E) ||
            isPiecePinned(p, Direction::W) ||
            isPiecePinned(p, Direction::NW) ||
            isPiecePinned(p, Direction::N) ||
            isPiecePinned(p, Direction::S) ||
            isPiecePinned(p, Direction::SE); 
    }
    else
    {
        result = isPiecePinned(p, Direction::E) ||
            isPiecePinned(p, Direction::W) ||
            isPiecePinned(p, Direction::NW) ||
            isPiecePinned(p, Direction::N) ||
            isPiecePinned(p, Direction::S) ||
            isPiecePinned(p, Direction::SE); 
    }

    return result;
}

bool Position::isCastleLegal(Square s) const
{
    bool result = false;

    if (s.toBitboard() == 0x0000000000000040ULL)
    {
        result = colorToMove == Color::WHITE &&
            castlingStatus.getWhiteKingSide() &&
            isSquareEmpty(Square {"f1"}) &&
            isSquareEmpty(Square {"g1"}) &&
            !isSquareAttacked(Square {"e1"}) &&
            !isSquareAttacked(Square {"f1"}) &&
            !isSquareAttacked(Square {"g1"}); 
    }
    else if (s.toBitboard() == 0x0000000000000004ULL)
    {
        result = colorToMove == Color::WHITE &&
            castlingStatus.getWhiteQueenSide() &&
            isSquareEmpty(Square {"d1"}) &&
            isSquareEmpty(Square {"c1"}) &&
            isSquareEmpty(Square {"b1"}) &&
            !isSquareAttacked(Square {"e1"}) &&
            !isSquareAttacked(Square {"d1"}) &&
            !isSquareAttacked(Square {"c1"}) &&
            !isSquareAttacked(Square {"b1"}); 
    }
    else if (s.toBitboard() == 0x4000000000000000ULL)
    {
        result = colorToMove == Color::BLACK &&
            castlingStatus.getBlackKingSide() &&
            isSquareEmpty(Square {"f8"}) &&
            isSquareEmpty(Square {"g8"}) &&
            !isSquareAttacked(Square {"e8"}) &&
            !isSquareAttacked(Square {"f8"}) &&
            !isSquareAttacked(Square {"g8"});
    }
    else if (s.toBitboard() == 0x0400000000000000ULL)
    {
        result = colorToMove == Color::BLACK &&
            castlingStatus.getBlackQueenSide() &&
            isSquareEmpty(Square {"d8"}) &&
            isSquareEmpty(Square {"c8"}) &&
            isSquareEmpty(Square {"b8"}) &&
            !isSquareAttacked(Square {"e8"}) &&
            !isSquareAttacked(Square {"d8"}) &&
            !isSquareAttacked(Square {"c8"}) &&
            !isSquareAttacked(Square {"b8"}); 
    }

    return result;
}

bool Position::isInCheck(Color c) const
{
    bool inCheck {false};

    if (c == Color::WHITE)
    {
        if (whiteKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(Square(whiteKing));
        }
    }
    else
    {
        if (blackKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(Square(blackKing));
        }
    }

    return inCheck;
}

bool Position::isMoveLegal(const Move& m)
{
    Piece p = m.getMovingPiece();
    std::vector<Square> kingLocationVector =
        findPiece(p.getColor(), Rank::KING);

    bool result = false;

    if (kingLocationVector.empty())
    {
        return false;
    }

    if (colorToMove != m.getMovingPiece().getColor())
    {
        return false;
    }

    Square kingLocation = kingLocationVector[0];

    if (!isSquareAttacked(kingLocation))
    {
        if (p.getRank() != Rank::KING)
        {
            result = true;
        }
        else
        {
            if (m.isCastle())
            {
                result = isCastleLegal(m.getTargetSquare());
            }
            else if (!isSquareAttacked(m.getTargetSquare()))
            {
                result = true;
            }
        }
    }
    else
    {
        makeMove(m);
        makeMove({Piece {}, Square {}, Rank::NONE});

        if (p.getRank() != Rank::KING)
        {
            if (!isSquareAttacked(kingLocation))
            {
                result = true;
            }
        }
        else
        {
            if (!isSquareAttacked(m.getTargetSquare()))
            {
                result = true;
            }
        }

        unmakeMove();
        unmakeMove();
    }

    return result;
}

bool Position::isSquareAttacked(Square s) const        
{
    bool result = false;
    int squareIndex = s.toBitboard().lsb();
    Bitboard square = s.toBitboard();
    int blockerIndex;
    Bitboard opposingKnights =
        colorToMove == Color::WHITE ? blackKnights : whiteKnights;
    Bitboard opposingBishops =
        colorToMove == Color::WHITE ? blackBishops : whiteBishops;
    Bitboard opposingRooks =
        colorToMove == Color::WHITE ? blackRooks : whiteBishops;
    Bitboard opposingQueens =
        colorToMove == Color::WHITE ? blackQueens : whiteKnights;

    if ((LookupData::knightAttacks[squareIndex] & opposingKnights) !=
        0x0000000000000000ULL)
    {
        result = true;
    }

    blockerIndex = (LookupData::rayAttacksN[squareIndex] & allPieces)
        .lsb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksNE[squareIndex] &
        allPieces).lsb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksE[squareIndex] &
        (allPieces)).lsb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksSE[squareIndex] &
        (allPieces)).msb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksS[squareIndex] &
        (allPieces)).msb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksSW[squareIndex] &
        (allPieces)).msb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksW[squareIndex] &
        (allPieces)).msb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksNW[squareIndex] &
        (allPieces)).lsb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    if (colorToMove == Color::WHITE)
    {
        if (((((square & 0x00FEFEFEFEFEFEFEULL) << 7) & blackPawns)
            != 0x0000000000000000ULL) ||
            ((((square & 0x007F7F7F7F7F7F7FULL) << 9) & blackPawns)
            != 0x0000000000000000ULL))
        {
            result = true;
        }

        if ((LookupData::kingAttacks[squareIndex] & blackKing)
            != 0x0000000000000000ULL)
        {
            result = true;
        }
    }
    else
    {
        if (((((square & 0xFEFEFEFEFEFEFE00ULL) >> 9) & whitePawns)
            != 0x0000000000000000ULL) ||
            ((((square & 0x7F7F7F7F7F7F7F00ULL) >> 7) & whitePawns)
            != 0x0000000000000000ULL))
        {
            result = true;
        }

        if ((LookupData::kingAttacks[squareIndex] & whiteKing) !=
            0x0000000000000000ULL)
        {
            result = true;
        }
    }

    return result;
}

bool Position::isSquareEmpty(Square s) const
{
    return (allPieces & s.toBitboard()).isEmpty();
}

void Position::removePieceAt(Bitboard loc)
{
    whitePawns = whitePawns & (~loc);
    whiteKnights = whiteKnights & (~loc);
    whiteBishops = whiteBishops & (~loc);
    whiteRooks = whiteRooks & (~loc);
    whiteQueens = whiteQueens & (~loc);
    whiteKing = whiteKing & (~loc);

    blackPawns = blackPawns & (~loc);
    blackKnights = blackKnights & (~loc);
    blackBishops = blackBishops & (~loc);
    blackRooks = blackRooks & (~loc);
    blackQueens = blackQueens & (~loc);
    blackKing = blackKing & (~loc);

    updatePieces();
}

void Position::parseFEN(std::string fen)
{
    std::vector<std::string> fenParts;
    std::vector<std::string> rankInfo;
    Bitboard currentSquare = 0x0000000000000000ULL;
    std::string currentCharacter;
    Color currentColor {Color::NONE};
    Rank currentRank {Rank::NONE};

    whitePawns = 0x0000000000000000ULL;
    whiteKnights = 0x0000000000000000ULL;
    whiteBishops = 0x0000000000000000ULL;
    whiteRooks = 0x0000000000000000ULL;
    whiteQueens = 0x0000000000000000ULL;
    whiteKing = 0x0000000000000000ULL;
    whitePieces = 0x0000000000000000ULL;
    blackPawns = 0x0000000000000000ULL;
    blackKnights = 0x0000000000000000ULL;
    blackBishops = 0x0000000000000000ULL;
    blackRooks = 0x0000000000000000ULL;
    blackQueens = 0x0000000000000000ULL;
    blackKing = 0x0000000000000000ULL;
    blackPieces = 0x0000000000000000ULL;

    Utility::split(fen, ' ', fenParts);

    if (fenParts.size() != 6)
    {
        return;
    }

    Utility::split(fenParts[0], '/', rankInfo);

    if (rankInfo.size() != 8)
    {
        return;
    }

    for (int i = 0; i < 8; i++)
    {
        currentSquare = 0x0000000000000001ULL << (8 * (7 - i));

        for (size_t j = 0; j < rankInfo[i].length(); j++)
        {
            currentCharacter = rankInfo[i].substr(j, 1);
            if (currentCharacter[0] > 47 && currentCharacter[0] < 58)
            {
                currentSquare = currentSquare <<
                    (currentCharacter[0] - 48);
            }
            else
            {
                currentColor = getColorFromString(currentCharacter);
                currentRank = getRankFromString(currentCharacter);

                if (currentColor != Color::NONE &&
                    currentRank != Rank::NONE)
                {
                    addPieceAt(currentSquare, currentColor, currentRank);
                }
                        
                currentSquare = currentSquare << 1;
            }
        }
    }

    if (fenParts[1] == "b")
    {
        colorToMove = Color::BLACK;
    }
    else if (fenParts[1] == "w")
    {
        colorToMove = Color::WHITE;
    }

    if (fenParts[2].find("K") != std::string::npos)
    {
        castlingStatus.setWhiteKingSide(true);
    }

    if (fenParts[2].find("Q") != std::string::npos)
    {
        castlingStatus.setWhiteQueenSide(true);
    }

    if (fenParts[2].find("k") != std::string::npos)
    {
        castlingStatus.setBlackKingSide(true);
    }

    if (fenParts[2].find("q") != std::string::npos)
    {
        castlingStatus.setBlackQueenSide(true);
    }

    if (fenParts[3] == "-")
    {
        enPassantTarget = 0x00ULL;
    }
    else
    {
        Bitboard epLoc = Square(fenParts[3]).toBitboard();
        
        if (epLoc != 0x00)
        {
            enPassantTarget = epLoc;
        }
        else
        {
            enPassantTarget = 0x00ULL;
        }
    }

    halfMoveCounter = std::stoi(fenParts[4]);
    fullMoveCounter = std::stoi(fenParts[5]);
}

void Position::updatePieces()
{
    whitePieces = (whitePawns
        | whiteKnights
        | whiteBishops
        | whiteRooks
        | whiteQueens
        | whiteKing);

    blackPieces = (blackPawns
        | blackKnights
        | blackBishops
        | blackRooks
        | blackQueens
        | blackKing);

    allPieces = whitePieces | blackPieces;
}
