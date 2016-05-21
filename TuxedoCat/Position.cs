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

        private List<Piece> pieces;
        private PieceColor colorToMove;
        private CastlingInfo castlingInfo;
        private int? enPassantTarget;
        private int fullMoveCounter;
        private int halfMoveCounter;
    }
}