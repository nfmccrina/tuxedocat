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

using System.Linq;
using System.Collections.Generic;

namespace TuxedoCat
{
    public class SANParser
    {
        public string GenerateSAN(ref Position position, Move move, List<Move> allMoves)
        {
            string san = "";
            bool isCastle = false;
            MoveGenerator mg = new MoveGenerator();
            TuxedoCatUtility util = new TuxedoCatUtility();

            if (move.MovingPiece != PieceRank.PAWN)
            {
                if (move.MovingPiece == PieceRank.KNIGHT)
                {
                    san += "N";
                }
                else if (move.MovingPiece == PieceRank.BISHOP)
                {
                    san += "B";
                }
                else if (move.MovingPiece == PieceRank.ROOK)
                {
                    san += "R";
                }
                else if (move.MovingPiece == PieceRank.QUEEN)
                {
                    san += "Q";
                }
                else if (move.MovingPiece == PieceRank.KING)
                {
                    if ((move.SourceLocation == 0x0000000000000010UL && move.TargetLocation == 0x0000000000000004UL)
                        || (move.SourceLocation == 0x0000000000000010UL && move.TargetLocation == 0x0000000000000040UL)
                        || (move.SourceLocation == 0x1000000000000000UL && move.TargetLocation == 0x0400000000000000UL)
                        || (move.SourceLocation == 0x1000000000000000UL && move.TargetLocation == 0x4000000000000000UL))
                    {
                        isCastle = true;
                    }
                    else
                    {
                        san += "K";
                    }
                }

                if (!isCastle)
                {
                    ulong pieces = 0x0000000000000000UL;

                    if (move.MovingPiece == PieceRank.KNIGHT)
                    {
                        pieces =
                            move.MoveColor == PieceColor.WHITE ? position.WhiteKnights : position.BlackKnights;
                    }
                    else if (move.MovingPiece == PieceRank.BISHOP)
                    {
                        pieces =
                            move.MoveColor == PieceColor.WHITE ? position.WhiteBishops : position.BlackBishops;
                    }
                    else if (move.MovingPiece == PieceRank.ROOK)
                    {
                        pieces =
                            move.MoveColor == PieceColor.WHITE ? position.WhiteRooks : position.BlackRooks;
                    }
                    else if (move.MovingPiece == PieceRank.QUEEN)
                    {
                        pieces =
                            move.MoveColor == PieceColor.WHITE ? position.WhiteQueens : position.BlackQueens;
                    }


                    if (util.PopCount(pieces) > 1)
                    {
                        List<Move> possibleMoves = allMoves.Where(
                            possibleMove => possibleMove.MovingPiece == move.MovingPiece
                                && possibleMove.MoveColor == move.MoveColor
                                && possibleMove.TargetLocation == move.TargetLocation
                                && possibleMove.SourceLocation != move.SourceLocation).ToList();

                        if (possibleMoves.Count != 0)
                        {
                            if (!possibleMoves.Any(
                                possibleMove => util.GetFileFromLocation(possibleMove.SourceLocation)
                                    == util.GetFileFromLocation(move.SourceLocation)))
                            {
                                san += util.GetFileFromLocation(move.SourceLocation);
                            }
                            else if (!possibleMoves.Any(
                                possibleMove => util.GetRankFromLocation(possibleMove.SourceLocation) == util.GetRankFromLocation(move.SourceLocation)))
                            {
                                san += util.GetRankFromLocation(move.SourceLocation).ToString();
                            }
                            else
                            {
                                san += util.GetFileFromLocation(move.SourceLocation);
                                san += util.GetRankFromLocation(move.SourceLocation).ToString();
                            }
                        }
                    }

                    if (move.CapturedPiece.HasValue)
                    {
                        san += "x";
                    }

                    san += util.GetFileFromLocation(move.TargetLocation);
                    san += util.GetRankFromLocation(move.TargetLocation).ToString();
                }
            }
            else
            {
                if (move.CapturedPiece.HasValue)
                {
                    san += util.GetFileFromLocation(move.SourceLocation);
                    san += "x";
                }

                san += util.GetFileFromLocation(move.TargetLocation);
                san += util.GetRankFromLocation(move.TargetLocation).ToString();

                if (move.TargetLocation == position.EnPassantTarget
                    && move.CapturedPiece.HasValue)
                {
                    san += "e.p";
                }

                if (move.PromotedRank.HasValue)
                {
                    if (move.PromotedRank.Value == PieceRank.KNIGHT)
                    {
                        san += "N";
                    }
                    else if (move.PromotedRank.Value == PieceRank.BISHOP)
                    {
                        san += "B";
                    }
                    else if (move.PromotedRank.Value == PieceRank.ROOK)
                    {
                        san += "R";
                    }
                    else if (move.PromotedRank.Value == PieceRank.QUEEN)
                    {
                        san += "Q";
                    }
                }
            }

            if (isCastle)
            {
                if (move.TargetLocation == 0x0000000000000040UL || move.TargetLocation == 0x4000000000000000UL)
                {
                    san = "0-0";
                }
                else
                {
                    san = "0-0-0";
                }
            }

            return san;
        }
    }
}