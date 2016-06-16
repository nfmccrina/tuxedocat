using System;
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

        public void pushMove(UInt64 src, UInt64 tgt, PieceRank m, PieceColor pc, int hm, CastlingInfo ci, UInt64 ep, PieceRank? c, PieceRank? p)
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