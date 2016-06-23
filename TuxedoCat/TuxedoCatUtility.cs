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

 /*
  * LSB, MSB and PopCount code is from the chess programming wiki (chessprogramming.wikispaces.com)
  */

using System;

namespace TuxedoCat
{
    public class TuxedoCatUtility
    {
        /*
         * begin wiki code
         */

        private static ulong k1;
        private static ulong k2;
        private static ulong k4;
        private static ulong kf;

        private static int[] index64 = {
            0,  1, 48,  2, 57, 49, 28,  3,
           61, 58, 50, 42, 38, 29, 17,  4,
           62, 55, 59, 36, 53, 51, 43, 22,
           45, 39, 33, 30, 24, 18, 12,  5,
           63, 47, 56, 27, 60, 41, 37, 16,
           54, 35, 52, 21, 44, 32, 23, 11,
           46, 26, 40, 15, 34, 20, 31, 10,
           25, 14, 19,  9, 13,  8,  7,  6
        };

        private static int[] index64Reverse = {
            0, 47,  1, 56, 48, 27,  2, 60,
           57, 49, 41, 37, 28, 16,  3, 61,
           54, 58, 35, 52, 50, 42, 21, 44,
           38, 32, 29, 23, 17, 11,  4, 62,
           46, 55, 26, 59, 40, 36, 15, 53,
           34, 51, 20, 43, 31, 22, 10, 45,
           25, 39, 14, 33, 19, 30,  9, 24,
           13, 18,  8, 12,  7,  6,  5, 63
        };

        /*
         * end wiki code
         */

        public TuxedoCatUtility()
        {
            k1 = 0x5555555555555555UL;
            k2 = 0x3333333333333333UL;
            k4 = 0x0f0f0f0f0f0f0f0fUL;
            kf = 0x0101010101010101UL;
    }

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

        public ulong GetSquareFromAlgebraic(string algebraic)
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

        public string GetFileFromLocation(ulong location)
        {
            ulong mask = 0x0101010101010101UL;
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

        public int GetRankFromLocation(ulong location)
        {
            ulong mask = 0x00000000000000FFUL;
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

        public int GetLSB(ulong bitmask)
        {
            /*
             * begin wiki code
             */

            if (bitmask == 0x0000000000000000UL)
            {
                return -1;
            }

            ulong debruijn64 = 0x03F79D71B4CB0A89UL;
            return index64[((bitmask & ((~bitmask) + 1)) * debruijn64) >> 58];

            /*
             * end wiki code
             */
        }

        public int GetMSB(ulong bitmask)
        {
            /*
             * begin wiki code
             */

            if (bitmask == 0x0000000000000000UL)
            {
                return -1;
            }

            ulong debruijn64 = 0x03F79D71B4CB0A89UL;
            bitmask |= bitmask >> 1;
            bitmask |= bitmask >> 2;
            bitmask |= bitmask >> 4;
            bitmask |= bitmask >> 8;
            bitmask |= bitmask >> 16;
            bitmask |= bitmask >> 32;
            return index64Reverse[(bitmask * debruijn64) >> 58];

            /*
             * end wiki code
             */
        }

        public int PopCount(ulong bitmask)
        {
            /*
             * begin wiki code
             */

            ulong x = bitmask;

            x = x - ((x >> 1) & k1);
            x = (x & k2) + ((x >> 2) & k2);
            x = (x + (x >> 4)) & k4;
            x = (x * kf) >> 56;

            return (int)x;

            /*
             * end wiki code
             */
        }
    }
}