using System;
using System.Linq;
using System.Collections.Generic;

namespace TuxedoCat
{
    public class SANParser
    {
        public string GenerateSAN(Position position, Move move)
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
                    if (Math.Abs((move.TargetLocation % 8) - (move.SourceLocation % 8)) > 1)
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
                    if (position.Pieces.Count(
                        piece => piece.Rank == move.MovingPiece && piece.Color == move.MoveColor) > 1)
                    {
                        List<Move> possibleMoves = mg.Moves(move.MovingPiece, position).Where(
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
                                possibleMove => ((possibleMove.SourceLocation / 8) + 1) == ((move.SourceLocation / 8) + 1)))
                            {
                                san += (move.SourceLocation / 8).ToString();
                            }
                            else
                            {
                                san += util.GetFileFromLocation(move.SourceLocation);
                                san += ((move.SourceLocation / 8) + 1).ToString();
                            }
                        }
                    }

                    if (move.CapturedPiece.HasValue)
                    {
                        san += "x";
                    }

                    san += util.GetFileFromLocation(move.TargetLocation);
                    san += ((move.TargetLocation / 8) + 1).ToString();
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
                san += ((move.TargetLocation / 8) + 1).ToString();

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
                if (move.TargetLocation == 6 || move.TargetLocation == 62)
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