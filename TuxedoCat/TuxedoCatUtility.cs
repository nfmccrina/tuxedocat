using System;

namespace TuxedoCat
{
    public class TuxedoCatUtility
    {
        public PieceRank? GetPieceRankFromChar(char piece)
        {
            PieceRank? rank;

            switch (piece.ToString().ToUpper())
            {
                case "P":
                    {
                        rank = PieceRank.PAWN;
                    }
                    break;
                case "N":
                    {
                        rank = PieceRank.KNIGHT;
                    }
                    break;
                case "B":
                    {
                        rank = PieceRank.BISHOP;
                    }
                    break;
                case "R":
                    {
                        rank = PieceRank.ROOK;
                    }
                    break;
                case "Q":
                    {
                        rank = PieceRank.QUEEN;
                    }
                    break;
                case "K":
                    {
                        rank = PieceRank.KING;
                    }
                    break;
                default:
                    {
                        rank = null;
                    }
                    break;
            }

            return rank;
        }

        public PieceColor GetPieceColorFromChar(char piece)
        {
            PieceColor color;

            if (char.IsUpper(piece))
            {
                color = PieceColor.WHITE;
            }
            else
            {
                color = PieceColor.BLACK;
            }

            return color;
        }

        public int GetSquareFromAlgebraic(string algebraic)
        {
            if (algebraic.Length != 2)
            {
                throw new ArgumentException("Invalid algebraic square notation; length was not two.");
            }

            int rank = 0;
            int file = (int)algebraic[0];

            if (file < 97 || file > 104)
            {
                throw new ArgumentException("Invalid algebraic square notation; could not parse file.");
            }

            file -= 97;

            if (char.IsDigit(algebraic[1]))
            {
                rank = Convert.ToInt32(algebraic[1].ToString()) - 1;
            }
            else
            {
                throw new ArgumentException("Invalid algebraic square notation; could not parse rank.");
            }

            return (rank * 8) + file;
        }
    }
}