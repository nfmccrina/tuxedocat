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

namespace TuxedoCat
{
    public class MoveList
    {
        public MoveList()
        {
            moves = new Move[250];
            count = 0;
        }

        public int Count
        {
            get
            {
                return count;
            }
        }

        public void pushMove(ulong src, ulong tgt, PieceRank m, PieceColor pc, int hm, CastlingInfo ci, ulong ep, PieceRank? c, PieceRank? p)
        {
            if (count < 250)
            {
                moves[count].SourceLocation = src;
                moves[count].TargetLocation = tgt;
                moves[count].CapturedPiece = c;
                moves[count].CurrentCastlingInfo = ci;
                moves[count].CurrentEnPassant = ep;
                moves[count].CurrentHalfMoves = hm;
                moves[count].MoveColor = pc;
                moves[count].MovingPiece = m;
                moves[count].PromotedRank = p;

                count++;
            }
        }

        public Move getMoveAt(int index)
        {
            return moves[index];
        }

        public Move[] getSlice(int num)
        {
            return moves.Take(num).ToArray();
        }

        public void clear()
        {
            count = 0;
        }

        private int count;
        private Move[] moves;
    }
}