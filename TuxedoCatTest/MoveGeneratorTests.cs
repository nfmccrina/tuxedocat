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
    }
}