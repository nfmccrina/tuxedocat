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

        public UInt64 GetSquareFromAlgebraic(string algebraic)
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

            return 0x0000000000000001UL << ((rank * 8) + file);
        }

        public string GetFileFromLocation(UInt64 location)
        {
            UInt64 mask = 0x0101010101010101UL;
            char file = ' ';

            for (int index = 0; index < 8; index++)
            {
                if ((mask & location) != 0x0000000000000000UL)
                {
                    file = (char)(index + 97);
                    break;
                }

                mask = mask << 1;
            }

            return file.ToString();
        }

        public int GetRankFromLocation(UInt64 location)
        {
            UInt64 mask = 0x00000000000000FFUL;
            int index = 0;

            for (index = 1; index < 9; index++)
            {
                if ((mask & location) != 0x0000000000000000UL)
                {
                    break;
                }

                mask = mask << 8;
            }

            return index;
        }

        public int GetLSB(UInt64 bitmask)
        {
            int index = 0;
            UInt64 currentBit = 0x0000000000000001UL;

            for (; index < 64; index++)
            {
                if ((bitmask & currentBit) != 0x0000000000000000UL)
                {
                    break;
                }

                currentBit = currentBit << 1;
            }

            if (index == 64)
            {
                index = -1;
            }

            return index;
        }

        public int GetMSB(UInt64 bitmask)
        {
            int index = 63;
            UInt64 currentBit = 0x8000000000000000UL;

            for (; index >= 0; index--)
            {
                if ((bitmask & currentBit) != 0x0000000000000000UL)
                {
                    break;
                }

                currentBit = currentBit >> 1;
            }

            return index;
        }

        public int PopCount(UInt64 bitmask)
        {
            int count = 0;
            
            for (UInt64 currentBit = 0x0000000000000001UL; currentBit != 0x0000000000000000UL;
                currentBit = currentBit << 1)
            {
                if ((bitmask & currentBit) != 0x0000000000000000UL)
                {
                    count++;
                }
            }

            return count;
        }
    }
}