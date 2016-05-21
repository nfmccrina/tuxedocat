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
    public class TuxedoCatUtilityTests
    {
        [TestMethod()]
        public void GetPieceColorFromCharTest_input1()
        {
            TuxedoCatUtility util = new TuxedoCatUtility();

            Assert.AreEqual(PieceColor.BLACK, util.GetPieceColorFromChar('b'));
        }

        [TestMethod()]
        public void GetPieceColorFromCharTest_input2()
        {
            TuxedoCatUtility util = new TuxedoCatUtility();

            Assert.AreEqual(PieceColor.WHITE, util.GetPieceColorFromChar('K'));
        }

        [TestMethod()]
        public void GetSquareFromAlgebraicTest_input1()
        {
            TuxedoCatUtility util = new TuxedoCatUtility();

            Assert.AreEqual(43, util.GetSquareFromAlgebraic("d6"));
        }

        [TestMethod()]
        public void GetSquareFromAlgebraicTest_input2()
        {
            TuxedoCatUtility util = new TuxedoCatUtility();
            string msg = "";

            try
            {
                int square = util.GetSquareFromAlgebraic("E34");
            }
            catch(ArgumentException ex)
            {
                msg = ex.Message;
            }

            Assert.AreEqual("Invalid algebraic square notation; length was not two.", msg);
        }

        [TestMethod()]
        public void GetSquareFromAlgebraicTest_input3()
        {
            TuxedoCatUtility util = new TuxedoCatUtility();
            string msg = "";

            try
            {
                int square = util.GetSquareFromAlgebraic("E3");
            }
            catch (ArgumentException ex)
            {
                msg = ex.Message;
            }

            Assert.AreEqual("Invalid algebraic square notation; could not parse file.", msg);
        }

        [TestMethod()]
        public void GetSquareFromAlgebraicTest_input4()
        {
            TuxedoCatUtility util = new TuxedoCatUtility();
            string msg = "";

            try
            {
                int square = util.GetSquareFromAlgebraic("by");
            }
            catch (ArgumentException ex)
            {
                msg = ex.Message;
            }

            Assert.AreEqual("Invalid algebraic square notation; could not parse rank.", msg);
        }
    }
}