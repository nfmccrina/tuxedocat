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

namespace TuxedoCat
{
    public struct Move
    {

        public Move(ulong src, ulong tgt, PieceRank m, PieceColor pc, int hm, CastlingInfo ci, ulong ep, PieceRank? c = null, PieceRank? p = null)
        {
            SourceLocation = src;
            TargetLocation = tgt;

            MovingPiece = m;
            MoveColor = pc;
            CurrentHalfMoves = hm;
            CapturedPiece = c;
            PromotedRank = p;
            CurrentEnPassant = ep;
            CurrentCastlingInfo = ci;
        }

        public ulong SourceLocation
        {
            get; set;
        }

        public ulong TargetLocation
        {
            get; set;
        }

        public PieceRank MovingPiece
        {
            get; set;
        }

        public int CurrentHalfMoves
        {
            get; set;
        }

        public PieceColor MoveColor
        {
            get; set;
        }

        public PieceRank? CapturedPiece
        {
            get; set;
        }

        public PieceRank? PromotedRank
        {
            get; set;
        }

        public ulong CurrentEnPassant
        {
            get; set;
        }

        public CastlingInfo CurrentCastlingInfo
        {
            get; set;
        }
    }
}