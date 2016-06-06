using Microsoft.VisualStudio.TestTools.UnitTesting;
using TuxedoCat;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TuxedoCat.Tests
{
    [TestClass()]
    public class MoveGeneratorTests
    {
        [TestMethod()]
        public void IsCaptureTest_positive()
        {
            MoveGenerator mg = new MoveGenerator();
            Position p = new Position("4k3/8/R7/2n5/8/8/8/4K3 b - - 14 56");

            Assert.IsTrue(mg.IsCapture(40, p));
        }

        [TestMethod()]
        public void IsCaptureTest_negative()
        {
            MoveGenerator mg = new MoveGenerator();
            Position p = new Position("4k3/8/R7/2n5/8/8/8/4K3 b - - 14 56");

            Assert.IsFalse(mg.IsCapture(44, p));
        }

        [TestMethod()]
        public void IsSquareAttacked_position1()
        {
            MoveGenerator mg = new MoveGenerator();
            Position p = new Position("4k3/8/8/8/8/b7/8/4K3 w - - 0 56");

            Assert.IsTrue(mg.IsSquareAttacked(2, p, PieceColor.BLACK));
        }

        [TestMethod()]
        public void IsSquareAttacked_position2()
        {
            MoveGenerator mg = new MoveGenerator();
            Position p = new Position("4k3/8/8/2r5/8/8/8/4K3 w - - 0 56");

            Assert.IsTrue(mg.IsSquareAttacked(2, p, PieceColor.BLACK));
        }

        [TestMethod()]
        public void IsSquareAttacked_position3()
        {
            MoveGenerator mg = new MoveGenerator();
            Position p = new Position("rnbqkbnr/1ppppppp/p7/8/2P5/8/PP1PPPPP/RNBQKBNR b KQkq - 0 1");

            Assert.IsFalse(mg.IsSquareAttacked(4, p, PieceColor.BLACK));
        }

        [TestMethod()]
        public void IsSquareAttacked_position4()
        {
            MoveGenerator mg = new MoveGenerator();
            Position p = new Position("rnbqkbnr/p1pppppp/8/8/1p6/3P4/PPPKPPPP/RNBQ1BNR w KQkq - 0 1");

            Assert.IsTrue(mg.IsSquareAttacked(18, p, PieceColor.BLACK));
        }

        [TestMethod()]
        public void BishopMovesTest_position1()
        {
            MoveGenerator mg = new MoveGenerator();
            Position position = new Position("4k3/8/8/8/8/8/1p6/B3K3 w - - 12 45");

            List<Move> moves = mg.BishopMoves(position);

            Assert.AreEqual(1, moves.Count);
            Assert.IsTrue(moves[0].CapturedPiece.HasValue);
            Assert.AreEqual(PieceRank.PAWN, moves[0].CapturedPiece.Value);
            Assert.AreEqual(0, moves[0].SourceLocation);
            Assert.AreEqual(9, moves[0].TargetLocation);
            Assert.AreEqual(12, moves[0].CurrentHalfMoves);
            Assert.IsNull(moves[0].CurrentEnPassant);
            Assert.IsNull(moves[0].PromotedRank);
            Assert.AreEqual(CastlingInfo.NONE, moves[0].CurrentCastlingInfo);
            Assert.AreEqual(PieceColor.WHITE, moves[0].MoveColor);
            Assert.AreEqual(PieceRank.BISHOP, moves[0].MovingPiece);
        }

        [TestMethod()]
        public void KnightMovesTest_position1()
        {
            MoveGenerator mg = new MoveGenerator();
            Position position = new Position("4k3/8/8/8/8/8/2p5/N3K3 w - - 12 45");

            List<Move> moves = mg.KnightMoves(position);

            Assert.AreEqual(2, moves.Count);

            Assert.IsTrue(moves.Any(move => move.CapturedPiece.HasValue
                && move.CapturedPiece.Value == PieceRank.PAWN
                && move.SourceLocation == 0
                && move.TargetLocation == 10
                && move.CurrentHalfMoves == 12
                && move.CurrentEnPassant == null
                && move.PromotedRank == null
                && move.CurrentCastlingInfo == CastlingInfo.NONE
                && move.MoveColor == PieceColor.WHITE
                && move.MovingPiece == PieceRank.KNIGHT));

            Assert.IsTrue(moves.Any(move => !move.CapturedPiece.HasValue
                && move.SourceLocation == 0
                && move.TargetLocation == 17
                && move.CurrentHalfMoves == 12
                && move.CurrentEnPassant == null
                && move.PromotedRank == null
                && move.CurrentCastlingInfo == CastlingInfo.NONE
                && move.MoveColor == PieceColor.WHITE
                && move.MovingPiece == PieceRank.KNIGHT));
        }

        [TestMethod()]
        public void RookMovesTest_position1()
        {
            MoveGenerator mg = new MoveGenerator();
            Position position = new Position("4k3/8/8/8/8/8/p7/RN2K3 w - - 12 45");

            List<Move> moves = mg.RookMoves(position);

            Assert.AreEqual(1, moves.Count);

            Assert.IsTrue(moves.Any(move => move.CapturedPiece.HasValue
                && move.CapturedPiece.Value == PieceRank.PAWN
                && move.SourceLocation == 0
                && move.TargetLocation == 8
                && move.CurrentHalfMoves == 12
                && move.CurrentEnPassant == null
                && move.PromotedRank == null
                && move.CurrentCastlingInfo == CastlingInfo.NONE
                && move.MoveColor == PieceColor.WHITE
                && move.MovingPiece == PieceRank.ROOK));
        }

        [TestMethod()]
        public void QueenMovesTest_position1()
        {
            MoveGenerator mg = new MoveGenerator();
            Position position = new Position("4k3/8/8/8/8/8/pp6/qN2K3 b - - 12 45");

            List<Move> moves = mg.QueenMoves(position);

            Assert.AreEqual(1, moves.Count);

            Assert.IsTrue(moves.Any(move => move.CapturedPiece.HasValue
                && move.CapturedPiece.Value == PieceRank.KNIGHT
                && move.SourceLocation == 0
                && move.TargetLocation == 1
                && move.CurrentHalfMoves == 12
                && move.CurrentEnPassant == null
                && move.PromotedRank == null
                && move.CurrentCastlingInfo == CastlingInfo.NONE
                && move.MoveColor == PieceColor.BLACK
                && move.MovingPiece == PieceRank.QUEEN));
        }

        [TestMethod()]
        public void KingMovesTest_position1()
        {
            MoveGenerator mg = new MoveGenerator();
            Position position = new Position("6nk/6Pp/8/8/8/8/8/4K3 b - - 12 45");

            List<Move> moves = mg.KingMoves(position);

            Assert.AreEqual(1, moves.Count);

            Assert.IsTrue(moves.Any(move => move.CapturedPiece.HasValue
                && move.CapturedPiece.Value == PieceRank.PAWN
                && move.SourceLocation == 63
                && move.TargetLocation == 54
                && move.CurrentHalfMoves == 12
                && move.CurrentEnPassant == null
                && move.PromotedRank == null
                && move.CurrentCastlingInfo == CastlingInfo.NONE
                && move.MoveColor == PieceColor.BLACK
                && move.MovingPiece == PieceRank.KING));
        }

        [TestMethod()]
        public void PawnMovesTest_position1()
        {
            MoveGenerator mg = new MoveGenerator();
            Position position = new Position("k7/6nr/6P1/8/8/8/8/4K3 w - - 12 45");

            List<Move> moves = mg.PawnMoves(position);

            Assert.AreEqual(1, moves.Count);

            Assert.IsTrue(moves.Any(move => move.CapturedPiece.HasValue
                && move.CapturedPiece.Value == PieceRank.ROOK
                && move.SourceLocation == 46
                && move.TargetLocation == 55
                && move.CurrentHalfMoves == 12
                && move.CurrentEnPassant == null
                && move.PromotedRank == null
                && move.CurrentCastlingInfo == CastlingInfo.NONE
                && move.MoveColor == PieceColor.WHITE
                && move.MovingPiece == PieceRank.PAWN));
        }

        [TestMethod()]
        public void Perft_position1_depth1()
        {
            MoveGenerator mg = new MoveGenerator();
            Position p = new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

            Assert.AreEqual(20, mg.Perft(1, p));
        }

        [TestMethod()]
        public void Perft_position1_depth2()
        {
            MoveGenerator mg = new MoveGenerator();
            Position p = new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

            Assert.AreEqual(400, mg.Perft(2, p));
        }

        [TestMethod()]
        public void Perft_position1_depth3()
        {
            MoveGenerator mg = new MoveGenerator();
            Position p = new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

            Assert.AreEqual(8902, mg.Perft(3, p));
        }

        [TestMethod()]
        public void Perft_position1_depth4()
        {
            MoveGenerator mg = new MoveGenerator();
            Position p = new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

            Assert.AreEqual(197281, mg.Perft(4, p));
        }
    }
}