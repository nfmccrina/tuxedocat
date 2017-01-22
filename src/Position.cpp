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

#include "Position.hpp"
#include "LookupData.hpp"
#include "Utility.hpp"
#include "MoveSearchCriteria.hpp"
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <cctype>

using namespace TuxedoCat;

// begin constructors

Position::Position(std::string fen)
    : castlingStatus(CastlingStatus(false, false, false, false)),
        currentPinnedPiecesN(0x00ULL),
        currentPinnedPiecesE(0x00ULL),
        currentPinnedPiecesW(0x00ULL),
        currentPinnedPiecesS(0x00ULL),
        currentPinnedPiecesNE(0x00ULL),
        currentPinnedPiecesNW(0x00ULL),
        currentPinnedPiecesSE(0x00ULL),
        currentPinnedPiecesSW(0x00ULL),
        currentPinnedPieces(0x00ULL),
        inCheck(false)
{
    parseFEN(fen);

    updatePieces();
}

Position::Position(const Position& p)
    : castlingStatus(p.castlingStatus),
        currentPinnedPiecesN(0x00ULL),
        currentPinnedPiecesE(0x00ULL),
        currentPinnedPiecesW(0x00ULL),
        currentPinnedPiecesS(0x00ULL),
        currentPinnedPiecesNE(0x00ULL),
        currentPinnedPiecesNW(0x00ULL),
        currentPinnedPiecesSE(0x00ULL),
        currentPinnedPiecesSW(0x00ULL),
        currentPinnedPieces(0x00ULL),
        inCheck(false)
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
    currentPinnedPieces = p.currentPinnedPieces;
    currentPinnedPiecesN = p.currentPinnedPiecesN;
    currentPinnedPiecesS = p.currentPinnedPiecesS;
    currentPinnedPiecesE = p.currentPinnedPiecesE;
    currentPinnedPiecesW = p.currentPinnedPiecesW;
    currentPinnedPiecesNE = p.currentPinnedPiecesNE;
    currentPinnedPiecesNW = p.currentPinnedPiecesNW;
    currentPinnedPiecesSE = p.currentPinnedPiecesSE;
    currentPinnedPiecesSW = p.currentPinnedPiecesSW;
    inCheck = p.inCheck;
}

// end constructors

// begin public methods

void Position::generateMoves(Rank rank, MoveList& moves, bool calculateNotation)
{
    uint64_t pieces;
    uint64_t currentSquare;
    int currentIndex;

    if (colorToMove == Color::WHITE)
    {
        pieces = whitePieces;
    }
    else
    {
        pieces = blackPieces;
    }

    calculateInCheck();
    calculatePinnedPieces();

    if (rank == Rank::NONE || rank == Rank::KING)
    {
        generateCastles(moves);
    }

    if (rank == Rank::NONE || rank == Rank::PAWN)
    {
        generatePawnCaptures(moves);
        generatePawnAdvances(moves);
    }

    if (rank == Rank::NONE || rank == Rank::KNIGHT)
    {
        generateKnightMoves(moves);
    }

    pieces = pieces & (~(whitePawns | blackPawns)) &
        (~(whiteKnights | blackKnights));

    while (pieces != 0x00ULL)
    {
        currentIndex = Utility::lsb(pieces);
        currentSquare = 0x01ULL << currentIndex;
        Piece currentPiece {getPieceAt(currentSquare)};

        if (!currentPiece.isValid() || currentPiece.color != colorToMove)
        {
            Utility::flipBit(pieces, currentIndex);
            continue;
        }

        if (rank == Rank::NONE || rank == Rank::KING)
        {
            generateKingMovesAt(currentSquare, moves);
        }

        if ((rank == Rank::NONE || rank == Rank::BISHOP) &&
            currentPiece.rank == Rank::BISHOP)
        {
            if ((currentSquare & currentPinnedPieces) == 0x00ULL)
            {
                generateSlidingMovesAt(currentPiece,
                    Direction::NE, moves);
                
                generateSlidingMovesAt(currentPiece,
                    Direction::NW, moves);

                generateSlidingMovesAt(currentPiece,
                    Direction::SE, moves);

                generateSlidingMovesAt(currentPiece,
                    Direction::SW, moves);
            }
            else
            {
                if (!isSlidingPiecePinned(currentPiece, Direction::NE))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::NE, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::NW))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::NW, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::SW))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::SW, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::SE))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::SE, moves);
                }
            }
        }

        if ((rank == Rank::NONE || rank == Rank::ROOK) &&
            currentPiece.rank == Rank::ROOK)
        {
            if ((currentSquare & currentPinnedPieces) == 0x00ULL)
            {
                generateSlidingMovesAt(currentPiece,
                    Direction::N, moves);
                
                generateSlidingMovesAt(currentPiece,
                    Direction::W, moves);

                generateSlidingMovesAt(currentPiece,
                    Direction::E, moves);

                generateSlidingMovesAt(currentPiece,
                    Direction::S, moves);
            }
            else
            {
                if (!isSlidingPiecePinned(currentPiece, Direction::N))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::N, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::S))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::S, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::W))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::W, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::E))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::E, moves);
                }
            }
        }

        if ((rank == Rank::NONE || rank == Rank::QUEEN) &&
            currentPiece.rank == Rank::QUEEN)
        {
            if ((currentSquare & currentPinnedPieces) == 0x00ULL)
            {
                generateSlidingMovesAt(currentPiece,
                    Direction::N, moves);
                
                generateSlidingMovesAt(currentPiece,
                    Direction::W, moves);

                generateSlidingMovesAt(currentPiece,
                    Direction::E, moves);

                generateSlidingMovesAt(currentPiece,
                    Direction::S, moves);

                generateSlidingMovesAt(currentPiece,
                    Direction::NE, moves);
                
                generateSlidingMovesAt(currentPiece,
                    Direction::NW, moves);

                generateSlidingMovesAt(currentPiece,
                    Direction::SE, moves);

                generateSlidingMovesAt(currentPiece,
                    Direction::SW, moves);
            }
            else
            {
                if (!isSlidingPiecePinned(currentPiece, Direction::N))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::N, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::S))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::S, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::W))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::W, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::E))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::E, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::NE))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::NE, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::NW))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::NW, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::SW))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::SW, moves);
                }

                if (!isSlidingPiecePinned(currentPiece, Direction::SE))
                {
                    generateSlidingMovesAt(currentPiece,
                        Direction::SE, moves);
                }
            }
        }

        Utility::flipBit(pieces, currentIndex);
    }

    if (calculateNotation)
    {
        computeMoveNotation(moves);
    }
}

Move Position::getMoveFromString(std::string s) const
{
    if (s.size() < 4)
    {
        return Move({}, 0x00ULL, Rank::NONE);
    }

    uint64_t source {Utility::algebraicToBitboard(s.substr(0, 2))};
    uint64_t target {Utility::algebraicToBitboard(s.substr(2, 2))};

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
    uint64_t sourceLocation {move.movingPiece.square};
    uint64_t targetLocation {move.targetSquare};
    uint64_t tmpEnPassant;
    Color sourceColor {move.movingPiece.color};
    Rank sourceRank {move.movingPiece.rank};
    Rank promotedRank {move.promotedRank};
    uint64_t capturedPieceLocation;

    if (sourceRank == Rank::PAWN && targetLocation == enPassantTarget)
    {
        if (sourceColor == Color::WHITE)
        {
            capturedPieceLocation = targetLocation >> 8;
        }
        else
        {
            capturedPieceLocation = targetLocation << 8;
        }
    }
    else
    {
        capturedPieceLocation = targetLocation;
    }

    Piece capturedPiece {getPieceAt(capturedPieceLocation)};

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

    if (enPassantTarget != 0x00ULL)
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
                if (enPassantTarget != 0x00ULL &&
                    targetLocation == enPassantTarget)
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
            else
            {
                halfMoveCounter++;
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

    if (enPassantTarget != 0x00ULL)
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
    std::stringstream ss;
    bool castlesAvailable {false};

    for (int row = 7; row >= 0; row--)
    {
        ss << "  +---+---+---+---+---+---+---+---+" << std::endl;
        ss << static_cast<char>(row + 49) << " ";

        for (int col = 0; col < 8; col++)
        {
            ss << "| ";

            Piece currentPiece {
                getPieceAt(Utility::coordinatesToBitboard(
                    std::pair<int, int>(row, col)))};

            if (currentPiece.isValid())
            {
                ss << currentPiece.toString();
            }
            else
            {
                ss << " ";
            }

            ss << " ";
        }

        ss << "|" << std::endl;
    }
        
    ss << "  +---+---+---+---+---+---+---+---+" << std::endl;
    ss << "    a   b   c   d   e   f   g   h  " << std::endl;

    ss << "To Move: " << colorToString(colorToMove) << std::endl;    
    ss << "Castling availability: ";

    if (castlingStatus.getWhiteKingSide())
    {
        ss << "K";
        castlesAvailable = true;
    }

    if (castlingStatus.getWhiteQueenSide())
    {
        ss << "Q";
        castlesAvailable = true;
    }

    if (castlingStatus.getBlackKingSide())
    {
        ss << "k";
        castlesAvailable = true;
    }

    if (castlingStatus.getBlackQueenSide())
    {
        ss << "q";
        castlesAvailable = true;
    }

    if (!castlesAvailable)
    {
        ss << "N/A";
    }

    ss << std::endl;
    ss << "En passant: ";

    if (enPassantTarget != 0x00ULL)
    {
        ss << Utility::toAlgebraicCoordinate(enPassantTarget);
    }
    else
    {
        ss << "N/A";
    }

    ss << std::endl;
    ss << "Half move counter: " << halfMoveCounter;
    ss << std::endl;
    ss << "Full move counter: " << fullMoveCounter;

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
        currentPinnedPieces = p.currentPinnedPieces;
        currentPinnedPiecesN = p.currentPinnedPiecesN;
        currentPinnedPiecesS = p.currentPinnedPiecesS;
        currentPinnedPiecesE = p.currentPinnedPiecesE;
        currentPinnedPiecesW = p.currentPinnedPiecesW;
        currentPinnedPiecesNE = p.currentPinnedPiecesNE;
        currentPinnedPiecesNW = p.currentPinnedPiecesNW;
        currentPinnedPiecesSE = p.currentPinnedPiecesSE;
        currentPinnedPiecesSW = p.currentPinnedPiecesSW;
        inCheck = p.inCheck;

        positionStack.pop();
    }
}

// end public methods

// begin private methods

void Position::addPieceAt(uint64_t loc, Color c, Rank r)
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

void Position::calculateInCheck()
{
    if (colorToMove == Color::WHITE)
    {
        if (whiteKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(whiteKing);
        }
    }
    else
    {
        if (blackKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(blackKing);
        }
    }
}

void Position::calculatePinnedPieces()
{
    uint64_t ownPieces {colorToMove == Color::WHITE ? whitePieces :
        blackPieces};
    int kingIndex {Utility::lsb(colorToMove == Color::WHITE ? whiteKing :
        blackKing)};

    currentPinnedPiecesN = 0x00ULL;
    currentPinnedPiecesNE = 0x00ULL;
    currentPinnedPiecesNW = 0x00ULL;
    currentPinnedPiecesW = 0x00ULL;
    currentPinnedPiecesE = 0x00ULL;
    currentPinnedPiecesSW = 0x00ULL;
    currentPinnedPiecesSE = 0x00ULL;
    currentPinnedPiecesS = 0x00ULL;

    currentPinnedPiecesN |= getPinnedPieceByDirection(kingIndex, ownPieces,
        Direction::N);

    currentPinnedPiecesNE |= getPinnedPieceByDirection(kingIndex, ownPieces,
        Direction::NE);

    currentPinnedPiecesNW |= getPinnedPieceByDirection(kingIndex, ownPieces,
        Direction::NW);

    currentPinnedPiecesW |= getPinnedPieceByDirection(kingIndex, ownPieces,
        Direction::W);

    currentPinnedPiecesE |= getPinnedPieceByDirection(kingIndex, ownPieces,
        Direction::E);

    currentPinnedPiecesSW |= getPinnedPieceByDirection(kingIndex, ownPieces,
        Direction::SW);

    currentPinnedPiecesSE |= getPinnedPieceByDirection(kingIndex, ownPieces,
        Direction::SE);

    currentPinnedPiecesS |= getPinnedPieceByDirection(kingIndex, ownPieces,
        Direction::S);

    currentPinnedPieces = currentPinnedPiecesN |
        currentPinnedPiecesS |
        currentPinnedPiecesW |
        currentPinnedPiecesE |
        currentPinnedPiecesSE |
        currentPinnedPiecesSW |
        currentPinnedPiecesNE |
        currentPinnedPiecesNW;
}

void Position::computeMoveNotation(MoveList& moves)
{
    std::stringstream san;

    for (int count = 0; count < moves.size(); count++)
    {
        san.str("");
        Move& move = moves[count];
        Rank movingRank = move.movingPiece.rank;

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
                        san << Utility::toAlgebraicCoordinate(
                            move.movingPiece.square)[0];
                    }
                    else if (ranksDiffer)
                    {
                        san << Utility::toAlgebraicCoordinate(
                            move.movingPiece.square)[1];
                    }
                    else
                    {
                        san << Utility::toAlgebraicCoordinate(
                            move.movingPiece.square)[0];
                        san << Utility::toAlgebraicCoordinate(
                            move.movingPiece.square)[1];
                    }
                }

                if ((move.targetSquare & allPieces) != 0x00ULL)
                {
                    san << "x";
                }

                san << Utility::toAlgebraicCoordinate(
                    move.targetSquare);
            }
            else
            {
                if (Utility::inMask(move.targetSquare,
                    0x4000000000000040ULL))
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

            if ((move.targetSquare & allPieces) != 0x00ULL)
            {
                san << Utility::toAlgebraicCoordinate(
                    move.movingPiece.square)[0];
                san << "x";

                if (move.targetSquare == enPassantTarget)
                {
                    ep = true;
                }
            }

            san << Utility::toAlgebraicCoordinate(
                move.targetSquare);

            if (ep)
            {
                san << "e.p.";
            }

            if (move.promotedRank != Rank::NONE)
            {
                san << "=";

                std::string pr {rankToString(move.promotedRank)};
                san <<  std::toupper(pr[0]);
            }
        }

        makeMove(move);

        if (inCheck)
        {
            san << "+";
        }

        unmakeMove();

        move.notation = san.str();
    }
}

void Position::computeSlidingMoves(int index, Piece p, bool highBitBlock,
    const std::array<uint64_t, 64>& rayMask, MoveList& moves)
{
    uint64_t moveMask {0x0000000000000000ULL};
    uint64_t ownPieces;
    int blockerIndex;

    ownPieces = getOwnPieces(colorToMove);
    moveMask = rayMask[index];

    blockerIndex = getBlockerIndex(moveMask, highBitBlock);

    if (blockerIndex != -1)
    {
        moveMask = moveMask & (~rayMask[blockerIndex]);
        moveMask = moveMask & (~ownPieces);
    }

    getMovesFromMask(moveMask, p, moves);
}

void Position::generateCastles(MoveList& moves)
{
    if (colorToMove == Color::WHITE)
    {
        Piece piece {getPieceAt(0x0000000000000010ULL)};

        if (piece.isValid() && piece.rank == Rank::KING &&
            piece.color == Color::WHITE)
        {
            Move castleKingSide(piece, 0x0000000000000040ULL,
                Rank::NONE);
            Move castleQueenSide(piece, 0x0000000000000004ULL,
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
        Piece piece {getPieceAt(0x1000000000000000ULL)};

        if (piece.isValid() && piece.rank == Rank::KING &&
            piece.color == Color::BLACK)
        {
            Move castleKingSide(piece, 0x4000000000000000ULL,
                Rank::NONE);
            Move castleQueenSide(piece, 0x0400000000000000ULL,
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

void Position::generateKingMovesAt(uint64_t s, MoveList& moves)
{
    int locationIndex;
    int currentIndex;
    Piece piece = getPieceAt(s);
    uint64_t location = s;
    uint64_t moveMask = 0x0000000000000000ULL;
    uint64_t currentMove;
    uint64_t ownPieces;

    if (!piece.isValid() || piece.rank != Rank::KING ||
        piece.color != colorToMove)
    {
        return;
    }

    ownPieces = getOwnPieces(colorToMove);
    locationIndex = Utility::lsb(location);
    moveMask = LookupData::kingAttacks[locationIndex] & (~ownPieces);

    while (moveMask != 0x0000000000000000ULL)
    {
        currentIndex = Utility::lsb(moveMask);
        currentMove = 0x01ULL << currentIndex;
        Move m(piece, currentMove, Rank::NONE);

        if (isMoveLegal(m))
        {
            moves.addMove(m);
        }

        Utility::flipBit(moveMask, currentIndex);
    }
}

void Position::generateKnightMoves(MoveList& moves)
{
    int currentIndex {0x00ULL};
    uint64_t currentSquare {0x00ULL};
    int currentTargetIndex {0x00ULL};
    uint64_t currentTargetSquare {0x00ULL};
    uint64_t ownKnights {colorToMove == Color::WHITE ? whiteKnights :
        blackKnights};
    uint64_t ownPieces {colorToMove == Color::WHITE ? whitePieces :
        blackPieces};
    uint64_t targets {0x00ULL};
    uint64_t unpinnedKnights {ownKnights & (~currentPinnedPieces)};

    while (unpinnedKnights != 0x00ULL)
    {
        currentIndex = Utility::lsb(unpinnedKnights);
        currentSquare = 0x01ULL << currentIndex;

        targets = LookupData::knightAttacks[currentIndex] & (~ownPieces);

        while (targets != 0x00ULL)
        {
            currentTargetIndex = Utility::lsb(targets);
            currentTargetSquare = 0x01ULL << currentTargetIndex;

            Move m {getPieceAt(currentSquare), currentTargetSquare, Rank::NONE};

            if ((inCheck && isMoveLegal(m)) || !inCheck)
            {
                moves.addMove(m);
            }

            Utility::flipBit(targets, currentTargetIndex);
        }

        Utility::flipBit(unpinnedKnights, currentIndex);
    }
}

void Position::generatePawnAdvances(MoveList& moves)
{
    uint64_t validSingleMask {0x00FFFFFFFFFFFF00ULL};
    uint64_t validDoubleMask {0x00ULL};
    uint64_t singleTargets {0x00ULL};
    uint64_t doubleTargets {0x00ULL};
    int currentIndex {0x00ULL};
    uint64_t currentSquare {0x00ULL};
    uint64_t currentSourceSquare {0x00ULL};
    uint64_t targets {0x00ULL};

    if (colorToMove == Color::WHITE)
    {
        validDoubleMask = 0x000000000000FF00ULL;
        singleTargets = ((whitePawns & validSingleMask) << 8) & (~allPieces);
        doubleTargets = (((((whitePawns & validDoubleMask) << 8) &
            (~allPieces))) << 8) & (~allPieces);
    }
    else
    {
        validDoubleMask = 0x00FF000000000000ULL;
        singleTargets = ((blackPawns & validSingleMask) >> 8) & (~allPieces);
        doubleTargets = (((((blackPawns & validDoubleMask) >> 8) &
            (~allPieces))) >> 8) & (~allPieces);
    }

    targets = singleTargets | doubleTargets;

    while (targets != 0x00ULL)
    {
        currentIndex = Utility::lsb(targets);
        currentSquare = 0x01ULL << currentIndex;
        currentSourceSquare = 0x00ULL;

        if (colorToMove == Color::WHITE)
        {
            if ((currentSquare & singleTargets) != 0x00ULL)
            {
                currentSourceSquare = currentSquare >> 8;
            }
            else if ((currentSquare & doubleTargets) != 0x00ULL)
            {
                currentSourceSquare = currentSquare >> 16;
            }
        }
        else
        {
            if ((currentSquare & singleTargets) != 0x00ULL)
            {
                currentSourceSquare = currentSquare << 8;
            }
            else if ((currentSquare & doubleTargets) != 0x00ULL)
            {
                currentSourceSquare = currentSquare << 16;
            }
        }

        Piece currentPiece {getPieceAt(currentSourceSquare)};

        if (isSlidingPiecePinned(currentPiece, Direction::N))
        {
            Utility::flipBit(targets, currentIndex);
            continue;
        }

        if ((currentSquare & 0xFF000000000000FFULL) != 0x00ULL)
        {
            Move m1 {currentPiece, currentSquare, Rank::QUEEN};
            Move m2 {currentPiece, currentSquare, Rank::ROOK};
            Move m3 {currentPiece, currentSquare, Rank::BISHOP};
            Move m4 {currentPiece, currentSquare, Rank::KNIGHT};

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
            Move m1(currentPiece, currentSquare, Rank::NONE);

            if ((inCheck && isMoveLegal(m1)) || !inCheck)
            {
                moves.addMove(m1);
            }
        }
        
        Utility::flipBit(targets, currentIndex);
    }
}

void Position::generatePawnCaptures(MoveList& moves)
{
    uint64_t validLeftMask {0x00ULL};
    uint64_t validRightMask {0x00ULL};
    uint64_t leftTargets {0x00ULL};
    uint64_t rightTargets {0x00ULL};
    int currentIndex {0x00ULL};
    uint64_t currentSquare {0x00ULL};
    int currentSourceIndex {0};
    uint64_t currentSourceSquare {0x00ULL};
    uint64_t targets {0x00ULL};
    uint64_t sources;

    if (colorToMove == Color::WHITE)
    {
        validLeftMask = 0x00FEFEFEFEFEFE00ULL;
        validRightMask = 0x007F7F7F7F7F7F00ULL;
        leftTargets = ((whitePawns & validLeftMask) << 7) & (blackPieces | enPassantTarget);
        rightTargets = ((whitePawns & validRightMask) << 9) & (blackPieces | enPassantTarget);
    }
    else
    {
        validLeftMask = 0x007F7F7F7F7F7F00ULL;
        validRightMask = 0x00FEFEFEFEFEFE00ULL;
        leftTargets = ((blackPawns & validLeftMask) >> 7) & (whitePieces | enPassantTarget);
        rightTargets = ((blackPawns & validRightMask) >> 9) & (whitePieces | enPassantTarget);
    }

    targets = leftTargets | rightTargets;

    while (targets != 0x00ULL)
    {
        currentIndex = Utility::lsb(targets);
        currentSquare = 0x01ULL << currentIndex;

        sources = 0x00ULL;

        if (colorToMove == Color::WHITE)
        {
            if ((currentSquare & leftTargets) != 0x00ULL)
            {
                sources |= currentSquare >> 7;
            }
            
            if ((currentSquare & rightTargets) != 0x00ULL)
            {
                sources |= currentSquare >> 9;
            }
        }
        else
        {
            if ((currentSquare & leftTargets) != 0x00ULL)
            {
                sources |= currentSquare << 7;
            }

            if ((currentSquare & rightTargets) != 0x00ULL)
            {
                sources |= currentSquare << 9;
            }
        }

        while (sources != 0x00ULL)
        {
            currentSourceIndex = Utility::lsb(sources);
            currentSourceSquare = 0x01ULL << currentSourceIndex;

            Piece currentPiece {getPieceAt(currentSourceSquare)};

            if ((currentSquare == currentSourceSquare << 7 ||
                currentSquare == currentSourceSquare >> 7) &&
                isSlidingPiecePinned(currentPiece, Direction::NW))
            {
                Utility::flipBit(sources, currentSourceIndex);
                continue;
            }

            if ((currentSquare == currentSourceSquare << 9 ||
                currentSquare == currentSourceSquare >> 9) &&
                isSlidingPiecePinned(currentPiece, Direction::NE))
            {
                Utility::flipBit(sources, currentSourceIndex);
                continue;
            }

            if ((currentSquare & 0xFF000000000000FFULL) != 0x00ULL)
            {
                Move m1 {currentPiece, currentSquare, Rank::QUEEN};
                Move m2 {currentPiece, currentSquare, Rank::ROOK};
                Move m3 {currentPiece, currentSquare, Rank::BISHOP};
                Move m4 {currentPiece, currentSquare, Rank::KNIGHT};

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
                Move m1(currentPiece, currentSquare, Rank::NONE);

                if ((inCheck && isMoveLegal(m1)) || !inCheck)
                {
                    moves.addMove(m1);
                }
            }
            
            Utility::flipBit(sources, currentSourceIndex);
        }
        
        Utility::flipBit(targets, currentIndex);
    }
}

void Position::generateSlidingMovesAt(Piece p, Direction d,
    MoveList& moves)
{
    return computeSlidingMoves(Utility::lsb(p.square), p,
        getHighBitBlockerByDirection(d),
        LookupData::getRayAttacksByDirection(d), moves);
}

int Position::getBlockerIndex(uint64_t mask, bool highBitBlock)
{
    if (highBitBlock)
    {
        return Utility::msb(mask & allPieces);
    }
    else
    {
        return Utility::lsb(mask & allPieces);
    }
}

void Position::getMovesFromMask(uint64_t mask, Piece p, MoveList& moves)
{
    int currentIndex;
    uint64_t currentMove;

    while (mask != 0x0000000000000000ULL)
    {
        currentIndex = Utility::lsb(mask);
        currentMove = 0x001ULL << currentIndex;

        Move m {p, currentMove, Rank::NONE};

        if ((inCheck && isMoveLegal(m)) || !inCheck)
        {
            moves.addMove(m);
        }

        Utility::flipBit(mask, currentIndex);
    }
}

bool Position::getHighBitBlockerByDirection(Direction direction) const
{
    if (direction == Direction::N ||
        direction == Direction::NW ||
        direction == Direction::NE ||
        direction == Direction::E)
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

uint64_t Position::getOpposingPieces(Color c) const
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

uint64_t Position::getOwnPieces(Color c) const
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

Piece Position::getPieceAt(uint64_t s) const
{
    if ((s & allPieces) != 0x00ULL)
    {
        if ((whitePawns & s) != 0x00)
        {
            return Piece(Color::WHITE, Rank::PAWN, s);
        }
        else if ((whiteKnights & s) != 0x00)
        {
            return Piece(Color::WHITE, Rank::KNIGHT, s);
        }
        else if ((whiteBishops & s) != 0x00)
        {
            return Piece(Color::WHITE, Rank::BISHOP, s);
        }
        else if ((whiteRooks & s) != 0x00)
        {
            return Piece(Color::WHITE, Rank::ROOK, s);
        }
        else if ((whiteQueens & s) != 0x00)
        {
            return Piece(Color::WHITE, Rank::QUEEN, s);
        }
        else if ((whiteKing & s) != 0x00)
        {
            return Piece(Color::WHITE, Rank::KING, s);
        }
        else if ((blackPawns & s) != 0x00)
        {
            return Piece(Color::BLACK, Rank::PAWN, s);
        }
        else if ((blackKnights & s) != 0x00)
        {
            return Piece(Color::BLACK, Rank::KNIGHT, s);
        }
        else if ((blackBishops & s) != 0x00)
        {
            return Piece(Color::BLACK, Rank::BISHOP, s);
        }
        else if ((blackRooks & s) != 0x00)
        {
            return Piece(Color::BLACK, Rank::ROOK, s);
        }
        else if ((blackQueens & s) != 0x00)
        {
            return Piece(Color::BLACK, Rank::QUEEN, s);
        }
        else if ((blackKing & s) != 0x00)
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

uint64_t Position::getPinnedPieceByDirection(int kingIndex, uint64_t ownPieces,
    Direction d) const
{
    uint64_t betweenMask {0x00ULL};
    int pinningPieceIndex {getPotentialPinningPiece(kingIndex, d)};

    if (pinningPieceIndex != -1)
    {
        betweenMask = LookupData::betweenLookup[kingIndex][pinningPieceIndex] &
            allPieces;

        if (Utility::popcount(betweenMask) == 1 &&
            (betweenMask & ownPieces) != 0x00ULL)
        {
            return betweenMask;
        }
    }

    return 0x00ULL;
}

int Position::getPotentialPinningPiece(int locationIndex, Direction dir) const
{
    int index {-1};
    uint64_t potentialPinners {0x00ULL};
    uint64_t queensRooks {0x00ULL};
    uint64_t queensBishops {0x00ULL};

    if (colorToMove == Color::WHITE)
    {
        queensRooks = blackQueens | blackRooks;
        queensBishops = blackQueens | blackBishops;
    }
    else
    {
        queensRooks = whiteQueens | whiteRooks;
        queensBishops = whiteQueens | whiteBishops;
    }

    if (dir == Direction::N || dir == Direction::S ||
        dir == Direction::E || dir == Direction::W)
    {
        potentialPinners = queensRooks &
            LookupData::getRayAttacksByDirection(dir)[locationIndex];

        if (dir == Direction::N || dir == Direction::E)
        {
            index = Utility::lsb(potentialPinners);
        }
        else
        {

            index = Utility::msb(potentialPinners);
        }
    }
    else
    {
        potentialPinners = queensBishops &
            LookupData::getRayAttacksByDirection(dir)[locationIndex];

        if (dir == Direction::NE || dir == Direction::NW)
        {
            index = Utility::lsb(potentialPinners);
        }
        else
        {
            index = Utility::msb(potentialPinners);
        }
    }

    return index;
}

bool Position::isSlidingPiecePinned(const Piece p, Direction d) const
{
    if (d == Direction::N)
    {
        return (p.square & (currentPinnedPiecesE |
            currentPinnedPiecesW |
            currentPinnedPiecesNE |
            currentPinnedPiecesNW |
            currentPinnedPiecesSW |
            currentPinnedPiecesSE)) != 0x00ULL;
    }
    else if (d == Direction::S)
    {
        return (p.square & (currentPinnedPiecesE |
            currentPinnedPiecesW |
            currentPinnedPiecesNE |
            currentPinnedPiecesNW |
            currentPinnedPiecesSW |
            currentPinnedPiecesSE)) != 0x00ULL;
    }
    else if (d == Direction::W)
    {
        return (p.square & (currentPinnedPiecesS |
            currentPinnedPiecesN |
            currentPinnedPiecesNE |
            currentPinnedPiecesNW |
            currentPinnedPiecesSW |
            currentPinnedPiecesSE)) != 0x00ULL;
    }
    else if (d == Direction::E)
    {
        return (p.square & (currentPinnedPiecesN |
            currentPinnedPiecesS |
            currentPinnedPiecesNE |
            currentPinnedPiecesNW |
            currentPinnedPiecesSW |
            currentPinnedPiecesSE)) != 0x00ULL;
    }
    else if (d == Direction::NW)
    {
        return (p.square & (currentPinnedPiecesE |
            currentPinnedPiecesW |
            currentPinnedPiecesNE |
            currentPinnedPiecesN |
            currentPinnedPiecesSW |
            currentPinnedPiecesS)) != 0x00ULL;
    }
    else if (d == Direction::NE)
    {
        return (p.square & (currentPinnedPiecesE |
            currentPinnedPiecesW |
            currentPinnedPiecesN |
            currentPinnedPiecesNW |
            currentPinnedPiecesS |
            currentPinnedPiecesSE)) != 0x00ULL;
    }
    else if (d == Direction::SW)
    {
        return (p.square & (currentPinnedPiecesE |
            currentPinnedPiecesW |
            currentPinnedPiecesN |
            currentPinnedPiecesNW |
            currentPinnedPiecesS |
            currentPinnedPiecesSE)) != 0x00ULL;
    }
    else
    {
        return (p.square & (currentPinnedPiecesE |
            currentPinnedPiecesW |
            currentPinnedPiecesNE |
            currentPinnedPiecesN |
            currentPinnedPiecesSW |
            currentPinnedPiecesS)) != 0x00ULL;
    }
}

bool Position::isCastleLegal(uint64_t s) const
{
    bool result = false;

    if (s == 0x0000000000000040ULL)
    {
        result = colorToMove == Color::WHITE &&
            castlingStatus.getWhiteKingSide() &&
            ((0x20ULL & allPieces) == 0x00) &&
            ((0x40ULL & allPieces) == 0x00) &&
            !isSquareAttacked(0x10ULL) &&
            !isSquareAttacked(0x20ULL) &&
            !isSquareAttacked(0x40ULL); 
    }
    else if (s == 0x0000000000000004ULL)
    {
        result = colorToMove == Color::WHITE &&
            castlingStatus.getWhiteQueenSide() &&
            ((0x08ULL & allPieces) == 0x00) &&
            ((0x04ULL & allPieces) == 0x00) &&
            ((0x02ULL & allPieces) == 0x00) &&
            !isSquareAttacked(0x10ULL) &&
            !isSquareAttacked(0x08ULL) &&
            !isSquareAttacked(0x04ULL);
    }
    else if (s == 0x4000000000000000ULL)
    {
        result = colorToMove == Color::BLACK &&
            castlingStatus.getBlackKingSide() &&
            ((0x2000000000000000ULL & allPieces) == 0x00) &&
            ((0x4000000000000000ULL & allPieces) == 0x00) &&
            !isSquareAttacked(0x1000000000000000ULL) &&
            !isSquareAttacked(0x2000000000000000ULL) &&
            !isSquareAttacked(0x4000000000000000ULL);
    }
    else if (s == 0x0400000000000000ULL)
    {
        result = colorToMove == Color::BLACK &&
            castlingStatus.getBlackQueenSide() &&
            ((0x0800000000000000ULL & allPieces) == 0x00) &&
            ((0x0400000000000000ULL & allPieces) == 0x00) &&
            ((0x0200000000000000ULL & allPieces) == 0x00) &&
            !isSquareAttacked(0x1000000000000000ULL) &&
            !isSquareAttacked(0x0800000000000000ULL) &&
            !isSquareAttacked(0x0400000000000000ULL);
    }

    return result;
}

bool Position::isMoveLegal(const Move& m)
{
    Piece p = m.movingPiece;
    uint64_t kingLocation {colorToMove == Color::WHITE ? whiteKing : blackKing};

    bool result = false;
    bool isCastle = m.isCastle();

    if (kingLocation == 0x00ULL)
    {
        return false;
    }

    if (colorToMove != m.movingPiece.color)
    {
        return false;
    }

    if (!isSquareAttacked(kingLocation))
    {
        if (p.rank != Rank::KING)
        {
            result = true;
        }
        else
        {
            if (isCastle)
            {
                result = isCastleLegal(m.targetSquare);
            }
            else if (!isSquareAttacked(m.targetSquare))
            {
                result = true;
            }
        }
    }
    else if (!isCastle)
    {
        makeMove(m);
        makeMove({Piece {}, 0x00ULL, Rank::NONE});

        if (p.rank != Rank::KING)
        {
            if (!isSquareAttacked(kingLocation))
            {
                result = true;
            }
        }
        else
        {
            if (!isSquareAttacked(m.targetSquare))
            {
                result = true;
            }
        }

        unmakeMove();
        unmakeMove();
    }

    return result;
}

bool Position::isSquareAttacked(uint64_t square) const        
{
    bool result = false;
    int squareIndex = Utility::lsb(square);
    int blockerIndex;
    uint64_t opposingKnights =
        colorToMove == Color::WHITE ? blackKnights : whiteKnights;
    uint64_t opposingBishops =
        colorToMove == Color::WHITE ? blackBishops : whiteBishops;
    uint64_t opposingRooks =
        colorToMove == Color::WHITE ? blackRooks : whiteRooks;
    uint64_t opposingQueens =
        colorToMove == Color::WHITE ? blackQueens : whiteQueens;

    if ((LookupData::knightAttacks[squareIndex] & opposingKnights) !=
        0x0000000000000000ULL)
    {
        result = true;
    }

    blockerIndex = Utility::lsb(LookupData::rayAttacksN[squareIndex] & allPieces);

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Utility::lsb(LookupData::rayAttacksNE[squareIndex] &
        allPieces);

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Utility::lsb(LookupData::rayAttacksE[squareIndex] &
        (allPieces));

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Utility::msb(LookupData::rayAttacksSE[squareIndex] &
        (allPieces));

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Utility::msb(LookupData::rayAttacksS[squareIndex] &
        (allPieces));

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Utility::msb(LookupData::rayAttacksSW[squareIndex] &
        (allPieces));

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Utility::msb(LookupData::rayAttacksW[squareIndex] &
        (allPieces));

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Utility::lsb(LookupData::rayAttacksNW[squareIndex] &
        (allPieces));

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

void Position::removePieceAt(uint64_t loc)
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
    uint64_t currentSquare = 0x0000000000000000ULL;
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
        uint64_t epLoc = Utility::algebraicToBitboard(fenParts[3]);
        
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
