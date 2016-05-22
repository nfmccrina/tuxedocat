using System.Diagnostics;

namespace TuxedoCat
{
    public struct Piece
    {
        private int location;
        private PieceRank rank;
        private PieceColor color;

        public Piece(int l, PieceRank r, PieceColor c)
        {
            Debug.Assert(l >= 0 && l < 64);

            location = l;
            rank = r;
            color = c;
        }

        public int Location
        {
            get
            {
                return location;
            }
        }

        public PieceRank Rank
        {
            get
            {
                return rank;
            }
        }

        public PieceColor Color
        {
            get
            {
                return color;
            }
        }
    }
}
