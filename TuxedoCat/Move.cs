using System;
using System.Diagnostics;

namespace TuxedoCat
{
    public struct Move
    {
        private int sourceLocation;
        private int targetLocation;
        private PieceRank movingPiece;
        private PieceColor moveColor;
        private int currentHalfMoves;
        private PieceRank? capturedPiece;
        private PieceRank? promotedPiece;
        private int? currentEnPassant;
        private CastlingInfo currentCastlingInfo;

        public Move(int src, int tgt, PieceRank m, PieceColor pc, int hm, CastlingInfo ci, PieceRank? c = null, PieceRank? p = null, int? ep = null)
        {
            Debug.Assert(src >= 0 && src < 64);
            Debug.Assert(tgt >= 0 && tgt < 64);

            if (src >= 0 && src < 64)
            {
                sourceLocation = src;
            }
            else
            {
                sourceLocation = 0;
            }

            if (tgt >= 0 && tgt < 64)
            {
                targetLocation = tgt;
            }
            else
            {
                targetLocation = 0;
            }

            movingPiece = m;
            moveColor = pc;
            currentHalfMoves = hm;
            capturedPiece = c;
            promotedPiece = p;
            currentEnPassant = ep;
            currentCastlingInfo = ci;
        }

        public int SourceLocation
        {
            get
            {
                return sourceLocation;
            }
        }

        public int TargetLocation
        {
            get
            {
                return targetLocation;
            }
        }

        public PieceRank MovingPiece
        {
            get
            {
                return movingPiece;
            }
        }

        public int CurrentHalfMoves
        {
            get
            {
                return currentHalfMoves;
            }
        }

        public PieceColor MoveColor
        {
            get
            {
                return moveColor;
            }
        }

        public PieceRank? CapturedPiece
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

        public int? CurrentEnPassant
        {
            get
            {
                return currentEnPassant;
            }
        }

        public CastlingInfo CurrentCastlingInfo
        {
            get
            {
                return currentCastlingInfo;
            }
        }
    }
}