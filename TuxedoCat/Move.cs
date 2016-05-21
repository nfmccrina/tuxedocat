using System;
using System.Diagnostics;

namespace TuxedoCat
{
    public struct Move
    {
        private int sourceLocation;
        private int targetLocation;
        private Piece movingPiece;
        private Piece? capturedPiece;
        private PieceRank? promotedPiece;

        public Move(int src, int tgt, Piece m, Piece? c, PieceRank? p = null)
        {
            Debug.Assert(src >= 0 && src < 64);
            Debug.Assert(tgt >= 0 && tgt < 64);

            if (src >= 0 && src < 64)
            {
                this.sourceLocation = src;
            }
            else
            {
                this.sourceLocation = 0;
            }

            if (tgt >= 0 && tgt < 64)
            {
                this.targetLocation = tgt;
            }
            else
            {
                this.targetLocation = 0;
            }

            this.movingPiece = m;
            capturedPiece = c;
            promotedPiece = p;
        }

        public int SourceLocation
        {
            get
            {
                return this.sourceLocation;
            }
        }

        public int TargetLocation
        {
            get
            {
                return this.targetLocation;
            }
        }

        public Piece MovingPiece
        {
            get
            {
                return this.movingPiece;
            }
        }

        public Piece? CapturedPiece
        {
            get
            {
                return capturedPiece;
            }
        }

        public PieceRank? PromotedRank
        {
            get
            {
                return promotedPiece;
            }
        }
    }
}