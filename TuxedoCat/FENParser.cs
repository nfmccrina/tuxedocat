using System;
using System.Collections.Generic;

namespace TuxedoCat
{
    public class FENParser
    {
        public List<UInt64> ReadPieceInfoFromFEN(string fen)
        {
            List<UInt64> pieces = new List<UInt64>();

            for (int i = 0; i < 14; i++)
            {
                pieces.Add(0x00);
            }

            string[] rankInfo = fen.Split(new char[] { '/' });
            UInt64 currentSquare = 0x0000000000000000UL;
            TuxedoCatUtility util = new TuxedoCatUtility();
            PieceRank? currentPieceRank;

            if (rankInfo.Length != 8)
            {
                throw new ArgumentException("Did not receive valid FEN; rankInfo length was not 8.");
            }

            for (int i = 0; i < 8; i++)
            {
                currentSquare = 0x0000000000000001UL << (8 * (7 - i));

                for (int j = 0; j < rankInfo[i].Length; j++)
                {
                    char currentCharacter = rankInfo[i][j];
                    if (char.IsDigit(currentCharacter))
                    {
                        currentSquare = currentSquare << Convert.ToInt32(currentCharacter.ToString());
                    }
                    else
                    {
                        currentPieceRank = util.GetPieceRankFromChar(currentCharacter);

                        if (currentPieceRank.HasValue)
                        {
                            switch (currentPieceRank.Value)
                            {
                                case PieceRank.PAWN:
                                    {
                                        if (util.GetPieceColorFromChar(currentCharacter) == PieceColor.WHITE)
                                        {
                                            pieces[0] = pieces[0] | currentSquare;
                                            pieces[6] = pieces[6] | currentSquare;
                                        }
                                        else
                                        {
                                            pieces[7] = pieces[7] | currentSquare;
                                            pieces[13] = pieces[13] | currentSquare;
                                        }
                                    }
                                    break;
                                case PieceRank.KNIGHT:
                                    {
                                        if (util.GetPieceColorFromChar(currentCharacter) == PieceColor.WHITE)
                                        {
                                            pieces[1] = pieces[1] | currentSquare;
                                            pieces[6] = pieces[6] | currentSquare;
                                        }
                                        else
                                        {
                                            pieces[8] = pieces[8] | currentSquare;
                                            pieces[13] = pieces[13] | currentSquare;
                                        }
                                    }
                                    break;
                                case PieceRank.BISHOP:
                                    {
                                        if (util.GetPieceColorFromChar(currentCharacter) == PieceColor.WHITE)
                                        {
                                            pieces[2] = pieces[2] | currentSquare;
                                            pieces[6] = pieces[6] | currentSquare;
                                        }
                                        else
                                        {
                                            pieces[9] = pieces[9] | currentSquare;
                                            pieces[13] = pieces[13] | currentSquare;
                                        }
                                    }
                                    break;
                                case PieceRank.ROOK:
                                    {
                                        if (util.GetPieceColorFromChar(currentCharacter) == PieceColor.WHITE)
                                        {
                                            pieces[3] = pieces[3] | currentSquare;
                                            pieces[6] = pieces[6] | currentSquare;
                                        }
                                        else
                                        {
                                            pieces[10] = pieces[10] | currentSquare;
                                            pieces[13] = pieces[13] | currentSquare;
                                        }
                                    }
                                    break;
                                case PieceRank.QUEEN:
                                    {
                                        if (util.GetPieceColorFromChar(currentCharacter) == PieceColor.WHITE)
                                        {
                                            pieces[4] = pieces[4] | currentSquare;
                                            pieces[6] = pieces[6] | currentSquare;
                                        }
                                        else
                                        {
                                            pieces[11] = pieces[11] | currentSquare;
                                            pieces[13] = pieces[13] | currentSquare;
                                        }
                                    }
                                    break;
                                case PieceRank.KING:
                                    {
                                        if (util.GetPieceColorFromChar(currentCharacter) == PieceColor.WHITE)
                                        {
                                            pieces[5] = pieces[5] | currentSquare;
                                            pieces[6] = pieces[6] | currentSquare;
                                        }
                                        else
                                        {
                                            pieces[12] = pieces[12] | currentSquare;
                                            pieces[13] = pieces[13] | currentSquare;
                                        }
                                    }
                                    break;
                            }
                            currentSquare = currentSquare << 1;
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

        public UInt64 ReadEnPassantTargetFromFEN(string fen)
        {
            TuxedoCatUtility util = new TuxedoCatUtility();
            UInt64 enPassantTarget;

            if (fen == "-")
            {
                enPassantTarget = 0x0000000000000000UL;
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