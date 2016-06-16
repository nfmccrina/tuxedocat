using System;
using System.Diagnostics;

namespace TuxedoCat
{
    public struct Move
    {

        public Move(UInt64 src, UInt64 tgt, PieceRank m, PieceColor pc, int hm, CastlingInfo ci, UInt64 ep, PieceRank? c = null, PieceRank? p = null)
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

        public UInt64 SourceLocation
        {
            get; set;
        }

        public UInt64 TargetLocation
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

        public UInt64 CurrentEnPassant
        {
            get; set;
        }

        public CastlingInfo CurrentCastlingInfo
        {
            get; set;
        }
    }
}