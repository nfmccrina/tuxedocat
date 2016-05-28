﻿using Microsoft.VisualStudio.TestTools.UnitTesting;
using TuxedoCat;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TuxedoCat.Tests
{
    [TestClass()]
    public class PositionTests
    {
        [TestMethod()]
        public void MakeTest_position1()
        {
            Position p = new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

            Move m = new Move(12, 28, PieceRank.PAWN, PieceColor.WHITE, 0, p.CastlingInfo);

            p.Make(m);

            Assert.AreEqual(32, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(56, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(57, PieceRank.KNIGHT, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(58, PieceRank.BISHOP, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(59, PieceRank.QUEEN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(61, PieceRank.BISHOP, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(62, PieceRank.KNIGHT, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(63, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(48, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(49, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(50, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(51, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(52, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(53, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(54, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(55, PieceRank.PAWN, PieceColor.BLACK)));

            Assert.IsTrue(p.Pieces.Contains(new Piece(0, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(1, PieceRank.KNIGHT, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(2, PieceRank.BISHOP, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(3, PieceRank.QUEEN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(4, PieceRank.KING, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(5, PieceRank.BISHOP, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(6, PieceRank.KNIGHT, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(7, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(8, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(9, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(10, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(11, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(28, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(13, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(14, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(15, PieceRank.PAWN, PieceColor.WHITE)));

            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(0, p.HalfMoveCounter);
            Assert.AreEqual(1, p.FullMoveCounter);
            Assert.AreEqual(20, p.EnPassantTarget);
            Assert.AreEqual(PieceColor.BLACK, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position2()
        {
            Position p = new Position("r1bqkb1r/p2p1ppp/2p1pn2/2p5/4P3/2N2N2/PPPP1PPP/R1BQK2R w KQkq - 0 6");
            Move m = new Move(4, 6, PieceRank.KING, PieceColor.WHITE, 0, p.CastlingInfo);

            p.Make(m);

            Assert.AreEqual(30, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(56, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(58, PieceRank.BISHOP, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(59, PieceRank.QUEEN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(61, PieceRank.BISHOP, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(45, PieceRank.KNIGHT, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(63, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(48, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(42, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(34, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(51, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(44, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(53, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(54, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(55, PieceRank.PAWN, PieceColor.BLACK)));

            Assert.IsTrue(p.Pieces.Contains(new Piece(0, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(18, PieceRank.KNIGHT, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(2, PieceRank.BISHOP, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(3, PieceRank.QUEEN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(6, PieceRank.KING, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(21, PieceRank.KNIGHT, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(5, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(8, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(9, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(10, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(11, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(28, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(13, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(14, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(15, PieceRank.PAWN, PieceColor.WHITE)));

            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(0, p.HalfMoveCounter);
            Assert.AreEqual(6, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.BLACK, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position3()
        {
            Position p = new Position("r3kb2/1ppq4/pnn2pp1/4p3/4P1pr/1BP5/PP1BQPPN/R3K2R w KQq - 0 17");
            Move m = new Move(4, 2, PieceRank.KING, PieceColor.WHITE, 0, p.CastlingInfo);

            p.Make(m);

            Assert.AreEqual(27, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(56, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(61, PieceRank.BISHOP, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(49, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(50, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(51, PieceRank.QUEEN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(40, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(41, PieceRank.KNIGHT, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(42, PieceRank.KNIGHT, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(45, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(46, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(36, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(28, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(30, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(31, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(17, PieceRank.BISHOP, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(18, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(8, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(9, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(11, PieceRank.BISHOP, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(12, PieceRank.QUEEN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(13, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(14, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(15, PieceRank.KNIGHT, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(2, PieceRank.KING, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(3, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(7, PieceRank.ROOK, PieceColor.WHITE)));

            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(0, p.HalfMoveCounter);
            Assert.AreEqual(17, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.BLACK, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position4()
        {
            Position p = new Position("r3kb2/1pp3q1/pnn2pp1/4p3/4P1N1/1BP3P1/PP1BQP2/2K4R b q - 0 20");
            Move m = new Move(60, 58, PieceRank.KING, PieceColor.BLACK, 0, p.CastlingInfo);

            p.Make(m);

            Assert.AreEqual(24, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(59, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(58, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(61, PieceRank.BISHOP, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(49, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(50, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(54, PieceRank.QUEEN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(40, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(41, PieceRank.KNIGHT, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(42, PieceRank.KNIGHT, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(45, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(46, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(36, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(28, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(17, PieceRank.BISHOP, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(18, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(8, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(9, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(11, PieceRank.BISHOP, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(12, PieceRank.QUEEN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(13, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(22, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(30, PieceRank.KNIGHT, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(2, PieceRank.KING, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(7, PieceRank.ROOK, PieceColor.WHITE)));

            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(0, p.HalfMoveCounter);
            Assert.AreEqual(21, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.WHITE, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position5()
        {
            Position p = new Position("rnb1k2r/ppp1qppp/3p1n2/4p3/3PP3/2PB1N2/P1P2PPP/R1BQK2R b KQkq - 2 7");
            Move m = new Move(60, 62, PieceRank.KING, PieceColor.BLACK, 2, p.CastlingInfo);

            p.Make(m);

            Assert.AreEqual(30, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(56, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(57, PieceRank.KNIGHT, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(58, PieceRank.BISHOP, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(52, PieceRank.QUEEN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(62, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(45, PieceRank.KNIGHT, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(61, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(48, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(49, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(50, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(43, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(36, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(53, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(54, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(55, PieceRank.PAWN, PieceColor.BLACK)));

            Assert.IsTrue(p.Pieces.Contains(new Piece(0, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(2, PieceRank.BISHOP, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(3, PieceRank.QUEEN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(4, PieceRank.KING, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(19, PieceRank.BISHOP, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(21, PieceRank.KNIGHT, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(7, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(8, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(18, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(10, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(27, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(28, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(13, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(14, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(15, PieceRank.PAWN, PieceColor.WHITE)));

            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(0, p.HalfMoveCounter);
            Assert.AreEqual(8, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.WHITE, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position6()
        {
            Position p = new Position("4k3/8/8/4Pp2/8/8/8/4K3 w - f6 0 56");
            Move m = new Move(36, 45, PieceRank.PAWN, PieceColor.WHITE, 0, p.CastlingInfo, PieceRank.PAWN, null, 45);

            p.Make(m);

            Assert.AreEqual(3, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(45, PieceRank.PAWN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(4, PieceRank.KING, PieceColor.WHITE)));

            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(0, p.HalfMoveCounter);
            Assert.AreEqual(56, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.BLACK, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position7()
        {
            Position p = new Position("4k3/8/8/8/4Pp2/8/8/4K3 b - e3 0 56");
            Move m = new Move(29, 20, PieceRank.PAWN, PieceColor.BLACK, 0, p.CastlingInfo, PieceRank.PAWN, null, 20);

            p.Make(m);

            Assert.AreEqual(3, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(20, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(4, PieceRank.KING, PieceColor.WHITE)));

            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(0, p.HalfMoveCounter);
            Assert.AreEqual(57, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.WHITE, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position8()
        {
            Position p = new Position("4k3/4p3/8/8/8/8/8/4K3 b - - 8 56");
            Move m = new Move(52, 36, PieceRank.PAWN, PieceColor.BLACK, 8, p.CastlingInfo);

            p.Make(m);

            Assert.AreEqual(3, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(36, PieceRank.PAWN, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(4, PieceRank.KING, PieceColor.WHITE)));

            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(0, p.HalfMoveCounter);
            Assert.AreEqual(57, p.FullMoveCounter);
            Assert.AreEqual(44, p.EnPassantTarget);
            Assert.AreEqual(PieceColor.WHITE, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position9()
        {
            Position p = new Position("4k3/7P/8/8/8/8/8/4K3 w - - 0 56");
            Move m = new Move(55, 63, PieceRank.PAWN, PieceColor.WHITE, 0, p.CastlingInfo, null, PieceRank.QUEEN, null);

            p.Make(m);

            Assert.AreEqual(3, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(63, PieceRank.QUEEN, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(4, PieceRank.KING, PieceColor.WHITE)));

            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(0, p.HalfMoveCounter);
            Assert.AreEqual(56, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.BLACK, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position10()
        {
            Position p = new Position("4k3/8/R7/2n5/8/8/8/4K3 b - - 14 56");
            Move m = new Move(34, 40, PieceRank.KNIGHT, PieceColor.BLACK, 14, p.CastlingInfo, PieceRank.ROOK, null, null);

            p.Make(m);

            Assert.AreEqual(3, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(40, PieceRank.KNIGHT, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(4, PieceRank.KING, PieceColor.WHITE)));

            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(0, p.HalfMoveCounter);
            Assert.AreEqual(57, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.WHITE, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position11()
        {
            Position p = new Position("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 56");
            Move m = new Move(7, 15, PieceRank.ROOK, PieceColor.WHITE, 0, p.CastlingInfo);

            p.Make(m);

            Assert.AreEqual(6, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(4, PieceRank.KING, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(56, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(63, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(0, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(15, PieceRank.ROOK, PieceColor.WHITE)));

            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(1, p.HalfMoveCounter);
            Assert.AreEqual(56, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.BLACK, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position12()
        {
            Position p = new Position("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 56");
            Move m = new Move(0, 8, PieceRank.ROOK, PieceColor.WHITE, 0, p.CastlingInfo);

            p.Make(m);

            Assert.AreEqual(6, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(4, PieceRank.KING, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(56, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(63, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(8, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(7, PieceRank.ROOK, PieceColor.WHITE)));

            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(1, p.HalfMoveCounter);
            Assert.AreEqual(56, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.BLACK, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position13()
        {
            Position p = new Position("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 56");
            Move m = new Move(56, 32, PieceRank.ROOK, PieceColor.BLACK, 0, p.CastlingInfo);

            p.Make(m);

            Assert.AreEqual(6, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(4, PieceRank.KING, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(32, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(63, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(0, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(7, PieceRank.ROOK, PieceColor.WHITE)));

            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(1, p.HalfMoveCounter);
            Assert.AreEqual(57, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.WHITE, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position14()
        {
            Position p = new Position("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 56");
            Move m = new Move(63, 15, PieceRank.ROOK, PieceColor.BLACK, 0, p.CastlingInfo);

            p.Make(m);

            Assert.AreEqual(6, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(4, PieceRank.KING, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(56, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(15, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(0, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(7, PieceRank.ROOK, PieceColor.WHITE)));

            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(1, p.HalfMoveCounter);
            Assert.AreEqual(57, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.WHITE, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position15()
        {
            Position p = new Position("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 56");
            Move m = new Move(4, 13, PieceRank.KING, PieceColor.WHITE, 0, p.CastlingInfo);

            p.Make(m);

            Assert.AreEqual(6, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(60, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(13, PieceRank.KING, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(56, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(63, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(0, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(7, PieceRank.ROOK, PieceColor.WHITE)));

            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(1, p.HalfMoveCounter);
            Assert.AreEqual(56, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.BLACK, p.ColorToMove);
        }

        [TestMethod()]
        public void MakeTest_position16()
        {
            Position p = new Position("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 56");

            Piece movingPiece = new Piece(60, PieceRank.KING, PieceColor.BLACK);

            Move m = new Move(60, 53, PieceRank.KING, PieceColor.BLACK, 0, p.CastlingInfo);

            p.Make(m);

            Assert.AreEqual(6, p.Pieces.Count);

            Assert.IsTrue(p.Pieces.Contains(new Piece(53, PieceRank.KING, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(4, PieceRank.KING, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(56, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(63, PieceRank.ROOK, PieceColor.BLACK)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(0, PieceRank.ROOK, PieceColor.WHITE)));
            Assert.IsTrue(p.Pieces.Contains(new Piece(7, PieceRank.ROOK, PieceColor.WHITE)));

            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsTrue(p.CastlingInfo.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsFalse(p.CastlingInfo.HasFlag(CastlingInfo.BLACK_LONG));

            Assert.AreEqual(1, p.HalfMoveCounter);
            Assert.AreEqual(57, p.FullMoveCounter);
            Assert.IsNull(p.EnPassantTarget);
            Assert.AreEqual(PieceColor.WHITE, p.ColorToMove);
        }

        [TestMethod()]
        public void UnmakeTest_position1()
        {
            Position p = new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            Position tmp = new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

            Move m = new Move(12, 28, PieceRank.PAWN, PieceColor.WHITE, 0, p.CastlingInfo);

            p.Make(m);

            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position2()
        {
            Position p = new Position("r1bqkb1r/p2p1ppp/2p1pn2/2p5/4P3/2N2N2/PPPP1PPP/R1BQK2R w KQkq - 0 6");
            Position tmp = new Position("r1bqkb1r/p2p1ppp/2p1pn2/2p5/4P3/2N2N2/PPPP1PPP/R1BQK2R w KQkq - 0 6");
            Move m = new Move(4, 6, PieceRank.KING, PieceColor.WHITE, 0, p.CastlingInfo);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position3()
        {
            Position p = new Position("r3kb2/1ppq4/pnn2pp1/4p3/4P1pr/1BP5/PP1BQPPN/R3K2R w KQq - 0 17");
            Position tmp = new Position("r3kb2/1ppq4/pnn2pp1/4p3/4P1pr/1BP5/PP1BQPPN/R3K2R w KQq - 0 17");
            Move m = new Move(4, 2, PieceRank.KING, PieceColor.WHITE, 0, p.CastlingInfo);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position4()
        {
            Position p = new Position("r3kb2/1pp3q1/pnn2pp1/4p3/4P1N1/1BP3P1/PP1BQP2/2K4R b q - 0 20");
            Position tmp = new Position("r3kb2/1pp3q1/pnn2pp1/4p3/4P1N1/1BP3P1/PP1BQP2/2K4R b q - 0 20");
            Move m = new Move(60, 58, PieceRank.KING, PieceColor.BLACK, 0, p.CastlingInfo);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position5()
        {
            Position p = new Position("rnb1k2r/ppp1qppp/3p1n2/4p3/3PP3/2PB1N2/P1P2PPP/R1BQK2R b KQkq - 2 7");
            Position tmp = new Position("rnb1k2r/ppp1qppp/3p1n2/4p3/3PP3/2PB1N2/P1P2PPP/R1BQK2R b KQkq - 2 7");
            Move m = new Move(60, 62, PieceRank.KING, PieceColor.BLACK, 2, p.CastlingInfo);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position6()
        {
            Position p = new Position("4k3/8/8/4Pp2/8/8/8/4K3 w - f6 0 56");
            Position tmp = new Position("4k3/8/8/4Pp2/8/8/8/4K3 w - f6 0 56");
            Move m = new Move(36, 45, PieceRank.PAWN, PieceColor.WHITE, 0, p.CastlingInfo, PieceRank.PAWN, null, 45);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position7()
        {
            Position p = new Position("4k3/8/8/8/4Pp2/8/8/4K3 b - e3 0 56");
            Position tmp = new Position("4k3/8/8/8/4Pp2/8/8/4K3 b - e3 0 56");
            Move m = new Move(29, 20, PieceRank.PAWN, PieceColor.BLACK, 0, p.CastlingInfo, PieceRank.PAWN, null, 20);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position8()
        {
            Position p = new Position("4k3/4p3/8/8/8/8/8/4K3 b - - 8 56");
            Position tmp = new Position("4k3/4p3/8/8/8/8/8/4K3 b - - 8 56");
            Move m = new Move(52, 36, PieceRank.PAWN, PieceColor.BLACK, 8, p.CastlingInfo);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position9()
        {
            Position p = new Position("4k3/7P/8/8/8/8/8/4K3 w - - 0 56");
            Position tmp = new Position("4k3/7P/8/8/8/8/8/4K3 w - - 0 56");
            Move m = new Move(55, 63, PieceRank.PAWN, PieceColor.WHITE, 0, p.CastlingInfo, null, PieceRank.QUEEN, null);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position10()
        {
            Position p = new Position("4k3/8/R7/2n5/8/8/8/4K3 b - - 14 56");
            Position tmp = new Position("4k3/8/R7/2n5/8/8/8/4K3 b - - 14 56");
            Move m = new Move(34, 40, PieceRank.KNIGHT, PieceColor.BLACK, 14, p.CastlingInfo, PieceRank.ROOK, null, null);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position11()
        {
            Position p = new Position("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 56");
            Position tmp = new Position("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 56");
            Move m = new Move(7, 15, PieceRank.ROOK, PieceColor.WHITE, 0, p.CastlingInfo);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position12()
        {
            Position p = new Position("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 56");
            Position tmp = new Position("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 56");
            Move m = new Move(0, 8, PieceRank.ROOK, PieceColor.WHITE, 0, p.CastlingInfo);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position13()
        {
            Position p = new Position("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 56");
            Position tmp = new Position("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 56");
            Move m = new Move(56, 32, PieceRank.ROOK, PieceColor.BLACK, 0, p.CastlingInfo);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position14()
        {
            Position p = new Position("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 56");
            Position tmp = new Position("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 56");
            Move m = new Move(63, 15, PieceRank.ROOK, PieceColor.BLACK, 0, p.CastlingInfo);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position15()
        {
            Position p = new Position("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 56");
            Position tmp = new Position("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 56");
            Move m = new Move(4, 13, PieceRank.KING, PieceColor.WHITE, 0, p.CastlingInfo);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }

        [TestMethod()]
        public void UnmakeTest_position16()
        {
            Position p = new Position("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 56");
            Position tmp = new Position("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 56");

            Piece movingPiece = new Piece(60, PieceRank.KING, PieceColor.BLACK);

            Move m = new Move(60, 53, PieceRank.KING, PieceColor.BLACK, 0, p.CastlingInfo);

            p.Make(m);
            p.Unmake(m);

            Assert.AreEqual(tmp, p);
        }
    }
}