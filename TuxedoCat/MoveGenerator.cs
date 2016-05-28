using System.Collections.Generic;
using System.Linq;

namespace TuxedoCat
{
    public class MoveGenerator
    {
        public List<Move> KingMoves(Position pos, MoveGenerationType mgt = (MoveGenerationType.Captures | MoveGenerationType.NonCaptures))
        {
            List<Move> moves = new List<Move>();



            return moves;
        }

        public bool IsCapture(int loc, Position pos)
        {
            return pos.Pieces.Any(p => p.Location == loc && p.Color == (pos.ColorToMove == PieceColor.WHITE ? PieceColor.BLACK : PieceColor.WHITE));
        }
    }
}