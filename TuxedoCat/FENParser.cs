using System;
using System.Collections.Generic;

namespace TuxedoCat
{
    public class FENParser
    {
        public List<Piece> ReadPieceInfoFromFEN(string fen)
        {
            List<Piece> pieces = new List<Piece>();
            string[] rankInfo = fen.Split(new char[] { '/' });
            int currentSquare = 0;
            TuxedoCatUtility util = new TuxedoCatUtility();
            PieceRank? currentPieceRank;

            if (rankInfo.Length != 8)
            {
                throw new ArgumentException("Did not receive valid FEN; rankInfo length was not 8.");
            }

            for (int i = 0; i < 8; i++)
            {
                currentSquare = 8 * (7 - i);

                for (int j = 0; j < rankInfo[i].Length; j++)
                {
                    char currentCharacter = rankInfo[i][j];
                    if (char.IsDigit(currentCharacter))
                    {
                        currentSquare += Convert.ToInt32(currentCharacter.ToString());
                    }
                    else
                    {
                        currentPieceRank = util.GetPieceRankFromChar(currentCharacter);

                        if (currentPieceRank.HasValue)
                        {
                            pieces.Add(new Piece(currentSquare, currentPieceRank.Value, util.GetPieceColorFromChar(currentCharacter)));
                            currentSquare++;
                        }
                        else
                        {
                            throw new ArgumentException("Invalid character found in FEN string");
                        }
                    }
                }
            }

            return pieces;
        }

        public PieceColor ReadColorToMoveFromFEN(string fen)
        {
            PieceColor colorToMove;

            if (fen == "b")
            {
                colorToMove = PieceColor.BLACK;
            }
            else if (fen == "w")
            {
                colorToMove = PieceColor.WHITE;
            }
            else
            {
                throw new ArgumentException("Invalid FEN; could not parse color to move.");
            }

            return colorToMove;
        }

        public CastlingInfo ReadCastlingInfoFromFEN(string fen)
        {
            CastlingInfo info = CastlingInfo.NONE;

            if (fen.Contains("K"))
            {
                info = info | CastlingInfo.WHITE_SHORT;
            }

            if (fen.Contains("Q"))
            {
                info = info | CastlingInfo.WHITE_LONG;
            }

            if (fen.Contains("k"))
            {
                info = info | CastlingInfo.BLACK_SHORT;
            }

            if (fen.Contains("q"))
            {
                info = info | CastlingInfo.BLACK_LONG;
            }

            return info;
        }

        public int? ReadEnPassantTargetFromFEN(string fen)
        {
            TuxedoCatUtility util = new TuxedoCatUtility();
            int? enPassantTarget;

            if (fen == "-")
            {
                enPassantTarget = null;
            }
            else
            {
                enPassantTarget = util.GetSquareFromAlgebraic(fen);
            }

            return enPassantTarget;
        }

        public int ReadHalfMovesFromFEN(string fen)
        {
            int count = 0;

            if (!int.TryParse(fen, out count))
            {
                throw new ArgumentException("Invalid FEN; could not parse half moves.");
            }

            return count;
        }

        public int ReadFullMovesFromFEN(string fen)
        {
            int count = 0;

            if (!int.TryParse(fen, out count))
            {
                throw new ArgumentException("Invalid FEN; could not parse full moves.");
            }

            return count;
        }
    }
}