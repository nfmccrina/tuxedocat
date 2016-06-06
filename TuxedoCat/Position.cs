using System;
using System.Collections.Generic;
using System.Linq;

namespace TuxedoCat
{
    public class Position
    {
        private static Position instance = null;
        private static Position testInstance = null;

        public static Position CurrentPosition
        {
            get
            {
                if (instance == null)
                {
                    instance = new Position();
                    instance.SetPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                }

                return instance;
            }
        }

        public static Position TestPosition
        {
            get
            {
                if (testInstance == null)
                {
                    testInstance = new Position();
                    testInstance.SetPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                }

                return testInstance;
            }
        }

        public void SetPosition(string fen)
        {
            FENParser parser = new FENParser();

            string[] fenParts = fen.Split(new char[] { ' ' });

            if (fenParts.Length != 6)
            {
                throw new ArgumentException("Invalid FEN; could not parse all sections.");
            }

            pieces = parser.ReadPieceInfoFromFEN(fenParts[0]);
            colorToMove = parser.ReadColorToMoveFromFEN(fenParts[1]);
            castlingInfo = parser.ReadCastlingInfoFromFEN(fenParts[2]);
            enPassantTarget = parser.ReadEnPassantTargetFromFEN(fenParts[3]);
            halfMoveCounter = parser.ReadHalfMovesFromFEN(fenParts[4]);
            fullMoveCounter = parser.ReadFullMovesFromFEN(fenParts[5]);
        }

        private Position()
        {   
        }

        public override int GetHashCode()
        {
            int hash = 17;

            hash = (hash * 23) + pieces.GetHashCode();
            hash = (hash * 23) + castlingInfo.GetHashCode();
            hash = (hash * 23) + fullMoveCounter.GetHashCode();


            return hash;
        }

        public override bool Equals(object obj)
        {
            if (obj == null || GetType() != obj.GetType())
            {
                return false;
            }

            Position pos2 = (Position)obj;

            bool piecesAreEqual = true;

            foreach (Piece p in Pieces)
            {
                if (!pos2.Pieces.Contains(p))
                {
                    piecesAreEqual = false;
                    break;
                }
            }

            return piecesAreEqual
                && (CastlingInfo == pos2.CastlingInfo)
                && (ColorToMove == pos2.ColorToMove)
                && (EnPassantTarget == pos2.EnPassantTarget)
                && (FullMoveCounter == pos2.FullMoveCounter)
                && (HalfMoveCounter == pos2.HalfMoveCounter);
        }

        public IReadOnlyList<Piece> Pieces
        {
            get
            {
                return pieces.AsReadOnly();
            }
        }

        public CastlingInfo CastlingInfo
        {
            get
            {
                return castlingInfo;
            }
        }

        public PieceColor ColorToMove
        {
            get
            {
                return colorToMove;
            }
        }

        public int? EnPassantTarget
        {
            get
            {
                return enPassantTarget;
            }
        }

        public int FullMoveCounter
        {
            get
            {
                return fullMoveCounter;
            }
        }

        public int HalfMoveCounter
        {
            get
            {
                return halfMoveCounter;
            }
        }

        public void Make(Move mv)
        {
            enPassantTarget = null;
            halfMoveCounter = mv.CurrentHalfMoves + 1;

            if (pieces.Find(p => p.Location == mv.SourceLocation).Rank == PieceRank.KING && Math.Abs((mv.SourceLocation % 8) - (mv.TargetLocation % 8)) > 1)
            {
                // castle

                halfMoveCounter = 0;

                if (mv.SourceLocation == 4 && mv.TargetLocation == 2)
                {
                    // white queen-side

                    pieces.Add(new Piece(3, PieceRank.ROOK, PieceColor.WHITE));
                    pieces.Add(new Piece(2, PieceRank.KING, PieceColor.WHITE));

                    pieces.RemoveAll(p => p.Location == 0 || p.Location == 4);

                    castlingInfo = castlingInfo & (~CastlingInfo.WHITE_SHORT);
                    castlingInfo = castlingInfo & (~CastlingInfo.WHITE_LONG);
                }
                else if(mv.SourceLocation == 4 && mv.TargetLocation == 6)
                {
                    // white king-side

                    pieces.Add(new Piece(5, PieceRank.ROOK, PieceColor.WHITE));
                    pieces.Add(new Piece(6, PieceRank.KING, PieceColor.WHITE));

                    pieces.RemoveAll(p => p.Location == 7 || p.Location == 4);

                    castlingInfo = castlingInfo & (~CastlingInfo.WHITE_SHORT);
                    castlingInfo = castlingInfo & (~CastlingInfo.WHITE_LONG);
                }
                else if(mv.SourceLocation == 60 && mv.TargetLocation == 58)
                {
                    // black queen-side

                    pieces.Add(new Piece(59, PieceRank.ROOK, PieceColor.BLACK));
                    pieces.Add(new Piece(58, PieceRank.KING, PieceColor.BLACK));

                    pieces.RemoveAll(p => p.Location == 56 || p.Location == 60);

                    castlingInfo = castlingInfo & (~CastlingInfo.BLACK_SHORT);
                    castlingInfo = castlingInfo & (~CastlingInfo.BLACK_LONG);
                }
                else if(mv.SourceLocation == 60 && mv.TargetLocation == 62)
                {
                    // black king-side

                    pieces.Add(new Piece(61, PieceRank.ROOK, PieceColor.BLACK));
                    pieces.Add(new Piece(62, PieceRank.KING, PieceColor.BLACK));

                    pieces.RemoveAll(p => p.Location == 63 || p.Location == 60);

                    castlingInfo = castlingInfo & (~CastlingInfo.BLACK_SHORT);
                    castlingInfo = castlingInfo & (~CastlingInfo.BLACK_LONG);
                }
            }
            else
            {
                // not a castle

                if (mv.MovingPiece == PieceRank.PAWN)
                {
                    // pawn move

                    halfMoveCounter = 0;

                    if (mv.CapturedPiece.HasValue)
                    {
                        if (mv.TargetLocation == mv.CurrentEnPassant)
                        {
                            pieces.RemoveAll(p => p.Location == (mv.MoveColor == PieceColor.WHITE ? (mv.TargetLocation - 8) : (mv.TargetLocation + 8)));
                        }
                        else
                        {
                            pieces.RemoveAll(p => p.Location == mv.TargetLocation);
                        }
                    }
                    else
                    {
                        if (mv.TargetLocation == (mv.SourceLocation + 16))
                        {
                            enPassantTarget = mv.SourceLocation + 8;
                        }
                        else if (mv.TargetLocation == (mv.SourceLocation - 16))
                        {
                            enPassantTarget = mv.SourceLocation - 8;
                        }
                    }

                    pieces.Add(new Piece(mv.TargetLocation, mv.PromotedRank.HasValue ? mv.PromotedRank.Value : mv.MovingPiece, mv.MoveColor));
                    pieces.RemoveAll(p => p.Location == mv.SourceLocation);
                }
                else
                {
                    if (mv.MovingPiece == PieceRank.ROOK && mv.MoveColor == PieceColor.WHITE
                        && mv.SourceLocation == 0)
                    {
                        castlingInfo = castlingInfo & (~CastlingInfo.WHITE_LONG);
                    }
                    else if (mv.MovingPiece == PieceRank.ROOK && mv.MoveColor == PieceColor.WHITE
                        && mv.SourceLocation == 7)
                    {
                        castlingInfo = castlingInfo & (~CastlingInfo.WHITE_SHORT);
                    }
                    else if (mv.MovingPiece == PieceRank.ROOK && mv.MoveColor == PieceColor.BLACK
                        && mv.SourceLocation == 56)
                    {
                        castlingInfo = castlingInfo & (~CastlingInfo.BLACK_LONG);
                    }
                    else if (mv.MovingPiece == PieceRank.ROOK && mv.MoveColor == PieceColor.BLACK
                        && mv.SourceLocation == 63)
                    {
                        castlingInfo = castlingInfo & (~CastlingInfo.BLACK_SHORT);
                    }

                    if (mv.MovingPiece == PieceRank.KING && mv.MoveColor == PieceColor.WHITE)
                    {
                        castlingInfo = castlingInfo & (~CastlingInfo.WHITE_LONG);
                        castlingInfo = castlingInfo & (~CastlingInfo.WHITE_SHORT);
                    }
                    else if (mv.MovingPiece == PieceRank.KING && mv.MoveColor == PieceColor.BLACK)
                    {
                        castlingInfo = castlingInfo & (~CastlingInfo.BLACK_LONG);
                        castlingInfo = castlingInfo & (~CastlingInfo.BLACK_SHORT);
                    }

                    if (mv.CapturedPiece.HasValue)
                    {
                        // a capture!
                        pieces.RemoveAll(p => p.Location == mv.TargetLocation); // he gone
                        halfMoveCounter = 0;
                    }

                    pieces.Add(new Piece(mv.TargetLocation, mv.MovingPiece, mv.MoveColor));
                    pieces.RemoveAll(p => p.Location == mv.SourceLocation);
                }
            }

            if (colorToMove == PieceColor.WHITE)
            {
                colorToMove = PieceColor.BLACK;
            }
            else
            {
                colorToMove = PieceColor.WHITE;
                fullMoveCounter++;
            }
        }

        public void Unmake(Move mv)
        {
            pieces.Add(new Piece(mv.SourceLocation, mv.MovingPiece, mv.MoveColor));
            pieces.RemoveAll(p => p.Location == mv.TargetLocation);

            if (mv.MovingPiece == PieceRank.KING && Math.Abs((mv.TargetLocation % 8) - (mv.SourceLocation % 8)) > 1)
            {
                int oldRookLocation = -1;
                int newRookLocation = -1;

                if (mv.SourceLocation == 4 && mv.TargetLocation == 6)
                {
                    oldRookLocation = 5;
                    newRookLocation = 7;
                }
                else if (mv.SourceLocation == 4 && mv.TargetLocation == 2)
                {
                    oldRookLocation = 3;
                    newRookLocation = 0;
                }
                else if (mv.SourceLocation == 60 && mv.TargetLocation == 62)
                {
                    oldRookLocation = 61;
                    newRookLocation = 63;
                }
                else if (mv.SourceLocation == 60 && mv.TargetLocation == 58)
                {
                    oldRookLocation = 59;
                    newRookLocation = 56;
                }

                pieces.RemoveAll(p => p.Location == mv.TargetLocation);
                pieces.RemoveAll(p => p.Location == oldRookLocation);

                pieces.Add(new Piece(mv.SourceLocation, PieceRank.KING,
                    mv.MoveColor == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK));

                pieces.Add(new Piece(newRookLocation, PieceRank.ROOK,
                    mv.MoveColor == PieceColor.WHITE ? PieceColor.WHITE : PieceColor.BLACK));
            }

            if (mv.CapturedPiece.HasValue)
            {
                if (mv.CurrentEnPassant.HasValue && mv.MovingPiece == PieceRank.PAWN
                    && mv.TargetLocation == mv.CurrentEnPassant)
                {
                    if (mv.MoveColor == PieceColor.WHITE)
                    {
                        pieces.Add(new Piece(mv.TargetLocation - 8, mv.CapturedPiece.Value, PieceColor.BLACK));
                    }
                    else
                    {
                        pieces.Add(new Piece(mv.TargetLocation + 8, mv.CapturedPiece.Value, PieceColor.WHITE));
                    }
                }
                else
                {
                    if (mv.MoveColor == PieceColor.WHITE)
                    {
                        pieces.Add(new Piece(mv.TargetLocation, mv.CapturedPiece.Value, PieceColor.BLACK));
                    }
                    else
                    {
                        pieces.Add(new Piece(mv.TargetLocation, mv.CapturedPiece.Value, PieceColor.WHITE));
                    }
                }
            }

            colorToMove = mv.MoveColor;
            castlingInfo = mv.CurrentCastlingInfo;
            enPassantTarget = mv.CurrentEnPassant;
            halfMoveCounter = mv.CurrentHalfMoves;

            if (mv.MoveColor == PieceColor.BLACK)
            {
                fullMoveCounter--;
            }
        }

        private List<Piece> pieces;
        private PieceColor colorToMove;
        private CastlingInfo castlingInfo;
        private int? enPassantTarget;
        private int fullMoveCounter;
        private int halfMoveCounter;
    }
}