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
#include "BitOperations.hpp"
#include "LookupData.hpp"
#include <sstream>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>

// begin constructors

TuxedoCat::Position::Position(std::string fen)
    : castlingStatus(CastlingStatus(false, false, false, false))
{
    setupPositionFromFen(fen);
}

TuxedoCat::Position::Position(const Position& p)
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
}

// end constructors

// begin public methods

bool TuxedoCat::Position::isMoveValid(const Move& m)
{
    Piece p = m.getMovingPiece();
    std::vector<Square> kingLocationVector =
        findPiece(p.getColor(), Rank::KING);

    Square kingLocation;
    bool result = false;

    if (kingLocationVector.empty())
    {
        return false;
    }

    kingLocation = kingLocationVector[0];

    if (!isSquareAttacked(kingLocation))
    {
        if (p.getRank() != Rank::KING)
        {
            result = true;
        }
        else
        {
            uint64_t kingBitmask = kingLocation.getLocation();
            uint64_t targetBitmask = m.getTargetLocation().getLocation();

            if (
                m.getMovingPiece().getRank() == Rank::KING &&
                kingBitmask == 0x0000000000000010ULL &&
                targetBitmask == 0x0000000000000040ULL &&
                castlingStatus.getWhiteKingSide() &&
                !getPieceAt(Square(0, 5)) &&
                !getPieceAt(Square(0, 6)) &&
                !isSquareAttacked(kingLocation) &&
                !isSquareAttacked(Square(0, 5)) &&
                !isSquareAttacked(Square(0, 6))
                )
            {
                result = true;
            }
            else if (
                m.getMovingPiece().getRank() == Rank::KING &&
                kingBitmask == 0x0000000000000010ULL &&
                targetBitmask == 0x0000000000000004ULL &&
                castlingStatus.getWhiteQueenSide() &&
                !getPieceAt(Square(0, 1)) &&
                !getPieceAt(Square(0, 2)) &&
                !getPieceAt(Square(0, 3)) &&
                !isSquareAttacked(kingLocation) &&
                !isSquareAttacked(Square(0, 3)) &&
                !isSquareAttacked(Square(0, 2))
                )
            {
                result = true;
            }
            else if (
                m.getMovingPiece().getRank() == Rank::KING &&
                kingBitmask == 0x1000000000000000ULL &&
                targetBitmask == 0x4000000000000000ULL &&
                castlingStatus.getBlackKingSide() &&
                !getPieceAt(Square(7, 5)) &&
                !getPieceAt(Square(7, 6)) &&
                !isSquareAttacked(kingLocation) &&
                !isSquareAttacked(Square(7, 5)) &&
                !isSquareAttacked(Square(7, 6))
                )
            {
                result = true;
            }
            else if (
                m.getMovingPiece().getRank() == Rank::KING &&
                kingBitmask == 0x1000000000000000ULL &&
                targetBitmask == 0x0400000000000000ULL &&
                castlingStatus.getBlackQueenSide()
                !getPieceAt(Square(7, 1)) &&
                !getPieceAt(Square(7, 2)) &&
                !getPieceAt(Square(7, 3)) &&
                !isSquareAttacked(kingLocation) &&
                !isSquareAttacked(Square(7, 3)) &&
                !isSquareAttacked(Square(7, 2))
                )
            {
                result = true;
            }
            else if (!isSquareAttacked(m.getTargetLocation()))
            {
                result = true;
            }
        }
    }
    else
    {
        makeMove(m);
        makeMove(boost::none);

        if (p.getRank() != Rank::KING)
        {
            if (!isSquareAttacked(kingLocation))
            {
                result = true;
            }
        }
        else
        {
            if (!isSquareAttacked(m.getTargetLocation()))
            {
                result = true;
            }
        }

        unmakeMove();
        unmakeMove();
    }

    return result;
}

void TuxedoCat::Position::makeMove(boost::optional<const Move&> move)
{
    uint64_t sourceLocation;
    uint64_t targetLocation;
    uint64_t tmpEnPassant;
    Color sourceColor;
    Rank sourceRank;
    boost::optional<Rank> promotedRank;
    boost::optional<Piece> capturedPiece;

    positionStack.push(*this);

    if (!move)
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

    sourceLocation = move->getMovingPiece().getLocation().getLocation();
    targetLocation = move->getTargetLocation().getLocation();
    sourceColor = move->getMovingPiece().getColor();
    sourceRank = move->getMovingPiece().getRank();
    capturedPiece = getPieceAt(Square(targetLocation));
    promotedRank = move->getPromotedRank();

    if (enPassantTarget)
    {
        tmpEnPassant = *enPassantTarget;
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

            if (capturedPiece)
            {
                if (enPassantTarget)
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

            if (promotedRank)
            {
                addPieceAt(targetLocation, sourceColor, *promotedRank);
            }
            else
            {
                addPieceAt(targetLocation, sourceColor, Rank::PAWN);
            }

            removePieceAt(sourceLocation);
        }
        else
        {
            if (capturedPiece)
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

    if (enPassantTarget)
    {
        if (tmpEnPassant == *enPassantTarget)
        {
            enPassantTarget = boost::none;
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

std::string TuxedoCat::Position::toString() const
{
    boost::optional<Piece> currentPiece;
    std::stringstream ss;

    for (int row = 7; row >= 0; row--)
    {
        for (int col = 0; col < 8; col++)
        {
            if ((currentPiece = getPieceAt(Square(row, col))))
            {
                ss << currentPiece->toString();
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

void TuxedoCat::Position::unmakeMove()
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

        positionStack.pop();
    }
}

// end public methods

// begin private methods

void TuxedoCat::Position::addPieceAt(uint64_t loc, Color c, Rank r)
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

std::vector<TuxedoCat::Square> TuxedoCat::Position::findPiece(Color c,
    Rank r) const
{
    std::vector<Square> locations;
    uint64_t bitboard;
    uint64_t currentLocation;

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
        currentLocation = 0x01ULL << BitOperations::getLSB(bitboard);

        locations.push_back(Square(currentLocation));

        bitboard = bitboard & (~currentLocation);
    }

    return locations;
}

std::vector<Move> generateCastles() const
{
    std::vector<Move> moves;
    boost::optional<Piece> piece;

    if (colorToMove == Color::WHITE)
    {
        piece = getPieceAt(Square(0, 4));

        if (piece && piece->getRank() == Rank::KING &&
            piece->getColor() == Color::WHITE)
        {
            Move castleKingSide(piece, Square(0, 6), boost::none);
            Move castleQueenSide(piece, Square(0, 2), boost::none);

            if (isMoveValid(castleKingSide))
            {
                moves.push_back(castleKingSide);
            }

            if (isMoveValid(castleQueenSide))
            {
                moves.push_back(castleQueenSide);
            }
        }
    }
    else
    {
        piece = getPieceAt(Square(7, 4));

        if (piece && piece->getRank() == Rank::KING &&
            piece->getColor() == Color::BLACK)
        {
            Move castleKingSide(piece, Square(7, 6), boost::none);
            Move castleQueenSide(piece, Square(7, 2), boost::none);

            if (isMoveValid(castleKingSide))
            {
                moves.push_back(castleKingSide);
            }

            if (isMoveValid(castleQueenSide))
            {
                moves.push_back(castleQueenSide);
            }
        }
    }

    return moves;
}

std::vector<Move> generateKingMovesAt(const Square& s) const
{
    std::vector<Move> moves;
    int locationIndex;
    boost::optional<Piece> piece = getPieceAt(s);
    uint64_t location = s.getLocation();
    uint64_t moveMask = 0x0000000000000000ULL;
    uint64_t currentMove;
    Color color;
    uint64_t ownPieces;

    if (!piece || piece->getRank() != Rank::KING)
    {
        return moves;
    }

    color = piece->getColor();

    if (color == Color::WHITE)
    {
        ownPieces = whitePieces;
    }
    else
    {
        ownPieces = blackPieces;
    }

    std::vector<Move> castles = generateCastles();
    moves.insert(std::end(moves), std::begin(castles), std::end(castles));

    locationIndex = GetLSB(location);
    moveMask = LookupData::kingAttacks[locationIndex] & (~ownPieces);

    while (moveMask != 0x0000000000000000ULL)
    {
        currentMove =
            0x0000000000000001ULL << BitOperations::getLSB(moveMask);
        Move m(piece, currentMove, boost::none);

        if (isMoveValid(m))
        {
            moves.push_back(m);
        }

        moveMask = moveMask & (~currentMove);
    }
}

std::vector<TuxedoCat::Move> generateKnightMovesAt(const Square& s,
    bool inCheck) const
{
    std::vector<Move> moves;
    boost::optional<Piece> piece = getPieceAt(s);
    uint64_t location = s.getLocation();
    int locationIndex;
    uint64_t moveMask = 0x0000000000000000ULL;
    uint64_t currentMove;
    Color color;
    uint64_t ownPieces;

    if (
        !piece ||
        piece->getRank() != Rank::KNIGHT ||
        isPiecePinned(*piece, Direction::NS) ||
        isPiecePinned(*piece, Direction::EW) ||
        isPiecePinned(*piece, Direction::SWNE) ||
        isPiecePinned(*piece, Direction::NWSE)
        )
    {
        return moves;
    }
    else
    {
        color = piece->getColor();
    }

    if (color == Color::WHITE)
    {
        ownPieces = whitePieces;
    }
    else
    {
        ownPieces = blackPieces;
    }

    locationIndex = BitOperations::getLSB(location);

    moveMask = LookupData::knightAttacks[locationIndex] & (~ownPieces);

    while (moveMask != 0x0000000000000000ULL)
    {
        currentMove = 0x0000000000000001ULL << GetLSB(moveMask);
        Move m(piece, Square(currentMove), boost::none);

        if ((inCheck && isMoveValid(m)) || !inCheck)
        {
            moves.push_back(Move(piece, Square(currentMove), boost::none);
        }

        moveMask = moveMask & (~currentMove);
    }

    return moves;
}



std::vector<Move> generatePawnMovesAt(const Square& s, bool inCheck) const
{
    uint64_t location = s.getLocation();
    uint64_t advancedLocation;
    uint64_t doubleAdvancedLocation;
    uint64_t captureLeftLocation;
    uint64_t captureRightLocation;
    uint64_t startRankMask;
    uint64_t backRankMask;
    uint64_t leftEdgeMask;
    uint64_t rightEdgeMask;
    uint64_t opposingPieces;
    uint64_t leftBackMask;
    uint64_t rightBackMask;
    uint64_t allTargets;
    uint64_t allPieces = whitePieces | blackPieces;
    boost::optional<Piece> piece = getPieceAt(s);
    std::vector<Move> moves;

    if (!piece)
    {
        return moves;
    }

    if (colorToMove == Color::WHITE)
    {
	advancedLocation = location << 8;
	doubleAdvancedLocation = location << 16;
        captureLeftLocation = location << 7;
        captureRightLocation = location << 9;
        startRankMask = 0x000000000000FF00UL;
        backRankMask = 0xFF00000000000000UL;
        leftEdgeMask = 0x0101010101010101UL;
        rightEdgeMask = 0x8080808080808080UL;
        opposingPieces = blackPieces;
    }
    else
    {
        advancedLocation = location >> 8;
	doubleAdvancedLocation = location >> 16;
        captureLeftLocation = location >> 7;
        captureRightLocation = location >> 9;
        startRankMask = 0x00FF000000000000UL;
        backRankMask = 0x00000000000000FFUL;
        leftEdgeMask = 0x8080808080808080UL;
        rightEdgeMask = 0x0101010101010101UL;
        opposingPieces = position.WhitePieces;
    }

    leftBackMask = leftEdgeMask | backRankMask;
    rightBackMask = rightEdgeMask | backRankMask;
    allTargets = opposingPieces | enPassantTarget;

    if (
        !isPiecePinned(Square(location)), Direction::EW &&
        !isPiecePinned(Square(location), Direction::NWSE) &&
        !isPiecePinned(Square(location), Direction::SWNE) &&
        (advancedLocation & allPieces) == 0x0000000000000000ULL &&
        (doubleAdvancedLocation & allPieces) == 0x0000000000000000ULL &&
        (location & startRankMask) == location
        )
    {
        Move m(*piece, Square(doubleAdvancedLocation), boost::none)

        if (!inCheck || (inCheck && isMoveValid(m)))
        {
            moves.push_back(m);
        }
    }

    if (
        !isPiecePinned(Square(location)), Direction::EW &&
        !isPiecePinned(Square(location), Direction::NWSE) &&
        !isPiecePinned(Square(location), Direction::SWNE) &&
        (advancedLocation & allPieces) == 0x0000000000000000ULL &&
        (location & backRankMask) == 0x0000000000000000ULL &&
        (advancedLocation & backRankMask) == 0x0000000000000000ULL
        )
    {
        Move m(*piece, Square(advancedLocation), boost::none)

        if (!inCheck || (inCheck && isMoveValid(m)))
        {
            moves.push_back(m);
        }
    }

    if (
        !isPiecePinned(Square(location)), Direction::EW &&
        !isPiecePinned(Square(location), Direction::NWSE) &&
        !isPiecePinned(Square(location), Direction::SWNE) &&
        (advancedLocation & allPieces) == 0x0000000000000000ULL &&
        (location & backRankMask) == 0x0000000000000000ULL &&
        (advancedLocation & backRankMask) != 0x0000000000000000ULL
        )
    {
        Move m1(*piece, Square(advancedLocation), Rank::QUEEN);
        Move m2(*piece, Square(advancedLocation), Rank::ROOK);
        Move m3(*piece, Square(advancedLocation), Rank::KNIGHT);
        Move m4(*piece, Square(advancedLocation), Rank::BISHOP);

        if (!inCheck || (inCheck && isMoveValid(m1)))
        {
            moves.push_back(m1);
            moves.push_back(m2);
            moves.push_back(m3);
            moves.push_back(m4);
        }
    }

    if (!isPiecePinned(Square(location), Direction::NS) &&
        !isPiecePinned(Square(location), Direction::SWNE) &&
        (location & leftBackMask) == 0x0000000000000000ULL &&
        (captureLeftLocation & allTargets) != 0x0000000000000000ULL
        )
    {
        Move m1(*piece, Square(captureLeftLocation), boost::none);
        Move m2(*piece, Square(captureLeftLocation), Rank::QUEEN);
        Move m3(*piece, Square(captureLeftLocation), Rank::ROOK);
        Move m4(*piece, Square(captureLeftLocation), Rank::BISHOP);
        Move m5(*piece, Square(captureLeftLocation), Rank::KNIGHT);

        if ((captureLeftLocation & enPassantTarget) != 0x00ULL &&
            isMoveValid(m1))
        {
            moves.push_back(m1);
        }

        if ((captureLeftLocation & backRankMask) != 0x00ULL)
        {
            if (!inCheck || isMoveValid(m2))
            {
                moves.push_back(m2);
                moves.push_back(m3);
                moves.push_back(m4);
                moves.push_back(m5);
            }
        }
    }

    if (!isPiecePinned(Square(location), Direction::NS) &&
        !isPiecePinned(Square(location), Direction::NWSE) &&
        (location & rightBackMask) == 0x0000000000000000ULL &&
        (captureRightLocation & allTargets) != 0x0000000000000000ULL
        )
    {
        Move m1(*piece, Square(captureRightLocation), boost::none);
        Move m2(*piece, Square(captureRightLocation), Rank::QUEEN);
        Move m3(*piece, Square(captureRightLocation), Rank::ROOK);
        Move m4(*piece, Square(captureRightLocation), Rank::BISHOP);
        Move m5(*piece, Square(captureRightLocation), Rank::KNIGHT);

        if ((captureRightLocation & enPassantTarget) != 0x00ULL &&
            isMoveValid(m1))
        {
            moves.push_back(m1);
        }

        if ((captureRightLocation & backRankMask) != 0x00ULL)
        {
            if (!inCheck || isMoveValid(m2))
            {
                moves.push_back(m2);
                moves.push_back(m3);
                moves.push_back(m4);
                moves.push_back(m5);
            }
        }
    }

    return moves;
}

boost::optional<TuxedoCat::Piece> TuxedoCat::Position::getPieceAt(Square s) const
{
    if (!isSquareEmpty(s))
    {
        if ((whitePawns & s.getLocation()) != 0x00)
        {
            return Piece(Color::WHITE, Rank::PAWN, s);
        }
        else if ((whiteKnights & s.getLocation()) != 0x00)
        {
            return Piece(Color::WHITE, Rank::KNIGHT, s);
        }
        else if ((whiteBishops & s.getLocation()) != 0x00)
        {
            return Piece(Color::WHITE, Rank::BISHOP, s);
        }
        else if ((whiteRooks & s.getLocation()) != 0x00)
        {
            return Piece(Color::WHITE, Rank::ROOK, s);
        }
        else if ((whiteQueens & s.getLocation()) != 0x00)
        {
            return Piece(Color::WHITE, Rank::QUEEN, s);
        }
        else if ((whiteKing & s.getLocation()) != 0x00)
        {
            return Piece(Color::WHITE, Rank::KING, s);
        }
        else if ((blackPawns & s.getLocation()) != 0x00)
        {
            return Piece(Color::BLACK, Rank::PAWN, s);
        }
        else if ((blackKnights & s.getLocation()) != 0x00)
        {
            return Piece(Color::BLACK, Rank::KNIGHT, s);
        }
        else if ((blackBishops & s.getLocation()) != 0x00)
        {
            return Piece(Color::BLACK, Rank::BISHOP, s);
        }
        else if ((blackRooks & s.getLocation()) != 0x00)
        {
            return Piece(Color::BLACK, Rank::ROOK, s);
        }
        else if ((blackQueens & s.getLocation()) != 0x00)
        {
            return Piece(Color::BLACK, Rank::QUEEN, s);
        }
        else if ((blackKing & s.getLocation()) != 0x00)
        {
            return Piece(Color::BLACK, Rank::KING, s);
        }
        else
        {
            return boost::none;
        }
    }
    else
    {
        return boost::none;
    }
}

bool TuxedoCat::Position::isPiecePinned(const Piece& pinnedPiece,
    Direction direction)
{
    uint64_t location = pinnedPiece.getLocation().getLocation();
    bool result = true;
    int locationMaskIndex;
    Color pinnedColor = pinnedPiece->getColor();
    Color pinningColor = pinnedColor == Color::WHITE ?
        Color::BLACK : Color::WHITE;

    uint64_t pinMask;
    uint64_t pinnedKingLocation;
    uint64_t mask;
    uint64_t occupancy;
    uint64_t tmpLocation;
    uint64_t guard = 0x00;
    int offset;
    bool goUp;

    if (pinningColor == Color::WHITE)
    {
        if (direction == Direction::NS || direction == Direction::EW) 
        {
            pinMask = whiteQueens | whiteRooks;
        }
        else
        {
            pinMask = whiteQueens | whiteBishops;
        }

        pinnedKingLocation = blackKing;
    }
    else
    {
        if (direction == Direction::NS || direction == Direction::EW)
        {
            pinMask = blackQueens | blackRooks;
        }
        else
        {
            pinMask = blackQueens | blackBishops;
        }

        pinnedKingLocation = whiteKing;
    }

    locationMaskIndex = BitOperations::getLSB(location);

    if (direction == Direction::NS)
    {
        mask = LookupData::fileMask[locationMaskIndex];
        offset = 8;
    }
    else if (direction == Direction::EW)
    {
        mask = LookupData::rankMask[locationMaskIndex];
        offset = 1;
    }
    else if (direction == Direction::SWNE)
    {
        mask = LookupData::swneMask[locationMaskIndex];
        offset = 9;
    }
    else if (direction == Direction::NWSE)
    {
        mask = LookupData::nwseMask[locationMaskIndex];
        offset = 7;
    }
    else
    {
        mask = 0x0000000000000000UL;
    }

    if ((mask & pinnedKingLocation) != 0x0000000000000000UL &&
        (mask & pinMask) != 0x0000000000000000UL)
    {
        occupancy = (whitePieces | blackPieces) & mask;

        if (pinnedKingLocation < location)
        {
            goUp = true;
        }
        else
        {
            goUp = false;
        }

        if (direction == Direction::SWNE)
        {
            guard = goUp ? 0xFF80808080808080UL : 0x01010101010101FFUL;
        }
        else if (direction == Direction::EW)
        {
            guard = goUp ? 0x8080808080808080UL : 0x0101010101010101UL;
        }
        else if (direction == Direction::NS)
        {
            guard = goUp ? 0xFF00000000000000UL : 0x00000000000000FFUL;
        }
        else if (direction == Direction::NWSE)
        {
            guard = goUp ? 0xFF01010101010101UL : 0x80808080808080FFUL;
        }

        if ((pinnedKingLocation & guard) == 0x0000000000000000UL)
        {
            tmpLocation = goUp ? pinnedKingLocation << offset :
                pinnedKingLocation >> offset;

            while (tmpLocation != location)
            {
                if ((tmpLocation & occupancy) != 0x0000000000000000UL)
                {
                    result = false;
                    break;
                }

                tmpLocation = goUp ? tmpLocation << offset :
                    tmpLocation >> offset;
            }

            if (result && (tmpLocation & guard) == 0x0000000000000000UL)
            {
                tmpLocation = goUp ? tmpLocation << offset :
                    tmpLocation >> offset;

                while (true)
                {
                    if ((tmpLocation & occupancy) !=
                        0x0000000000000000UL)
                    {
                        if ((tmpLocation & pinMask) ==
                            0x0000000000000000UL)
                        {
                            result = false;
                            break;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        if ((tmpLocation & guard) != 0x0000000000000000UL)
                        {
                            tmpLocation = goUp ?
                                tmpLocation << offset :
                                tmpLocation >> offset;
                        }
                        else
                        {
                            result = false;
                            break;
                        }
                    }
                }
            }
            else
            {
                result = false;
            }
        }
        else
        {
            result = false;
        }
    }
    else
    {
        result = false;
    }

    return result;
}

bool TuxedoCat::Position::isSquareAttacked(Square s) const        
{
    bool result = false;
    int squareIndex = BitOperations::getLSB(s.getLocation());
    uint64_t square = s.getLocation();
    int blockerIndex;
    uint64_t opposingKnights =
        colorToMove == Color::WHITE ? blackKnights : whiteKnights;
    uint64_t opposingBishops =
        colorToMove == Color::WHITE ? blackBishops : whiteBishops;
    uint64_t opposingRooks =
        colorToMove == Color::WHITE ? blackRooks : whiteBishops;
    uint64_t opposingQueens =
        colorToMove == Color::WHITE ? blackQueens : whiteKnights;

    if ((LookupData::knightAttacks[squareIndex] & opposingKnights) !=
        0x0000000000000000ULL)
    {
        result = true;
    }

    blockerIndex = BitOperations::getLSB(
        LookupData::rayAttacksN[squareIndex] &
        (whitePieces | blackPieces));

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = BitOperations::getLSB(
        LookupData::rayAttacksNE[squareIndex] &
        (whitePieces | blackPieces));

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = BitOperations::getLSB(
        LookupData::rayAttacksE[squareIndex] &
        (whitePieces | blackPieces));

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = BitOperations::getMSB(
        LookupData::rayAttacksSE[squareIndex] &
        (whitePieces | blackPieces));

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = BitOperations::getMSB(
        LookupData::rayAttacksS[squareIndex] &
        (whitePieces | blackPieces));

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = BitOperations::getMSB(
        LookupData::rayAttacksSW[squareIndex] &
        (whitePieces | blackPieces));

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = BitOperations::getMSB(
        LookupData::rayAttacksW[squareIndex] &
        (whitePieces | blackPieces));

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = BitOperations::getLSB(
        LookupData::rayAttacksNW[squareIndex] &
        (whitePieces | blackPieces));

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

bool TuxedoCat::Position::isSquareEmpty(Square s) const
{
    return ((whitePieces | blackPieces) && s.getLocation()) == 0x00;
}

void TuxedoCat::Position::removePieceAt(uint64_t loc)
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

void TuxedoCat::Position::setupPositionFromFen(std::string fen)
{
    std::vector<std::string> fenParts;
    std::vector<std::string> rankInfo;
    uint64_t currentSquare = 0x0000000000000000ULL;

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

    boost::split(fenParts, fen, boost::is_any_of(" \t"),
        boost::token_compress_on);

    if (fenParts.size() != 6)
    {
        return;
    }

    boost::split(rankInfo, fenParts[0], boost::is_any_of("/"),
        boost::token_compress_on);

    if (rankInfo.size() != 8)
    {
        return;
    }

    for (int i = 0; i < 8; i++)
    {
        currentSquare = 0x0000000000000001ULL << (8 * (7 - i));

        for (size_t j = 0; j < rankInfo[i].length(); j++)
        {
            char currentCharacter = rankInfo[i][j];
            if (currentCharacter > 47 && currentCharacter < 58)
            {
                currentSquare = currentSquare << (currentCharacter - 48);
            }
            else
            {
                addPieceAt(currentSquare,
                    getColorFromChar(currentCharacter),
                    getRankFromChar(currentCharacter));
                        
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
        enPassantTarget = boost::none;
    }
    else
    {
        uint64_t epLoc = Square(fenParts[3]).getLocation();
        
        if (epLoc != 0x00)
        {
            enPassantTarget = epLoc;
        }
        else
        {
            enPassantTarget = boost::none;
        }
    }

    halfMoveCounter = std::stoi(fenParts[4]);
    fullMoveCounter = std::stoi(fenParts[5]);
}

void TuxedoCat::Position::updatePieces()
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
}
