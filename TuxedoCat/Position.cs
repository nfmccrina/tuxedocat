using System;
using System.Collections.Generic;

namespace TuxedoCat
{
    struct Position
    {
        public Position(string fen)
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

        public IReadOnlyList<Piece> Pieces
        {
            get
            {
                return pieces.AsReadOnly();
            }
        }

        public bool WhiteCanCastleShort
        {
            get
            {
                return castlingInfo.WhiteCanCastleShort;
            }
        }

        public bool WhiteCanCastleLong
        {
            get
            {
                return castlingInfo.WhiteCanCastleLong;
            }
        }

        public bool BlackCanCastleShort
        {
            get
            {
                return castlingInfo.BlackCanCastleShort;
            }
        }

        public bool BlackCanCastleLong
        {
            get
            {
                return castlingInfo.BlackCanCastleLong;
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

        public void ApplyMove(Move mv)
        {
            enPassantTarget = null;
            halfMoveCounter++;

            if (pieces.Find(p => p.Location == mv.SourceLocation).Rank == PieceRank.KING && Math.Abs(mv.SourceLocation - mv.TargetLocation) > 1)
            {
                // castle

                if (mv.SourceLocation == 4 && mv.TargetLocation == 2)
                {
                    // white queen-side

                    pieces.Add(new Piece(3, PieceRank.ROOK, PieceColor.WHITE));
                    pieces.Add(new Piece(2, PieceRank.KING, PieceColor.WHITE));

                    pieces.RemoveAll(p => p.Location == 0 || p.Location == 4);

                    castlingInfo.WhiteCanCastleLong = false;
                    castlingInfo.WhiteCanCastleShort = false;
                }
                else if(mv.SourceLocation == 4 && mv.TargetLocation == 6)
                {
                    // white king-side

                    pieces.Add(new Piece(5, PieceRank.ROOK, PieceColor.WHITE));
                    pieces.Add(new Piece(6, PieceRank.KING, PieceColor.WHITE));

                    pieces.RemoveAll(p => p.Location == 7 || p.Location == 4);

                    castlingInfo.WhiteCanCastleLong = false;
                    castlingInfo.WhiteCanCastleShort = false;
                }
                else if(mv.SourceLocation == 60 && mv.TargetLocation == 58)
                {
                    // black queen-side

                    pieces.Add(new Piece(59, PieceRank.ROOK, PieceColor.BLACK));
                    pieces.Add(new Piece(58, PieceRank.KING, PieceColor.BLACK));

                    pieces.RemoveAll(p => p.Location == 56 || p.Location == 60);

                    castlingInfo.BlackCanCastleLong = false;
                    castlingInfo.BlackCanCastleShort = false;
                }
                else if(mv.SourceLocation == 60 && mv.TargetLocation == 62)
                {
                    // black king-side

                    pieces.Add(new Piece(61, PieceRank.ROOK, PieceColor.BLACK));
                    pieces.Add(new Piece(62, PieceRank.KING, PieceColor.BLACK));

                    pieces.RemoveAll(p => p.Location == 63 || p.Location == 60);

                    castlingInfo.BlackCanCastleLong = false;
                    castlingInfo.BlackCanCastleShort = false;
                }
            }
            else
            {
                // not a castle

                if (mv.MovingPiece.Rank == PieceRank.PAWN)
                {
                    halfMoveCounter = 0;

                    if (mv.TargetLocation == (mv.SourceLocation + 16))
                    {
                        enPassantTarget = mv.SourceLocation + 8;
                    }
                    else if (mv.TargetLocation == (mv.SourceLocation - 16))
                    {
                        enPassantTarget = mv.SourceLocation - 8;
                    }
                }

                if (mv.MovingPiece.Rank == PieceRank.ROOK && mv.MovingPiece.Color == PieceColor.WHITE
                    && mv.SourceLocation == 0)
                {
                    castlingInfo.WhiteCanCastleLong = false;
                }
                else if (mv.MovingPiece.Rank == PieceRank.ROOK && mv.MovingPiece.Color == PieceColor.WHITE
                    && mv.SourceLocation == 7)
                {
                    castlingInfo.WhiteCanCastleShort = false;
                }
                else if (mv.MovingPiece.Rank == PieceRank.ROOK && mv.MovingPiece.Color == PieceColor.BLACK
                    && mv.SourceLocation == 56)
                {
                    castlingInfo.BlackCanCastleLong = false;
                }
                else if (mv.MovingPiece.Rank == PieceRank.ROOK && mv.MovingPiece.Color == PieceColor.BLACK
                    && mv.SourceLocation == 63)
                {
                    castlingInfo.BlackCanCastleShort = false;
                }

                if (mv.MovingPiece.Rank == PieceRank.KING && mv.MovingPiece.Color == PieceColor.WHITE)
                {
                    castlingInfo.WhiteCanCastleLong = false;
                    castlingInfo.WhiteCanCastleShort = false;
                }
                else if (mv.MovingPiece.Rank == PieceRank.KING && mv.MovingPiece.Color == PieceColor.BLACK)
                {
                    castlingInfo.BlackCanCastleLong = false;
                    castlingInfo.BlackCanCastleShort = false;
                }

                if (mv.CapturedPiece.HasValue)
                {
                    // a capture!
                    pieces.RemoveAll(p => p.Location == mv.CapturedPiece.Value.Location); // he gone
                    halfMoveCounter = 0;
                }

                pieces.Add(new Piece(mv.TargetLocation, mv.PromotedRank.HasValue ? mv.PromotedRank.Value : mv.MovingPiece.Rank, mv.MovingPiece.Color));
                pieces.RemoveAll(p => p.Location == mv.SourceLocation);
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

        private List<Piece> pieces;
        private PieceColor colorToMove;
        private CastlingInfo castlingInfo;
        private int? enPassantTarget;
        private int fullMoveCounter;
        private int halfMoveCounter;
    }
}