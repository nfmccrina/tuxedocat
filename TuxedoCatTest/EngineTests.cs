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
    public class EngineTests
    {
        [TestMethod()]
        public void Perft_StartPosition_1()
        {
            Engine engine = new Engine();

            Assert.AreEqual((ulong)20, engine.Perft(1));
        }

        [TestMethod()]
        public void Perft_StartPosition_2()
        {
            Engine engine = new Engine();

            Assert.AreEqual((ulong)400, engine.Perft(2));
        }

        [TestMethod()]
        public void Perft_StartPosition_3()
        {
            Engine engine = new Engine();

            Assert.AreEqual((ulong)8902, engine.Perft(3));
        }

        [TestMethod()]
        public void Perft_StartPosition_4()
        {
            Engine engine = new Engine();

            Assert.AreEqual((ulong)197281, engine.Perft(4));
        }

        [TestMethod()]
        public void Perft_StartPosition_5()
        {
            Engine engine = new Engine();

            Assert.AreEqual((ulong)4865609, engine.Perft(5));
        }

        [TestMethod()]
        public void Perft_Kiwipete_1()
        {
            Engine engine = new Engine();
            engine.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

            Assert.AreEqual((ulong)48, engine.Perft(1));
        }

        [TestMethod()]
        public void Perft_Kiwipete_2()
        {
            Engine engine = new Engine();
            engine.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

            Assert.AreEqual((ulong)2039, engine.Perft(2));
        }

        [TestMethod()]
        public void Perft_Kiwipete_3()
        {
            Engine engine = new Engine();
            engine.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

            Assert.AreEqual((ulong)97862, engine.Perft(3));
        }

        [TestMethod()]
        public void Perft_Kiwipete_4()
        {
            Engine engine = new Engine();
            engine.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

            Assert.AreEqual((ulong)4085603, engine.Perft(4));
        }

        /*[TestMethod()]
        public void Perft_Kiwipete_5()
        {
            Engine engine = new Engine();
            engine.SetBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

            Assert.AreEqual((ulong)193690690, engine.Perft(5));
        }*/

        [TestMethod()]
        public void Perft_Position3_1()
        {
            Engine engine = new Engine();
            engine.SetBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

            Assert.AreEqual((ulong)14, engine.Perft(1));
        }

        [TestMethod()]
        public void Perft_Position3_2()
        {
            Engine engine = new Engine();
            engine.SetBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

            Assert.AreEqual((ulong)191, engine.Perft(2));
        }

        [TestMethod()]
        public void Perft_Position3_3()
        {
            Engine engine = new Engine();
            engine.SetBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

            Assert.AreEqual((ulong)2812, engine.Perft(3));
        }

        [TestMethod()]
        public void Perft_Position3_4()
        {
            Engine engine = new Engine();
            engine.SetBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

            Assert.AreEqual((ulong)43238, engine.Perft(4));
        }

        [TestMethod()]
        public void Perft_Position3_5()
        {
            Engine engine = new Engine();
            engine.SetBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

            Assert.AreEqual((ulong)674624, engine.Perft(5));
        }

        [TestMethod()]
        public void Perft_Position3_6()
        {
            Engine engine = new Engine();
            engine.SetBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

            Assert.AreEqual((ulong)11030083, engine.Perft(6));
        }

        [TestMethod()]
        public void Perft_Position4_1()
        {
            Engine engine = new Engine();
            engine.SetBoard("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

            Assert.AreEqual((ulong)6, engine.Perft(1));
        }

        [TestMethod()]
        public void Perft_Position4_2()
        {
            Engine engine = new Engine();
            engine.SetBoard("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

            Assert.AreEqual((ulong)264, engine.Perft(2));
        }

        [TestMethod()]
        public void Perft_Position4_3()
        {
            Engine engine = new Engine();
            engine.SetBoard("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

            Assert.AreEqual((ulong)9467, engine.Perft(3));
        }

        [TestMethod()]
        public void Perft_Position4_4()
        {
            Engine engine = new Engine();
            engine.SetBoard("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

            Assert.AreEqual((ulong)422333, engine.Perft(4));
        }

        [TestMethod()]
        public void Perft_Position4_1_mirrored()
        {
            Engine engine = new Engine();
            engine.SetBoard("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");

            Assert.AreEqual((ulong)6, engine.Perft(1));
        }

        [TestMethod()]
        public void Perft_Position4_2_mirrored()
        {
            Engine engine = new Engine();
            engine.SetBoard("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");

            Assert.AreEqual((ulong)264, engine.Perft(2));
        }

        [TestMethod()]
        public void Perft_Position4_3_mirrored()
        {
            Engine engine = new Engine();
            engine.SetBoard("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");

            Assert.AreEqual((ulong)9467, engine.Perft(3));
        }

        [TestMethod()]
        public void Perft_Position4_4_mirrored()
        {
            Engine engine = new Engine();
            engine.SetBoard("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");

            Assert.AreEqual((ulong)422333, engine.Perft(4));
        }

        [TestMethod()]
        public void Perft_Position5_1()
        {
            Engine engine = new Engine();
            engine.SetBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

            Assert.AreEqual((ulong)44, engine.Perft(1));
        }

        [TestMethod()]
        public void Perft_Position5_2()
        {
            Engine engine = new Engine();
            engine.SetBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

            Assert.AreEqual((ulong)1486, engine.Perft(2));
        }

        [TestMethod()]
        public void Perft_Position5_3()
        {
            Engine engine = new Engine();
            engine.SetBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

            Assert.AreEqual((ulong)62379, engine.Perft(3));
        }

        [TestMethod()]
        public void Perft_Position5_4()
        {
            Engine engine = new Engine();
            engine.SetBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

            Assert.AreEqual((ulong)2103487, engine.Perft(4));
        }

        [TestMethod()]
        public void Perft_Position6_1()
        {
            Engine engine = new Engine();
            engine.SetBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

            Assert.AreEqual((ulong)46, engine.Perft(1));
        }

        [TestMethod()]
        public void Perft_Position6_2()
        {
            Engine engine = new Engine();
            engine.SetBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

            Assert.AreEqual((ulong)2079, engine.Perft(2));
        }

        [TestMethod()]
        public void Perft_Position6_3()
        {
            Engine engine = new Engine();
            engine.SetBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

            Assert.AreEqual((ulong)89890, engine.Perft(3));
        }

        [TestMethod()]
        public void Perft_Position6_4()
        {
            Engine engine = new Engine();
            engine.SetBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

            Assert.AreEqual((ulong)3894594, engine.Perft(4));
        }
    }
}