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
    public class FENParserTests
    {
        [TestMethod()]
        public void ReadPieceInfoFromFENTest_position1()
        {
            FENParser parser = new FENParser();

            List<Piece> pieces = parser.ReadPieceInfoFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

            Assert.IsNotNull(pieces);
            Assert.AreEqual(32, pieces.Count);
            CollectionAssert.Contains(pieces, new Piece(56, PieceRank.ROOK, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(57, PieceRank.KNIGHT, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(58, PieceRank.BISHOP, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(59, PieceRank.QUEEN, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(60, PieceRank.KING, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(61, PieceRank.BISHOP, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(62, PieceRank.KNIGHT, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(63, PieceRank.ROOK, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(48, PieceRank.PAWN, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(49, PieceRank.PAWN, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(50, PieceRank.PAWN, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(51, PieceRank.PAWN, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(52, PieceRank.PAWN, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(53, PieceRank.PAWN, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(54, PieceRank.PAWN, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(55, PieceRank.PAWN, PieceColor.BLACK));
            CollectionAssert.Contains(pieces, new Piece(8, PieceRank.PAWN, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(9, PieceRank.PAWN, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(10, PieceRank.PAWN, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(11, PieceRank.PAWN, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(12, PieceRank.PAWN, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(13, PieceRank.PAWN, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(14, PieceRank.PAWN, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(15, PieceRank.PAWN, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(0, PieceRank.ROOK, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(1, PieceRank.KNIGHT, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(2, PieceRank.BISHOP, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(3, PieceRank.QUEEN, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(4, PieceRank.KING, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(5, PieceRank.BISHOP, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(6, PieceRank.KNIGHT, PieceColor.WHITE));
            CollectionAssert.Contains(pieces, new Piece(7, PieceRank.ROOK, PieceColor.WHITE));
        }

        [TestMethod()]
        public void ReadColorToMoveFromFENTest_white()
        {
            FENParser parser = new FENParser();

            PieceColor pc = parser.ReadColorToMoveFromFEN("w");

            Assert.AreEqual(PieceColor.WHITE, pc);
        }

        [TestMethod()]
        public void ReadColorToMoveFromFENTest_black()
        {
            FENParser parser = new FENParser();

            PieceColor pc = parser.ReadColorToMoveFromFEN("b");

            Assert.AreEqual(PieceColor.BLACK, pc);
        }

        [TestMethod()]
        public void ReadColorToMoveFromFENTest_capital()
        {
            FENParser parser = new FENParser();
            string msg = "";

            try
            {
                PieceColor pc = parser.ReadColorToMoveFromFEN("W");
            }
            catch (ArgumentException ex)
            {
                msg = ex.Message;
            }

            Assert.AreEqual("Invalid FEN; could not parse color to move.", msg);
        }

        [TestMethod()]
        public void ReadCastlingInfoFromFENTest_input1()
        {
            FENParser parser = new FENParser();

            CastlingInfo ci = parser.ReadCastlingInfoFromFEN("-");

            Assert.AreEqual(ci, CastlingInfo.NONE);
        }

        [TestMethod()]
        public void ReadCastlingInfoFromFENTest_input2()
        {
            FENParser parser = new FENParser();

            CastlingInfo ci = parser.ReadCastlingInfoFromFEN("KQkq");

            Assert.IsTrue(ci.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsTrue(ci.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsTrue(ci.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsTrue(ci.HasFlag(CastlingInfo.BLACK_LONG));
        }

        [TestMethod()]
        public void ReadCastlingInfoFromFENTest_input3()
        {
            FENParser parser = new FENParser();

            CastlingInfo ci = parser.ReadCastlingInfoFromFEN("Kq");

            Assert.IsTrue(ci.HasFlag(CastlingInfo.WHITE_SHORT));
            Assert.IsFalse(ci.HasFlag(CastlingInfo.WHITE_LONG));
            Assert.IsFalse(ci.HasFlag(CastlingInfo.BLACK_SHORT));
            Assert.IsTrue(ci.HasFlag(CastlingInfo.BLACK_LONG));
        }

        [TestMethod()]
        public void ReadCastlingInfoFromFENTest_garbage()
        {
            FENParser parser = new FENParser();

            CastlingInfo ci = parser.ReadCastlingInfoFromFEN("sdes");

            Assert.AreEqual(ci, CastlingInfo.NONE);
        }

        [TestMethod()]
        public void ReadEnPassantTargetFromFENTest_input1()
        {
            FENParser parser = new FENParser();

            int? result = parser.ReadEnPassantTargetFromFEN("-");

            Assert.IsFalse(result.HasValue);
        }

        [TestMethod()]
        public void ReadEnPassantTargetFromFENTest_input2()
        {
            FENParser parser = new FENParser();

            int? result = parser.ReadEnPassantTargetFromFEN("f7");

            Assert.IsTrue(result.HasValue);
            Assert.AreEqual(53, result.Value);
        }

        [TestMethod()]
        public void ReadEnPassantTargetFromFENTest_input3()
        {
            FENParser parser = new FENParser();
            string msg = "";

            try
            {
                int? result = parser.ReadEnPassantTargetFromFEN("B4");
            }
            catch(ArgumentException ex)
            {
                msg = ex.Message;
            }

            Assert.AreEqual("Invalid algebraic square notation; could not parse file.", msg);
        }

        [TestMethod()]
        public void ReadHalfMovesFromFENTest_input1()
        {
            FENParser parser = new FENParser();

            Assert.AreEqual(4, parser.ReadHalfMovesFromFEN("4"));
        }

        [TestMethod()]
        public void ReadHalfMovesFromFENTest_input2()
        {
            FENParser parser = new FENParser();
            string msg = "";

            try
            {
                int result = parser.ReadHalfMovesFromFEN("e");
            }
            catch(ArgumentException ex)
            {
                msg = ex.Message;
            }

            Assert.AreEqual("Invalid FEN; could not parse half moves.", msg);
        }

        [TestMethod()]
        public void ReadFullMovesFromFENTest_input1()
        {
            FENParser parser = new FENParser();

            Assert.AreEqual(4, parser.ReadHalfMovesFromFEN("4"));
        }

        [TestMethod()]
        public void ReadFullMovesFromFENTest_input2()
        {
            FENParser parser = new FENParser();
            string msg = "";

            try
            {
                int result = parser.ReadFullMovesFromFEN("e");
            }
            catch (ArgumentException ex)
            {
                msg = ex.Message;
            }

            Assert.AreEqual("Invalid FEN; could not parse full moves.", msg);
        }
    }
}