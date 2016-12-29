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
#include <sstream>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
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
}

// end constructors

// begin public methods

std::vector<Move> Position::generateMoves(boost::optional<Rank> rank)
{
    std::vector<Move> moves;
    std::vector<Move> pawnCaptures;
    std::vector<Move> pawnAdvances;
    std::vector<Move> pawnDblAdvances;
    std::vector<Move> knightMoves;
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

    while (pieces != 0x00ULL)
    {
        currentIndex = pieces.lsb();
        currentSquare = 0x01ULL << currentIndex;

        if (!rank || *rank == Rank::PAWN)
        {
            pawnCaptures = generatePawnCapturesAt(currentSquare);
            pawnAdvances = generatePawnAdvancesAt(currentSquare);
            pawnDblAdvances = generatePawnDblAdvancesAt(currentSquare);

            moves.insert(moves.end(), pawnCaptures.begin(),
                pawnCaptures.end());
            moves.insert(moves.end(), pawnAdvances.begin(),
                pawnAdvances.end());
            moves.insert(moves.end(), pawnDblAdvances.begin(),
                pawnDblAdvances.end());
        }

        if (!rank || *rank == Rank::KNIGHT)
        {
            knightMoves = generateKnightMovesAt(currentSquare);

            moves.insert(moves.end(), knightMoves.begin(),
                knightMoves.end());
        }

        pieces.flipBit(currentIndex);
    }

    return moves;
}

void Position::makeMove(boost::optional<const Move&> move)
{
    Bitboard sourceLocation;
    Bitboard targetLocation;
    Bitboard tmpEnPassant;
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

    sourceLocation = move->getMovingPiece().getSquare().toBitboard();
    targetLocation = move->getTargetSquare().toBitboard();
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

std::string Position::toString() const
{
    boost::optional<Piece> currentPiece;
    std::stringstream ss;

    for (int row = 7; row >= 0; row--)
    {
        for (int col = 0; col < 8; col++)
        {
            if ((currentPiece =
                getPieceAt(Square(std::pair<int, int>(row, col)))))
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

Bitboard Position::computePinningPieceMask(Direction direction) const
{
    Bitboard mask;

    if (colorToMove == Color::BLACK)
    {
        if (direction == Direction::NS || direction == Direction::EW) 
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
        if (direction == Direction::NS || direction == Direction::EW)
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

std::vector<Square> Position::findPiece(Color c,
    Rank r) const
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

std::vector<Move> Position::generatePawnAdvancesAt(Bitboard b)
{
    std::vector<Move> moves;
    Bitboard legalMask;
    Bitboard target;
    boost::optional<Piece> movingPiece = getPieceAt(Square(b));
    bool inCheck {false};

    if (!movingPiece || movingPiece->getRank() != Rank::PAWN ||
        isPiecePinned(*movingPiece, Direction::EW) ||
        isPiecePinned(*movingPiece, Direction::NWSE) ||
        isPiecePinned(*movingPiece, Direction::SWNE))
    {
        return moves;
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

    if (b.inMask(legalMask) && !target.inMask(whitePieces | blackPieces))
    {
        if (target.inMask(0xFF000000000000FFULL))
        {
            Move m1(*movingPiece, target, Rank::QUEEN);
            Move m2(*movingPiece, target, Rank::ROOK);
            Move m3(*movingPiece, target, Rank::BISHOP);
            Move m4(*movingPiece, target, Rank::KNIGHT);

            if ((inCheck && isMoveValid(m1)) || !inCheck)
            {
                moves.push_back(m1);
            }

            if ((inCheck && isMoveValid(m2)) || !inCheck)
            {
                moves.push_back(m2);
            }

            if ((inCheck && isMoveValid(m3)) || !inCheck)
            {
                moves.push_back(m3);
            }

            if ((inCheck && isMoveValid(m4)) || !inCheck)
            {
                moves.push_back(m4);
            }
        }
        else
        {
            Move m1(*movingPiece, target, boost::none);

            if ((inCheck && isMoveValid(m1)) || !inCheck)
            {
                moves.push_back(m1);
            }
        }
    }

    return moves;
}

std::vector<Move> Position::generatePawnCapturesAt(Bitboard b)
{
    std::vector<Move> captures;
    Bitboard validRankMask;
    Bitboard validCaptureLeftMask;
    Bitboard validCaptureRightMask;
    Bitboard captureLeftTarget;
    Bitboard captureRightTarget;
    Bitboard targets;
    Bitboard currentSquare;
    Bitboard opposingPieces;
    int currentIndex;
    boost::optional<Piece> movePiece = getPieceAt(Square(b));
    bool inCheck {false};

    if (!movePiece || movePiece->getRank() != Rank::PAWN)
    {
        return captures;
    }

    if (colorToMove == Color::WHITE)
    {
        validRankMask.setValue(0x00FFFFFFFFFFFFFFULL);
        validCaptureLeftMask.setValue(0xFEFEFEFEFEFEFEFEULL);
        validCaptureRightMask.setValue(0x7F7F7F7F7F7F7F7FULL);
        captureLeftTarget = ((b & validRankMask) & validCaptureLeftMask)
            << 7;
        captureRightTarget = ((b & validRankMask) & validCaptureRightMask)
            << 9;

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
        captureLeftTarget = ((b & validRankMask) & validCaptureLeftMask)
            >> 7;
        captureRightTarget = ((b & validRankMask) & validCaptureRightMask)
            >> 9;

        opposingPieces = whitePieces;

        if (blackKing != 0x00ULL)
        {
            inCheck = isSquareAttacked(Square(blackKing));
        }
    }

    targets = (captureLeftTarget | captureRightTarget) & opposingPieces;

    if (enPassantTarget)
    {
        targets |= *enPassantTarget;
    }

    while (targets != 0x00ULL)
    {
        currentIndex = targets.lsb();
        currentSquare = 0x01ULL << currentIndex;

        if (currentSquare.inMask(0xFF000000000000FFULL))
        {
            Move m1(*movePiece, currentSquare, Rank::QUEEN);
            Move m2(*movePiece, currentSquare, Rank::ROOK);
            Move m3(*movePiece, currentSquare, Rank::BISHOP);
            Move m4(*movePiece, currentSquare, Rank::KNIGHT);

            if ((inCheck && isMoveValid(m1)) || !inCheck)
            {
                captures.push_back(m1);
            }

            if ((inCheck && isMoveValid(m2)) || !inCheck)
            {
                captures.push_back(m2);
            }

            if ((inCheck && isMoveValid(m3)) || !inCheck)
            {
                captures.push_back(m3);
            }

            if ((inCheck && isMoveValid(m4)) || !inCheck)
            {
                captures.push_back(m4);
            }
        }
        else
        {
            Move m1(*movePiece, currentSquare, boost::none);

            if ((inCheck && isMoveValid(m1)) || !inCheck)
            {
                captures.push_back(m1);
            }
        }

        targets.flipBit(currentIndex);
    }

    return captures;
}

std::vector<Move> Position::generatePawnDblAdvancesAt(Bitboard b)
{
    std::vector<Move> moves;
    Bitboard legalMask;
    Bitboard target;
    Bitboard firstSquare;
    boost::optional<Piece> movingPiece = getPieceAt(Square(b));
    bool inCheck {false};

    if (!movingPiece || movingPiece->getRank() != Rank::PAWN ||
        isPiecePinned(*movingPiece, Direction::EW) ||
        isPiecePinned(*movingPiece, Direction::NWSE) ||
        isPiecePinned(*movingPiece, Direction::SWNE))
    {
        return moves;
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
        !firstSquare.inMask(whitePieces | blackPieces) &&
        !target.inMask(whitePieces | blackPieces))
    {
        Move m(*movingPiece, target, boost::none);

        if ((inCheck && isMoveValid(m)) || !inCheck)
        moves.push_back(m);
    }

    return moves;
}

/*std::vector<Move> generateCastles() const
{
    std::vector<Move> moves;
    boost::optional<Piece> piece;

    if (colorToMove == Color::WHITE)
    {
        piece = getPieceAt(Square(std::pair<int, int>(0, 4)));

        if (piece && piece->getRank() == Rank::KING &&
            piece->getColor() == Color::WHITE)
        {
            Move castleKingSide(piece, Square(std::pair<int, int>(0, 6)),
                boost::none);
            Move castleQueenSide(piece, Square(std::pair<int, int>(0, 2)),
                boost::none);

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
        piece = getPieceAt(Square(std::pair<int, int>(7, 4)));

        if (piece && piece->getRank() == Rank::KING &&
            piece->getColor() == Color::BLACK)
        {
            Move castleKingSide(piece, Square(std::pair<int, int>(7, 6)),
                boost::none);
            Move castleQueenSide(piece, Square(std::pair<int, int>(7, 2)),
                boost::none);

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
    Bitboard location = s.toBitboard();
    Bitboard moveMask = 0x0000000000000000ULL;
    Bitboard currentMove;
    Color color;
    Bitboard ownPieces;

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

    locationIndex = location.lsb();
    moveMask = LookupData::kingAttacks[locationIndex] & (~ownPieces);

    while (moveMask != 0x0000000000000000ULL)
    {
        currentMove =
            0x0000000000000001ULL << moveMask.lsb();
        Move m(piece, currentMove, boost::none);

        if (isMoveValid(m))
        {
            moves.push_back(m);
        }

        moveMask = moveMask & (~currentMove);
    }
}*/

std::vector<Move> Position::generateKnightMovesAt(Square s)
{
    std::vector<Move> moves;
    boost::optional<Piece> piece = getPieceAt(s);
    Bitboard location = s.toBitboard();
    int locationIndex;
    Bitboard moveMask = 0x0000000000000000ULL;
    Bitboard currentMove;
    Color color;
    Bitboard ownPieces;
    int currentIndex;
    bool inCheck {false};

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

    color = piece->getColor();

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
        Move m(*piece, Square(currentMove), boost::none);

        if ((inCheck && isMoveValid(m)) || !inCheck)
        {
            moves.push_back(m);
        }

        moveMask = moveMask.flipBit(currentIndex);
    }

    return moves;
}

int Position::getOffsetFromDirection(Direction direction) const
{
    int offset = 0;
    
    if (direction == Direction::NS)
    {
        offset = 8;
    }
    else if (direction == Direction::EW)
    {
        offset = 1;
    }
    else if (direction == Direction::SWNE)
    {
        offset = 9;
    }
    else if (direction == Direction::NWSE)
    {
        offset = 7;
    }

    return offset;
}

boost::optional<Piece> Position::getPieceAt(Square s) const
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
            return boost::none;
        }
    }
    else
    {
        return boost::none;
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

bool Position::isMoveValid(const Move& m)
{
    Piece p = m.getMovingPiece();
    std::vector<Square> kingLocationVector =
        findPiece(p.getColor(), Rank::KING);

    bool result = false;

    if (kingLocationVector.empty())
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
            Bitboard kingBitmask = kingLocation.toBitboard();
            Bitboard targetBitmask = m.getTargetSquare().toBitboard();

            if (
                m.getMovingPiece().getRank() == Rank::KING &&
                kingBitmask == 0x0000000000000010ULL &&
                targetBitmask == 0x0000000000000040ULL &&
                castlingStatus.getWhiteKingSide() &&
                !getPieceAt(Square(std::pair<int, int>(0, 5))) &&
                !getPieceAt(Square(std::pair<int, int>(0, 6))) &&
                !isSquareAttacked(kingLocation) &&
                !isSquareAttacked(Square(std::pair<int, int>(0, 5))) &&
                !isSquareAttacked(Square(std::pair<int, int>(0, 6)))
                )
            {
                result = true;
            }
            else if (
                m.getMovingPiece().getRank() == Rank::KING &&
                kingBitmask == 0x0000000000000010ULL &&
                targetBitmask == 0x0000000000000004ULL &&
                castlingStatus.getWhiteQueenSide() &&
                !getPieceAt(Square(std::pair<int, int>(0, 1))) &&
                !getPieceAt(Square(std::pair<int, int>(0, 2))) &&
                !getPieceAt(Square(std::pair<int, int>(0, 3))) &&
                !isSquareAttacked(kingLocation) &&
                !isSquareAttacked(Square(std::pair<int, int>(0, 3))) &&
                !isSquareAttacked(Square(std::pair<int, int>(0, 2)))
                )
            {
                result = true;
            }
            else if (
                m.getMovingPiece().getRank() == Rank::KING &&
                kingBitmask == 0x1000000000000000ULL &&
                targetBitmask == 0x4000000000000000ULL &&
                castlingStatus.getBlackKingSide() &&
                !getPieceAt(Square(std::pair<int, int>(7, 5))) &&
                !getPieceAt(Square(std::pair<int, int>(7, 6))) &&
                !isSquareAttacked(kingLocation) &&
                !isSquareAttacked(Square(std::pair<int, int>(7, 5))) &&
                !isSquareAttacked(Square(std::pair<int, int>(7, 6)))
                )
            {
                result = true;
            }
            else if (
                m.getMovingPiece().getRank() == Rank::KING &&
                kingBitmask == 0x1000000000000000ULL &&
                targetBitmask == 0x0400000000000000ULL &&
                castlingStatus.getBlackQueenSide() &&
                !getPieceAt(Square(std::pair<int, int>(7, 1))) &&
                !getPieceAt(Square(std::pair<int, int>(7, 2))) &&
                !getPieceAt(Square(std::pair<int, int>(7, 3))) &&
                !isSquareAttacked(kingLocation) &&
                !isSquareAttacked(Square(std::pair<int, int>(7, 3))) &&
                !isSquareAttacked(Square(std::pair<int, int>(7, 2)))
                )
            {
                result = true;
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

    blockerIndex = (LookupData::rayAttacksN[squareIndex] &
        (whitePieces | blackPieces)).lsb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksNE[squareIndex] &
        (whitePieces | blackPieces)).lsb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksE[squareIndex] &
        (whitePieces | blackPieces)).lsb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksSE[squareIndex] &
        (whitePieces | blackPieces)).msb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksS[squareIndex] &
        (whitePieces | blackPieces)).msb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksSW[squareIndex] &
        (whitePieces | blackPieces)).msb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingBishops | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksW[squareIndex] &
        (whitePieces | blackPieces)).msb();

    if (blockerIndex != -1)
    {
        if (((0x0000000000000001ULL << blockerIndex) &
            (opposingRooks | opposingQueens)) != 0x0000000000000000ULL)
        {
            result = true;
        }
    }

    blockerIndex = Bitboard(LookupData::rayAttacksNW[squareIndex] &
        (whitePieces | blackPieces)).lsb();

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
    return ((whitePieces | blackPieces) & s.toBitboard()) == 0x00;
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
    boost::optional<Color> currentColor {boost::none};
    boost::optional<Rank> currentRank {boost::none};

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

                if (currentColor && currentRank)
                {
                    addPieceAt(currentSquare, *currentColor, *currentRank);
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
        enPassantTarget = boost::none;
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
            enPassantTarget = boost::none;
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
}
