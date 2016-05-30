using System;
using System.Collections.Generic;
using System.Linq;

namespace TuxedoCat
{
    public class MoveGenerator
    {
        public int Perft(int depth, Position position)
        {
            if (depth <= 1)
            {
                return AllLegalMoves(position).Count;
            }
            else
            {
                int count = 0;

                foreach (Move move in AllLegalMoves(position))
                {
                    position.Make(move);
                    count += Perft(depth - 1, position);
                    position.Unmake(move);
                }

                return count;
            }
        }

        public void Divide(int depth, Position position)
        {
            SANParser sanParser = new SANParser();

            if (depth <= 1)
            {
                foreach(Move move in AllLegalMoves(position))
                {
                    string moveSAN = sanParser.GenerateSAN(position, move);

                    Console.WriteLine(moveSAN + ": 1");
                }
            }
            else
            {
                foreach (Move move in AllLegalMoves(position))
                {
                    string moveSAN = sanParser.GenerateSAN(position, move);

                    position.Make(move);

                    int count = Perft(depth - 1, position);

                    Console.WriteLine(moveSAN + ": " + count.ToString());
                    position.Unmake(move);
                }
            }
        }

        public bool IsLegalMove(Move move, Position position)
        {
            bool result;

            position.Make(move);

            result = !IsSquareAttacked(position.Pieces.First(
                piece => piece.Rank == PieceRank.KING && piece.Color == move.MoveColor).Location, position,
                position.ColorToMove);

            position.Unmake(move);

            return result;
        }

        public List<Move> AllMoves(Position position,
            MoveGenerationType mgt = (MoveGenerationType.Captures | MoveGenerationType.NonCaptures))
        {
            List<Move> moves = new List<Move>();

            moves.AddRange(PawnMoves(position, mgt));
            moves.AddRange(KnightMoves(position, mgt));
            moves.AddRange(BishopMoves(position, mgt));
            moves.AddRange(RookMoves(position, mgt));
            moves.AddRange(QueenMoves(position, mgt));
            moves.AddRange(KingMoves(position, mgt));

            return moves;
        }

        public List<Move> AllLegalMoves(Position position,
            MoveGenerationType mgt = (MoveGenerationType.Captures | MoveGenerationType.NonCaptures))
        {
            return AllMoves(position, mgt).Where(move => IsLegalMove(move, position)).ToList();
        }

        public List<Move> Moves(PieceRank rank, Position pos,
            MoveGenerationType mgt = (MoveGenerationType.Captures | MoveGenerationType.NonCaptures))
        {
            List<Move> moves;

            switch (rank)
            {
                case PieceRank.PAWN:
                    {
                        moves = PawnMoves(pos, mgt);
                    }
                    break;
                case PieceRank.KNIGHT:
                    {
                        moves = KnightMoves(pos, mgt);
                    }
                    break;
                case PieceRank.BISHOP:
                    {
                        moves = BishopMoves(pos, mgt);
                    }
                    break;
                case PieceRank.ROOK:
                    {
                        moves = RookMoves(pos, mgt);
                    }
                    break;
                case PieceRank.QUEEN:
                    {
                        moves = QueenMoves(pos, mgt);
                    }
                    break;
                case PieceRank.KING:
                    {
                        moves = KingMoves(pos, mgt);
                    }
                    break;
                default:
                    {
                        moves = new List<Move>();
                    }
                    break;
            }

            return moves;
        }

        public List<Move> KingMoves(Position pos,
            MoveGenerationType mgt = (MoveGenerationType.Captures | MoveGenerationType.NonCaptures))
        {
            List<int> targets = new List<int>();
            List<Move> moves = new List<Move>();

            int currentKingLocation = pos.Pieces.First(p => p.Color == pos.ColorToMove && p.Rank == PieceRank.KING)
                .Location;

            if ((currentKingLocation % 8) != 0 && currentKingLocation < 56)
            {
                targets.Add(currentKingLocation + 7);
            }

            if (currentKingLocation < 56)
            {
                targets.Add(currentKingLocation + 8);
            }

            if ((currentKingLocation % 8) != 7 && currentKingLocation < 56)
            {
                targets.Add(currentKingLocation + 9);
            }

            if ((currentKingLocation % 8) != 7)
            {
                targets.Add(currentKingLocation + 1);
            }

            if ((currentKingLocation % 8) != 7 && currentKingLocation > 7)
            {
                targets.Add(currentKingLocation - 7);
            }

            if (currentKingLocation > 7)
            {
                targets.Add(currentKingLocation - 8);
            }

            if ((currentKingLocation % 8) != 0 && currentKingLocation > 7)
            {
                targets.Add(currentKingLocation - 9);
            }

            if ((currentKingLocation % 8) != 0)
            {
                targets.Add(currentKingLocation - 1);
            }

            if (mgt.HasFlag(MoveGenerationType.NonCaptures))
            {
                if (pos.ColorToMove == PieceColor.WHITE
                    && pos.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT)
                    && pos.Pieces.All(p => p.Location != 5)
                    && pos.Pieces.All(p => p.Location != 6)
                    && !IsSquareAttacked(4, pos,
                        pos.ColorToMove == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK)
                    && !IsSquareAttacked(5, pos,
                        pos.ColorToMove == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK)
                    && !IsSquareAttacked(6, pos,
                        pos.ColorToMove == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK))
                {
                    moves.Add(new Move(4, 6, PieceRank.KING, pos.ColorToMove, pos.HalfMoveCounter, pos.CastlingInfo, null, null, pos.EnPassantTarget));
                }

                if (pos.ColorToMove == PieceColor.WHITE
                    && pos.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG)
                    && pos.Pieces.All(p => p.Location != 1)
                    && pos.Pieces.All(p => p.Location != 2)
                    && pos.Pieces.All(p => p.Location != 3)
                    && !IsSquareAttacked(4, pos,
                        pos.ColorToMove == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK)
                    && !IsSquareAttacked(3, pos,
                        pos.ColorToMove == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK)
                    && !IsSquareAttacked(2, pos,
                        pos.ColorToMove == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK))
                {
                    moves.Add(new Move(4, 2, PieceRank.KING, pos.ColorToMove, pos.HalfMoveCounter, pos.CastlingInfo, null, null, pos.EnPassantTarget));
                }

                if (pos.ColorToMove == PieceColor.BLACK
                    && pos.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG)
                    && pos.Pieces.All(p => p.Location != 57)
                    && pos.Pieces.All(p => p.Location != 58)
                    && pos.Pieces.All(p => p.Location != 59)
                    && !IsSquareAttacked(60, pos,
                        pos.ColorToMove == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK)
                    && !IsSquareAttacked(59, pos,
                        pos.ColorToMove == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK)
                    && !IsSquareAttacked(58, pos,
                        pos.ColorToMove == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK))
                {
                    moves.Add(new Move(60, 58, PieceRank.KING, pos.ColorToMove, pos.HalfMoveCounter, pos.CastlingInfo, null, null, pos.EnPassantTarget));
                }

                if (pos.ColorToMove == PieceColor.BLACK
                    && pos.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT)
                    && pos.Pieces.All(p => p.Location != 61)
                    && pos.Pieces.All(p => p.Location != 62)
                    && !IsSquareAttacked(60, pos,
                        pos.ColorToMove == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK)
                    && !IsSquareAttacked(61, pos,
                        pos.ColorToMove == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK)
                    && !IsSquareAttacked(62, pos,
                        pos.ColorToMove == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK))
                {
                    moves.Add(new Move(4, 6, PieceRank.KING, pos.ColorToMove, pos.HalfMoveCounter, pos.CastlingInfo, null, null, pos.EnPassantTarget));
                }
            }

            foreach (int l in targets)
            {
                if (pos.Pieces.Any(p => p.Location == l && p.Color != pos.ColorToMove)
                    && mgt.HasFlag(MoveGenerationType.Captures))
                {
                    moves.Add(new Move(currentKingLocation, l, PieceRank.KING, pos.ColorToMove,
                        pos.HalfMoveCounter, pos.CastlingInfo, pos.Pieces.First(p => p.Location == l).Rank, null, pos.EnPassantTarget));
                }
                else if (pos.Pieces.All(p => p.Location != l) && mgt.HasFlag(MoveGenerationType.NonCaptures))
                {
                    moves.Add(new Move(currentKingLocation, l, PieceRank.KING, pos.ColorToMove, pos.HalfMoveCounter,
                        pos.CastlingInfo, null, null, pos.EnPassantTarget));
                }
            }

            return moves;
        }

        public List<Move> KnightMoves(Position pos, MoveGenerationType mgt = (MoveGenerationType.Captures | MoveGenerationType.NonCaptures))
        {
            List<Move> moves = new List<Move>();

            foreach (Piece knight in pos.Pieces.Where(p => p.Rank == PieceRank.KNIGHT && p.Color == pos.ColorToMove))
            {
                List<int> targets = new List<int>();

                if (knight.Location < 56 && (knight.Location % 8) > 1)
                {
                    targets.Add(knight.Location + 6);
                }

                if (knight.Location < 48 && (knight.Location % 8) > 0)
                {
                    targets.Add(knight.Location + 15);
                }

                if (knight.Location < 48 && (knight.Location % 8) < 7)
                {
                    targets.Add(knight.Location + 17);
                }

                if (knight.Location < 56 && (knight.Location % 8) < 6)
                {
                    targets.Add(knight.Location + 10);
                }

                if (knight.Location > 7 && (knight.Location % 8) < 6)
                {
                    targets.Add(knight.Location - 6);
                }

                if (knight.Location > 15 && (knight.Location % 8) < 7)
                {
                    targets.Add(knight.Location - 15);
                }

                if (knight.Location > 15 && (knight.Location % 8) > 0)
                {
                    targets.Add(knight.Location - 17);
                }

                if (knight.Location > 7 && (knight.Location % 8) > 1)
                {
                    targets.Add(knight.Location - 10);
                }

                foreach (int target in targets)
                {
                    if (pos.Pieces.Any(p => p.Location == target && p.Color != pos.ColorToMove)
                    && mgt.HasFlag(MoveGenerationType.Captures))
                    {
                        moves.Add(new Move(knight.Location, target, PieceRank.KNIGHT, pos.ColorToMove,
                            pos.HalfMoveCounter, pos.CastlingInfo,
                            pos.Pieces.First(p => p.Location == target).Rank, null, pos.EnPassantTarget));
                    }
                    else if (pos.Pieces.All(p => p.Location != target) && mgt.HasFlag(MoveGenerationType.NonCaptures))
                    {
                        moves.Add(new Move(knight.Location, target, PieceRank.KNIGHT, pos.ColorToMove, pos.HalfMoveCounter,
                            pos.CastlingInfo, null, null, pos.EnPassantTarget));
                    }
                }
            }

            return moves;
        }

        public List<Move> BishopMoves(Position pos, MoveGenerationType mgt = (MoveGenerationType.Captures | MoveGenerationType.NonCaptures))
        {
            List<Move> moves = new List<Move>();
            int tmp_loc;

            foreach (Piece bishop in pos.Pieces.Where(p => p.Rank == PieceRank.BISHOP && p.Color == pos.ColorToMove))
            {
                List<int> targets = new List<int>();
                tmp_loc = bishop.Location;

                while (tmp_loc < 56 && (tmp_loc % 8) != 7)
                {
                    tmp_loc += 9;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = bishop.Location;

                while (tmp_loc < 56 && (tmp_loc % 8) != 0)
                {
                    tmp_loc += 7;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = bishop.Location;

                while (tmp_loc >7 && (tmp_loc % 8) != 0)
                {
                    tmp_loc -= 9;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = bishop.Location;

                while (tmp_loc > 7 && (tmp_loc % 8) != 7)
                {
                    tmp_loc -= 7;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                foreach (int target in targets)
                {
                    if (pos.Pieces.Any(p => p.Location == target && p.Color != pos.ColorToMove)
                    && mgt.HasFlag(MoveGenerationType.Captures))
                    {
                        moves.Add(new Move(bishop.Location, target, PieceRank.BISHOP, pos.ColorToMove,
                            pos.HalfMoveCounter, pos.CastlingInfo,
                            pos.Pieces.First(p => p.Location == target).Rank, null, pos.EnPassantTarget));
                    }
                    else if (pos.Pieces.All(p => p.Location != target) && mgt.HasFlag(MoveGenerationType.NonCaptures))
                    {
                        moves.Add(new Move(bishop.Location, target, PieceRank.BISHOP, pos.ColorToMove, pos.HalfMoveCounter,
                            pos.CastlingInfo, null, null, pos.EnPassantTarget));
                    }
                }
            }

            return moves;
        }

        public List<Move> RookMoves(Position pos, MoveGenerationType mgt = (MoveGenerationType.Captures | MoveGenerationType.NonCaptures))
        {
            List<Move> moves = new List<Move>();
            int tmp_loc;

            foreach (Piece rook in pos.Pieces.Where(p => p.Rank == PieceRank.ROOK && p.Color == pos.ColorToMove))
            {
                List<int> targets = new List<int>();
                tmp_loc = rook.Location;

                while (tmp_loc < 56)
                {
                    tmp_loc += 8;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = rook.Location;

                while ((tmp_loc % 8) != 7)
                {
                    tmp_loc += 1;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = rook.Location;

                while ((tmp_loc % 8) != 0)
                {
                    tmp_loc -= 1;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = rook.Location;

                while (tmp_loc > 7)
                {
                    tmp_loc -= 8;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = rook.Location;

                foreach (int target in targets)
                {
                    if (pos.Pieces.Any(p => p.Location == target && p.Color != pos.ColorToMove)
                    && mgt.HasFlag(MoveGenerationType.Captures))
                    {
                        moves.Add(new Move(rook.Location, target, PieceRank.ROOK, pos.ColorToMove,
                            pos.HalfMoveCounter, pos.CastlingInfo,
                            pos.Pieces.First(p => p.Location == target).Rank, null, pos.EnPassantTarget));
                    }
                    else if (pos.Pieces.All(p => p.Location != target) && mgt.HasFlag(MoveGenerationType.NonCaptures))
                    {
                        moves.Add(new Move(rook.Location, target, PieceRank.ROOK, pos.ColorToMove, pos.HalfMoveCounter,
                            pos.CastlingInfo, null, null, pos.EnPassantTarget));
                    }
                }
            }

            return moves;
        }

        public List<Move> QueenMoves(Position pos, MoveGenerationType mgt = (MoveGenerationType.Captures | MoveGenerationType.NonCaptures))
        {
            List<Move> moves = new List<Move>();
            int tmp_loc;

            foreach (Piece queen in pos.Pieces.Where(p => p.Rank == PieceRank.QUEEN && p.Color == pos.ColorToMove))
            {
                List<int> targets = new List<int>();
                tmp_loc = queen.Location;

                while (tmp_loc < 56 && (tmp_loc % 8) != 7)
                {
                    tmp_loc += 9;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = queen.Location;

                while (tmp_loc < 56 && (tmp_loc % 8) != 0)
                {
                    tmp_loc += 7;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = queen.Location;

                while (tmp_loc > 7 && (tmp_loc % 8) != 0)
                {
                    tmp_loc -= 9;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = queen.Location;

                while (tmp_loc > 7 && (tmp_loc % 8) != 7)
                {
                    tmp_loc -= 7;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = queen.Location;

                while (tmp_loc < 56)
                {
                    tmp_loc += 8;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = queen.Location;

                while ((tmp_loc % 8) != 7)
                {
                    tmp_loc += 1;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = queen.Location;

                while ((tmp_loc % 8) != 0)
                {
                    tmp_loc -= 1;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = queen.Location;

                while (tmp_loc > 7)
                {
                    tmp_loc -= 8;

                    if (pos.Pieces.Any(p => p.Location == tmp_loc))
                    {
                        if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color != pos.ColorToMove))
                        {
                            targets.Add(tmp_loc);
                        }

                        break;
                    }

                    targets.Add(tmp_loc);
                }

                tmp_loc = queen.Location;

                foreach (int target in targets)
                {
                    if (pos.Pieces.Any(p => p.Location == target && p.Color != pos.ColorToMove)
                    && mgt.HasFlag(MoveGenerationType.Captures))
                    {
                        moves.Add(new Move(queen.Location, target, PieceRank.QUEEN, pos.ColorToMove,
                            pos.HalfMoveCounter, pos.CastlingInfo,
                            pos.Pieces.First(p => p.Location == target).Rank, null, pos.EnPassantTarget));
                    }
                    else if (pos.Pieces.All(p => p.Location != target) && mgt.HasFlag(MoveGenerationType.NonCaptures))
                    {
                        moves.Add(new Move(queen.Location, target, PieceRank.QUEEN, pos.ColorToMove, pos.HalfMoveCounter,
                            pos.CastlingInfo, null, null, pos.EnPassantTarget));
                    }
                }
            }

            return moves;
        }

        public List<Move> PawnMoves(Position pos, MoveGenerationType mgt = (MoveGenerationType.Captures | MoveGenerationType.NonCaptures))
        {
            List<Move> moves = new List<Move>();

            foreach (Piece pawn in pos.Pieces.Where(piece => piece.Color == pos.ColorToMove
                && piece.Rank == PieceRank.PAWN))
            {
                List<int> targets = new List<int>();

                if (pos.ColorToMove == PieceColor.WHITE)
                {
                    if (pawn.Location < 56 && !pos.Pieces.Any(piece => piece.Location == (pawn.Location + 8)))
                    {
                        targets.Add(pawn.Location + 8);
                    }

                    if (pawn.Location > 7
                        && pawn.Location < 16
                        && !pos.Pieces.Any(piece => piece.Location == (pawn.Location + 8))
                        && !pos.Pieces.Any(piece => piece.Location == (pawn.Location + 16)))
                    {
                        targets.Add(pawn.Location + 16);
                    }

                    if (pawn.Location < 56
                        && (pawn.Location % 8) != 0
                        && (pos.Pieces.Any(piece => piece.Color != pos.ColorToMove
                            && piece.Location == (pawn.Location + 7))
                        || pos.EnPassantTarget == (pawn.Location + 7)))
                    {
                        targets.Add(pawn.Location + 7);
                    }

                    if (pawn.Location < 56
                        && (pawn.Location % 8) != 7
                        && (pos.Pieces.Any(piece => piece.Color != pos.ColorToMove
                            && piece.Location == (pawn.Location + 9))
                        || pos.EnPassantTarget == (pawn.Location + 9)))
                    {
                        targets.Add(pawn.Location + 9);
                    }
                }
                else
                {
                    if (pawn.Location > 7 && !pos.Pieces.Any(piece => piece.Location == (pawn.Location - 8)))
                    {
                        targets.Add(pawn.Location - 8);
                    }

                    if (pawn.Location > 47
                        && pawn.Location < 56
                        && !pos.Pieces.Any(piece => piece.Location == (pawn.Location - 8))
                        && !pos.Pieces.Any(piece => piece.Location == (pawn.Location - 16)))
                    {
                        targets.Add(pawn.Location - 16);
                    }

                    if (pawn.Location > 7
                        && (pawn.Location % 8) != 0
                        && (pos.Pieces.Any(piece => piece.Color != pos.ColorToMove
                            && piece.Location == (pawn.Location - 9))
                        || pos.EnPassantTarget == (pawn.Location - 9)))
                    {
                        targets.Add(pawn.Location - 9);
                    }

                    if (pawn.Location > 7
                        && (pawn.Location % 8) != 7
                        && (pos.Pieces.Any(piece => piece.Color != pos.ColorToMove
                            && piece.Location == (pawn.Location - 7))
                        || pos.EnPassantTarget == (pawn.Location - 7)))
                    {
                        targets.Add(pawn.Location - 7);
                    }
                }

                foreach (int target in targets)
                {
                    if (pos.Pieces.Any(piece => piece.Location == target && piece.Color != pos.ColorToMove)
                        && mgt.HasFlag(MoveGenerationType.Captures))
                    {
                        if (target > 56 || target < 8)
                        {
                            moves.Add(new Move(pawn.Location,
                                target,
                                PieceRank.PAWN,
                                pos.ColorToMove,
                                pos.HalfMoveCounter,
                                pos.CastlingInfo,
                                pos.Pieces.First(piece => piece.Location == target).Rank,
                                PieceRank.QUEEN,
                                pos.EnPassantTarget));

                            moves.Add(new Move(pawn.Location,
                                target,
                                PieceRank.PAWN,
                                pos.ColorToMove,
                                pos.HalfMoveCounter,
                                pos.CastlingInfo,
                                pos.Pieces.First(piece => piece.Location == target).Rank,
                                PieceRank.ROOK,
                                pos.EnPassantTarget));

                            moves.Add(new Move(pawn.Location,
                                target,
                                PieceRank.PAWN,
                                pos.ColorToMove,
                                pos.HalfMoveCounter,
                                pos.CastlingInfo,
                                pos.Pieces.First(piece => piece.Location == target).Rank,
                                PieceRank.BISHOP,
                                pos.EnPassantTarget));

                            moves.Add(new Move(pawn.Location,
                                target,
                                PieceRank.PAWN,
                                pos.ColorToMove,
                                pos.HalfMoveCounter,
                                pos.CastlingInfo,
                                pos.Pieces.First(piece => piece.Location == target).Rank,
                                PieceRank.KNIGHT,
                                pos.EnPassantTarget));
                        }
                        else
                        {
                            moves.Add(new Move(pawn.Location,
                                target,
                                PieceRank.PAWN,
                                pos.ColorToMove,
                                pos.HalfMoveCounter,
                                pos.CastlingInfo,
                                pos.Pieces.First(piece => piece.Location == target).Rank,
                                null,
                                pos.EnPassantTarget));
                        }
                    }
                    
                    if (!pos.Pieces.Any(piece => piece.Location == target)
                        && mgt.HasFlag(MoveGenerationType.NonCaptures))
                    {
                        moves.Add(new Move(pawn.Location,
                                target,
                                PieceRank.PAWN,
                                pos.ColorToMove,
                                pos.HalfMoveCounter,
                                pos.CastlingInfo,
                                null,
                                null,
                                pos.EnPassantTarget));
                    }
                }
            }

            return moves;
        }

        public bool IsSquareAttacked(int loc, Position pos, PieceColor attackColor)
        {
            int tmp_loc = loc;
            bool isAttacked = false;

            while (tmp_loc < 56)
            {
                tmp_loc += 8;

                if (pos.Pieces.Any(p => p.Location == tmp_loc))
                {
                    if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && (p.Rank == PieceRank.QUEEN || p.Rank == PieceRank.ROOK)))
                    {
                        isAttacked = true;
                    }
                    else if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && p.Rank == PieceRank.KING && tmp_loc == (loc + 8)))
                    {
                        isAttacked = true;
                    }

                    break;
                }
            }

            if (isAttacked)
            {
                return true;
            }

            tmp_loc = loc;

            while (tmp_loc < 56 && (tmp_loc % 8) != 7)
            {
                tmp_loc += 9;

                if (pos.Pieces.Any(p => p.Location == tmp_loc))
                {
                    if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && (p.Rank == PieceRank.QUEEN || p.Rank == PieceRank.BISHOP)))
                    {
                        isAttacked = true;
                    }
                    else if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && p.Rank == PieceRank.KING && tmp_loc == (loc + 9)))
                    {
                        isAttacked = true;
                    }

                    break;
                }
            }

            if (isAttacked)
            {
                return true;
            }

            tmp_loc = loc;

            while ((tmp_loc % 8) != 7)
            {
                tmp_loc += 1;

                if (pos.Pieces.Any(p => p.Location == tmp_loc))
                {
                    if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && (p.Rank == PieceRank.QUEEN || p.Rank == PieceRank.ROOK)))
                    {
                        isAttacked = true;
                    }
                    else if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && p.Rank == PieceRank.KING && tmp_loc == (loc + 1)))
                    {
                        isAttacked = true;
                    }

                    break;
                }
            }

            if (isAttacked)
            {
                return true;
            }

            tmp_loc = loc;

            while (tmp_loc > 7 && (tmp_loc % 8) != 7)
            {
                tmp_loc -= 7;

                if (pos.Pieces.Any(p => p.Location == tmp_loc))
                {
                    if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && (p.Rank == PieceRank.QUEEN || p.Rank == PieceRank.BISHOP)))
                    {
                        isAttacked = true;
                    }
                    else if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && p.Rank == PieceRank.KING && tmp_loc == (loc - 7)))
                    {
                        isAttacked = true;
                    }

                    break;
                }
            }

            if (isAttacked)
            {
                return true;
            }

            tmp_loc = loc;

            while (tmp_loc > 7)
            {
                tmp_loc -= 8;

                if (pos.Pieces.Any(p => p.Location == tmp_loc))
                {
                    if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && (p.Rank == PieceRank.QUEEN || p.Rank == PieceRank.ROOK)))
                    {
                        isAttacked = true;
                    }
                    else if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && p.Rank == PieceRank.KING && tmp_loc == (loc - 8)))
                    {
                        isAttacked = true;
                    }

                    break;
                }
            }

            if (isAttacked)
            {
                return true;
            }

            tmp_loc = loc;

            while (tmp_loc > 7 && (tmp_loc % 8) != 0)
            {
                tmp_loc -= 9;

                if (pos.Pieces.Any(p => p.Location == tmp_loc))
                {
                    if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && (p.Rank == PieceRank.QUEEN || p.Rank == PieceRank.BISHOP)))
                    {
                        isAttacked = true;
                    }
                    else if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && p.Rank == PieceRank.KING && tmp_loc == (loc - 9)))
                    {
                        isAttacked = true;
                    }

                    break;
                }
            }

            if (isAttacked)
            {
                return true;
            }

            tmp_loc = loc;

            while ((tmp_loc % 8) != 0)
            {
                tmp_loc -= 1;

                if (pos.Pieces.Any(p => p.Location == tmp_loc))
                {
                    if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && (p.Rank == PieceRank.QUEEN || p.Rank == PieceRank.ROOK)))
                    {
                        isAttacked = true;
                    }
                    else if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && p.Rank == PieceRank.KING && tmp_loc == (loc -1)))
                    {
                        isAttacked = true;
                    }

                    break;
                }
            }

            if (isAttacked)
            {
                return true;
            }

            tmp_loc = loc;

            while (tmp_loc < 56 && (tmp_loc % 8) != 0)
            {
                tmp_loc += 7;

                if (pos.Pieces.Any(p => p.Location == tmp_loc))
                {
                    if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && (p.Rank == PieceRank.QUEEN || p.Rank == PieceRank.BISHOP)))
                    {
                        isAttacked = true;
                    }
                    else if (pos.Pieces.Any(p => p.Location == tmp_loc && p.Color == attackColor
                        && p.Rank == PieceRank.KING && tmp_loc == (loc + 7)))
                    {
                        isAttacked = true;
                    }

                    break;
                }
            }

            if (isAttacked)
            {
                return true;
            }

            if (loc < 56 && (loc % 8) > 1)
            {
                if (pos.Pieces.Any(p => p.Color == attackColor && p.Location == (loc + 6)
                    && p.Rank == PieceRank.KNIGHT))
                {
                    isAttacked = true;
                }
            }

            if (loc < 48 && (loc % 8) > 0)
            {
                if (pos.Pieces.Any(p => p.Color == attackColor && p.Location == (loc + 15)
                    && p.Rank == PieceRank.KNIGHT))
                {
                    isAttacked = true;
                }
            }

            if (loc < 48 && (loc % 8) < 7)
            {
                if (pos.Pieces.Any(p => p.Color == attackColor && p.Location == (loc + 17)
                    && p.Rank == PieceRank.KNIGHT))
                {
                    isAttacked = true;
                }
            }

            if (loc < 56 && (loc % 8) < 6)
            {
                if (pos.Pieces.Any(p => p.Color == attackColor && p.Location == (loc + 10)
                    && p.Rank == PieceRank.KNIGHT))
                {
                    isAttacked = true;
                }
            }

            if (loc > 7 && (loc % 8) < 6)
            {
                if (pos.Pieces.Any(p => p.Color == attackColor && p.Location == (loc - 6)
                    && p.Rank == PieceRank.KNIGHT))
                {
                    isAttacked = true;
                }
            }

            if (loc > 15 && (loc % 8) < 7)
            {
                if (pos.Pieces.Any(p => p.Color == attackColor && p.Location == (loc - 15)
                    && p.Rank == PieceRank.KNIGHT))
                {
                    isAttacked = true;
                }
            }

            if (loc > 15 && (loc % 8) > 0)
            {
                if (pos.Pieces.Any(p => p.Color == attackColor && p.Location == (loc - 17)
                    && p.Rank == PieceRank.KNIGHT))
                {
                    isAttacked = true;
                }
            }

            if (loc > 7 && (loc % 8) > 1)
            {
                if (pos.Pieces.Any(p => p.Color == attackColor && p.Location == (loc - 10)
                    && p.Rank == PieceRank.KNIGHT))
                {
                    isAttacked = true;
                }
            }

            if (isAttacked)
            {
                return true;
            }

            return false;
        }

        public bool IsCapture(int loc, Position pos)
        {
            return pos.Pieces.Any(p => p.Location == loc && p.Color == (pos.ColorToMove == PieceColor.WHITE ? PieceColor.BLACK : PieceColor.WHITE));
        }

        public List<int> GetCaptures(List<int> locs, Position pos)
        {
            return locs.FindAll(l => IsCapture(l, pos));
        }

        public List<int> GetNonCaptures(List<int> locs, Position pos)
        {
            return locs.FindAll(l => !IsCapture(l, pos));
        }


    }
}