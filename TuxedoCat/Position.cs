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

using System;
using System.Collections.Generic;

namespace TuxedoCat
{
    public class Position
    {
        public Position()
        {
            SetPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        }

        public Position(string fen)
        {
            SetPosition(fen);
        }

        public void SetPosition(string fen)
        {
            FENParser parser = new FENParser();
            List<ulong> pieceInfo;

            string[] fenParts = fen.Split(new char[] { ' ' });

            if (fenParts.Length != 6)
            {
                throw new ArgumentException("Invalid FEN; could not parse all sections.");
            }

            pieceInfo = parser.ReadPieceInfoFromFEN(fenParts[0]);
            colorToMove = parser.ReadColorToMoveFromFEN(fenParts[1]);
            castlingInfo = parser.ReadCastlingInfoFromFEN(fenParts[2]);
            enPassantTarget = parser.ReadEnPassantTargetFromFEN(fenParts[3]);
            halfMoveCounter = parser.ReadHalfMovesFromFEN(fenParts[4]);
            fullMoveCounter = parser.ReadFullMovesFromFEN(fenParts[5]);

            WhitePawns = pieceInfo[0];
            WhiteKnights = pieceInfo[1];
            WhiteBishops = pieceInfo[2];
            WhiteRooks = pieceInfo[3];
            WhiteQueens = pieceInfo[4];
            WhiteKing = pieceInfo[5];
            WhitePieces = pieceInfo[6];
            BlackPawns = pieceInfo[7];
            BlackKnights = pieceInfo[8];
            BlackBishops = pieceInfo[9];
            BlackRooks = pieceInfo[10];
            BlackQueens = pieceInfo[11];
            BlackKing = pieceInfo[12];
            BlackPieces = pieceInfo[13];
        }

        public override int GetHashCode()
        {
            int hash = 17;

            hash = (hash * 23) + WhitePieces.GetHashCode();
            hash = (hash * 23) + BlackPieces.GetHashCode();
            hash = (hash * 23) + castlingInfo.GetHashCode();
            hash = (hash * 23) + fullMoveCounter.GetHashCode();


            return hash;
        }

        public override bool Equals(object obj)
        {
            if (obj == null || GetType() != obj.GetType())
            {
                return false;
            }

            Position pos2 = (Position)obj;

            bool piecesAreEqual = true;

            piecesAreEqual = (WhitePawns == pos2.WhitePawns
                && WhiteKnights == pos2.WhiteKnights
                && WhiteBishops == pos2.WhiteBishops
                && WhiteRooks == pos2.WhiteRooks
                && WhiteQueens == pos2.WhiteQueens
                && WhiteKing == pos2.WhiteKing
                && WhitePieces == pos2.WhitePieces
                && BlackPawns == pos2.BlackPawns
                && BlackKnights == pos2.BlackKnights
                && BlackBishops == pos2.BlackBishops
                && BlackRooks == pos2.BlackRooks
                && BlackQueens == pos2.BlackQueens
                && BlackKing == pos2.BlackKing
                && BlackPieces == pos2.BlackPieces);

            return piecesAreEqual
                && (CastlingInfo == pos2.CastlingInfo)
                && (ColorToMove == pos2.ColorToMove)
                && (EnPassantTarget == pos2.EnPassantTarget)
                && (FullMoveCounter == pos2.FullMoveCounter)
                && (HalfMoveCounter == pos2.HalfMoveCounter);
        }

        public void Make(Move mv)
        {
            if (mv.SourceLocation == 0x0000000000000000UL)
            {
                if (colorToMove == PieceColor.WHITE)
                {
                    colorToMove = PieceColor.BLACK;
                }
                else
                {
                    colorToMove = PieceColor.WHITE;
                    fullMoveCounter++;
                }
            }
            else
            {
                enPassantTarget = 0x00000000000000UL;
                halfMoveCounter = mv.CurrentHalfMoves + 1;

                if (((mv.SourceLocation & WhiteKing) != 0x00000000000000UL
                    || (mv.SourceLocation & BlackKing) != 0x00000000000000UL)
                    && ((mv.SourceLocation == 0x0000000000000010UL && mv.TargetLocation == 0x0000000000000004UL)
                        || (mv.SourceLocation == 0x0000000000000010UL && mv.TargetLocation == 0x0000000000000040UL)
                        || (mv.SourceLocation == 0x1000000000000000UL && mv.TargetLocation == 0x0400000000000000UL)
                        || (mv.SourceLocation == 0x1000000000000000UL && mv.TargetLocation == 0x4000000000000000UL)))
                {
                    // castle

                    halfMoveCounter = 0;

                    if (mv.SourceLocation == 0x0000000000000010UL && mv.TargetLocation == 0x0000000000000004UL)
                    {
                        // white queen-side
                        AddPieceAt(0x0000000000000004UL, PieceColor.WHITE, PieceRank.KING);
                        AddPieceAt(0x0000000000000008UL, PieceColor.WHITE, PieceRank.ROOK);
                        RemovePieceAt(0x0000000000000010UL);
                        RemovePieceAt(0x0000000000000001UL);

                        castlingInfo = castlingInfo & (~CastlingInfo.WHITE_SHORT);
                        castlingInfo = castlingInfo & (~CastlingInfo.WHITE_LONG);
                    }
                    else if (mv.SourceLocation == 0x0000000000000010UL && mv.TargetLocation == 0x0000000000000040UL)
                    {
                        // white king-side
                        AddPieceAt(0x0000000000000040UL, PieceColor.WHITE, PieceRank.KING);
                        AddPieceAt(0x0000000000000020UL, PieceColor.WHITE, PieceRank.ROOK);
                        RemovePieceAt(0x0000000000000010UL);
                        RemovePieceAt(0x0000000000000080UL);

                        castlingInfo = castlingInfo & (~CastlingInfo.WHITE_SHORT);
                        castlingInfo = castlingInfo & (~CastlingInfo.WHITE_LONG);
                    }
                    else if (mv.SourceLocation == 0x1000000000000000UL && mv.TargetLocation == 0x0400000000000000UL)
                    {
                        // black queen-side
                        AddPieceAt(0x0400000000000000UL, PieceColor.BLACK, PieceRank.KING);
                        AddPieceAt(0x0800000000000000UL, PieceColor.BLACK, PieceRank.ROOK);
                        RemovePieceAt(0x1000000000000000UL);
                        RemovePieceAt(0x0100000000000000UL);

                        castlingInfo = castlingInfo & (~CastlingInfo.BLACK_SHORT);
                        castlingInfo = castlingInfo & (~CastlingInfo.BLACK_LONG);
                    }
                    else if (mv.SourceLocation == 0x1000000000000000UL && mv.TargetLocation == 0x4000000000000000UL)
                    {
                        // black king-side
                        AddPieceAt(0x4000000000000000UL, PieceColor.BLACK, PieceRank.KING);
                        AddPieceAt(0x2000000000000000UL, PieceColor.BLACK, PieceRank.ROOK);
                        RemovePieceAt(0x1000000000000000UL);
                        RemovePieceAt(0x8000000000000000UL);

                        castlingInfo = castlingInfo & (~CastlingInfo.BLACK_SHORT);
                        castlingInfo = castlingInfo & (~CastlingInfo.BLACK_LONG);
                    }
                }
                else
                {
                    // not a castle

                    if (mv.MovingPiece == PieceRank.PAWN)
                    {
                        // pawn move

                        halfMoveCounter = 0;

                        if (mv.CapturedPiece.HasValue)
                        {
                            if (mv.TargetLocation == mv.CurrentEnPassant)
                            {
                                if (mv.MoveColor == PieceColor.WHITE)
                                {
                                    RemovePieceAt((ulong)(mv.TargetLocation >> 8));
                                }
                                else
                                {
                                    RemovePieceAt((ulong)(mv.TargetLocation << 8));
                                }
                            }
                            else
                            {
                                if (mv.CapturedPiece == PieceRank.ROOK)
                                {
                                    if (mv.TargetLocation == 0x0000000000000001UL)
                                    {
                                        castlingInfo = castlingInfo & (~CastlingInfo.WHITE_LONG);
                                    }
                                    else if (mv.TargetLocation == 0x0000000000000080UL)
                                    {
                                        castlingInfo = castlingInfo & (~CastlingInfo.WHITE_SHORT);
                                    }
                                    else if (mv.TargetLocation == 0x0100000000000000UL)
                                    {
                                        castlingInfo = castlingInfo & (~CastlingInfo.BLACK_LONG);
                                    }
                                    else if (mv.TargetLocation == 0x8000000000000000UL)
                                    {
                                        castlingInfo = castlingInfo & (~CastlingInfo.BLACK_SHORT);
                                    }
                                }

                                RemovePieceAt(mv.TargetLocation);
                            }
                        }
                        else
                        {
                            if (mv.TargetLocation == (mv.SourceLocation << 16))
                            {
                                enPassantTarget = mv.SourceLocation << 8;
                            }
                            else if (mv.TargetLocation == (mv.SourceLocation >> 16))
                            {
                                enPassantTarget = mv.SourceLocation >> 8;
                            }
                        }

                        if (mv.PromotedRank.HasValue)
                        {
                            AddPieceAt(mv.TargetLocation, mv.MoveColor, mv.PromotedRank.Value);
                        }
                        else
                        {
                            AddPieceAt(mv.TargetLocation, mv.MoveColor, PieceRank.PAWN);
                        }

                        RemovePieceAt(mv.SourceLocation);
                    }
                    else
                    {
                        if (mv.MovingPiece == PieceRank.ROOK && mv.MoveColor == PieceColor.WHITE
                            && mv.SourceLocation == 0x0000000000000001UL)
                        {
                            castlingInfo = castlingInfo & (~CastlingInfo.WHITE_LONG);
                        }
                        else if (mv.MovingPiece == PieceRank.ROOK && mv.MoveColor == PieceColor.WHITE
                            && mv.SourceLocation == 0x0000000000000080UL)
                        {
                            castlingInfo = castlingInfo & (~CastlingInfo.WHITE_SHORT);
                        }
                        else if (mv.MovingPiece == PieceRank.ROOK && mv.MoveColor == PieceColor.BLACK
                            && mv.SourceLocation == 0x0100000000000000UL)
                        {
                            castlingInfo = castlingInfo & (~CastlingInfo.BLACK_LONG);
                        }
                        else if (mv.MovingPiece == PieceRank.ROOK && mv.MoveColor == PieceColor.BLACK
                            && mv.SourceLocation == 0x8000000000000000UL)
                        {
                            castlingInfo = castlingInfo & (~CastlingInfo.BLACK_SHORT);
                        }

                        if (mv.MovingPiece == PieceRank.KING && mv.MoveColor == PieceColor.WHITE)
                        {
                            castlingInfo = castlingInfo & (~CastlingInfo.WHITE_LONG);
                            castlingInfo = castlingInfo & (~CastlingInfo.WHITE_SHORT);
                        }
                        else if (mv.MovingPiece == PieceRank.KING && mv.MoveColor == PieceColor.BLACK)
                        {
                            castlingInfo = castlingInfo & (~CastlingInfo.BLACK_LONG);
                            castlingInfo = castlingInfo & (~CastlingInfo.BLACK_SHORT);
                        }

                        if (mv.CapturedPiece.HasValue)
                        {
                            if (mv.CapturedPiece == PieceRank.ROOK)
                            {
                                if (mv.TargetLocation == 0x0000000000000001UL)
                                {
                                    castlingInfo = castlingInfo & (~CastlingInfo.WHITE_LONG);
                                }
                                else if (mv.TargetLocation == 0x0000000000000080UL)
                                {
                                    castlingInfo = castlingInfo & (~CastlingInfo.WHITE_SHORT);
                                }
                                else if (mv.TargetLocation == 0x0100000000000000UL)
                                {
                                    castlingInfo = castlingInfo & (~CastlingInfo.BLACK_LONG);
                                }
                                else if (mv.TargetLocation == 0x8000000000000000UL)
                                {
                                    castlingInfo = castlingInfo & (~CastlingInfo.BLACK_SHORT);
                                }
                            }
                            // a capture!
                            RemovePieceAt(mv.TargetLocation); // he gone
                            halfMoveCounter = 0;
                        }

                        AddPieceAt(mv.TargetLocation, mv.MoveColor, mv.MovingPiece);
                        RemovePieceAt(mv.SourceLocation);
                    }
                }

                if (colorToMove == PieceColor.WHITE)
                {
                    colorToMove = PieceColor.BLACK;
                }
                else
                {
                    colorToMove = PieceColor.WHITE;
                    fullMoveCounter++;
                }
            }
        }

        public void Unmake(Move mv)
        {
            if (mv.SourceLocation == 0x0000000000000000UL)
            {
                if (colorToMove == PieceColor.WHITE)
                {
                    colorToMove = PieceColor.BLACK;
                    fullMoveCounter--;
                }
                else
                {
                    colorToMove = PieceColor.WHITE;
                }
            }
            else
            {
                AddPieceAt(mv.SourceLocation, mv.MoveColor, mv.MovingPiece);
                RemovePieceAt(mv.TargetLocation);

                if (((mv.SourceLocation & WhiteKing) != 0x0000000000000000UL
                    || (mv.SourceLocation & BlackKing) != 0x0000000000000000UL)
                    && ((mv.SourceLocation == 0x0000000000000010UL && mv.TargetLocation == 0x0000000000000004UL)
                        || (mv.SourceLocation == 0x0000000000000010UL && mv.TargetLocation == 0x0000000000000040UL)
                        || (mv.SourceLocation == 0x1000000000000000UL && mv.TargetLocation == 0x0400000000000000UL)
                        || (mv.SourceLocation == 0x1000000000000000UL && mv.TargetLocation == 0x4000000000000000UL)))
                {
                    ulong oldRookLocation = 0x0000000000000000UL;
                    ulong newRookLocation = 0x0000000000000000UL;

                    if (mv.SourceLocation == 0x0000000000000010UL && mv.TargetLocation == 0x0000000000000040UL)
                    {
                        oldRookLocation = 0x0000000000000020UL;
                        newRookLocation = 0x0000000000000080UL;
                    }
                    else if (mv.SourceLocation == 0x0000000000000010UL && mv.TargetLocation == 0x0000000000000004UL)
                    {
                        oldRookLocation = 0x0000000000000008UL;
                        newRookLocation = 0x0000000000000001UL;
                    }
                    else if (mv.SourceLocation == 0x1000000000000000UL && mv.TargetLocation == 0x4000000000000000UL)
                    {
                        oldRookLocation = 0x2000000000000000UL;
                        newRookLocation = 0x8000000000000000UL;
                    }
                    else if (mv.SourceLocation == 0x1000000000000000UL && mv.TargetLocation == 0x0400000000000000UL)
                    {
                        oldRookLocation = 0x0800000000000000UL;
                        newRookLocation = 0x0100000000000000UL;
                    }

                    RemovePieceAt(mv.TargetLocation);
                    RemovePieceAt(oldRookLocation);
                    AddPieceAt(mv.SourceLocation, mv.MoveColor, PieceRank.KING);
                    AddPieceAt(newRookLocation, mv.MoveColor, PieceRank.ROOK);
                }

                if (mv.CapturedPiece.HasValue)
                {
                    if (mv.CurrentEnPassant != 0x0000000000000000UL && mv.MovingPiece == PieceRank.PAWN
                        && mv.TargetLocation == mv.CurrentEnPassant)
                    {
                        if (mv.MoveColor == PieceColor.WHITE)
                        {
                            AddPieceAt(mv.TargetLocation >> 8, PieceColor.BLACK, mv.CapturedPiece.Value);
                        }
                        else
                        {
                            AddPieceAt(mv.TargetLocation << 8, PieceColor.WHITE, mv.CapturedPiece.Value);
                        }
                    }
                    else
                    {
                        AddPieceAt(mv.TargetLocation,
                            mv.MoveColor == PieceColor.WHITE ? PieceColor.BLACK : PieceColor.WHITE,
                            mv.CapturedPiece.Value);
                    }
                }

                colorToMove = mv.MoveColor;
                castlingInfo = mv.CurrentCastlingInfo;
                enPassantTarget = mv.CurrentEnPassant;
                halfMoveCounter = mv.CurrentHalfMoves;

                if (mv.MoveColor == PieceColor.BLACK)
                {
                    fullMoveCounter--;
                }
            }
        }

        public ulong WhitePawns
        {
            get; set;
        }

        public ulong WhiteKnights
        {
            get; set;
        }

        public ulong WhiteBishops
        {
            get; set;
        }

        public ulong WhiteRooks
        {
            get; set;
        }

        public ulong WhiteQueens
        {
            get; set;
        }

        public ulong WhiteKing
        {
            get; set;
        }

        public ulong WhitePieces
        {
            get; set;
        }


        public ulong BlackPawns
        {
            get; set;
        }

        public ulong BlackKnights
        {
            get; set;
        }

        public ulong BlackBishops
        {
            get; set;
        }

        public ulong BlackRooks
        {
            get; set;
        }

        public ulong BlackQueens
        {
            get; set;
        }

        public ulong BlackKing
        {
            get; set;
        }

        public ulong BlackPieces
        {
            get; set;
        }

        public CastlingInfo CastlingInfo
        {
            get
            {
                return castlingInfo;
            }
        }

        public PieceColor ColorToMove
        {
            get
            {
                return colorToMove;
            }
        }

        public ulong EnPassantTarget
        {
            get
            {
                return enPassantTarget;
            }
        }

        public int FullMoveCounter
        {
            get
            {
                return fullMoveCounter;
            }
        }

        public int HalfMoveCounter
        {
            get
            {
                return halfMoveCounter;
            }
        }

        public void AddPieceAt(ulong loc, PieceColor color, PieceRank rank)
        {
            if (rank == PieceRank.QUEEN)
            {
                if (color == PieceColor.WHITE)
                {
                    WhiteQueens = WhiteQueens | loc;
                }
                else
                {
                    BlackQueens = BlackQueens | loc;
                }
            }
            else if (rank == PieceRank.ROOK)
            {
                if (color == PieceColor.WHITE)
                {
                    WhiteRooks = WhiteRooks | loc;
                }
                else
                {
                    BlackRooks = BlackRooks | loc;
                }
            }
            else if (rank == PieceRank.BISHOP)
            {
                if (color == PieceColor.WHITE)
                {
                    WhiteBishops = WhiteBishops | loc;
                }
                else
                {
                    BlackBishops = BlackBishops | loc;
                }
            }
            else if (rank == PieceRank.KNIGHT)
            {
                if (color == PieceColor.WHITE)
                {
                    WhiteKnights = WhiteKnights | loc;
                }
                else
                {
                    BlackKnights = BlackKnights | loc;
                }
            }
            else if (rank == PieceRank.PAWN)
            {
                if (color == PieceColor.WHITE)
                {
                    WhitePawns = WhitePawns | loc;
                }
                else
                {
                    BlackPawns = BlackPawns | loc;
                }
            }
            else if (rank == PieceRank.KING)
            {
                if (color == PieceColor.WHITE)
                {
                    WhiteKing = loc;
                }
                else
                {
                    BlackKing = loc;
                }
            }

            UpdatePieces();
        }

        public void RemovePieceAt(ulong loc)
        {
            WhitePawns = WhitePawns & (~loc);
            WhiteKnights = WhiteKnights & (~loc);
            WhiteBishops = WhiteBishops & (~loc);
            WhiteRooks = WhiteRooks & (~loc);
            WhiteQueens = WhiteQueens & (~loc);
            WhiteKing = WhiteKing & (~loc);

            BlackPawns = BlackPawns & (~loc);
            BlackKnights = BlackKnights & (~loc);
            BlackBishops = BlackBishops & (~loc);
            BlackRooks = BlackRooks & (~loc);
            BlackQueens = BlackQueens & (~loc);
            BlackKing = BlackKing & (~loc);

            UpdatePieces();
        }

        public void UpdatePieces()
        {
            WhitePieces = (WhitePawns
                | WhiteKnights
                | WhiteBishops
                | WhiteRooks
                | WhiteQueens
                | WhiteKing);

            BlackPieces = (BlackPawns
                | BlackKnights
                | BlackBishops
                | BlackRooks
                | BlackQueens
                | BlackKing);
        }

        public PieceRank? GetRankAt(ulong loc)
        {
            PieceRank? rank = null;

            if ((WhitePawns & loc) != 0x0000000000000000UL
                || (BlackPawns & loc) != 0x0000000000000000UL)
            {
                rank = PieceRank.PAWN;
            }

            if ((WhiteKnights & loc) != 0x0000000000000000UL
                || (BlackKnights & loc) != 0x0000000000000000UL)
            {
                rank = PieceRank.KNIGHT;
            }

            if ((WhiteBishops & loc) != 0x0000000000000000UL
                || (BlackBishops & loc) != 0x0000000000000000UL)
            {
                rank = PieceRank.BISHOP;
            }

            if ((WhiteRooks & loc) != 0x0000000000000000UL
                || (BlackRooks & loc) != 0x0000000000000000UL)
            {
                rank = PieceRank.ROOK;
            }

            if ((WhiteQueens & loc) != 0x0000000000000000UL
                || (BlackQueens & loc) != 0x0000000000000000UL)
            {
                rank = PieceRank.QUEEN;
            }

            if ((WhiteKing & loc) != 0x0000000000000000UL
                || (BlackKing & loc) != 0x0000000000000000UL)
            {
                rank = PieceRank.KING;
            }

            return rank;
        }

        public PieceColor? GetColorAt(ulong loc)
        {
            PieceColor? color = null;

            if ((WhitePieces & loc) != 0x0000000000000000UL)
            {
                color = PieceColor.WHITE;
            }

            if ((BlackPieces & loc) != 0x0000000000000000UL)
            {
                color = PieceColor.BLACK;
            }

            return color;
        }

        private PieceColor colorToMove;
        private CastlingInfo castlingInfo;
        private ulong enPassantTarget;
        private int fullMoveCounter;
        private int halfMoveCounter;
    }
}