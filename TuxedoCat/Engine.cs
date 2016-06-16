using System;
using System.Linq;

namespace TuxedoCat
{
    public class Engine
    {
        private MoveGenerator moveGenerator;
        private Position currentPosition;

        public Engine()
        {
            moveGenerator = new MoveGenerator();
            currentPosition = new Position();
        }

        public UInt64 Perft(int depth)
        {
            if (depth <= 1)
            {
                return (UInt64)moveGenerator.GenerateMoves(ref currentPosition).Length;
            }
            else
            {
                UInt64 count = 0;
                Move[] availableMoves = moveGenerator.GenerateMoves(ref currentPosition);

                foreach (Move move in availableMoves)
                {
                    currentPosition.Make(move);
                    count += Perft(depth - 1);
                    currentPosition.Unmake(move);
                }

                return count;
            }
        }

        public void SetBoard(string fen)
        {
            currentPosition.SetPosition(fen);
        }

        public void Divide(int depth)
        {
            SANParser sanParser = new SANParser();
            Move[] availableMoves;
            UInt64 totalCount = 0;
            int moveCount = 0;

            if (depth <= 1)
            {
                availableMoves = moveGenerator.GenerateMoves(ref currentPosition);

                foreach (Move move in availableMoves)
                {
                    string moveSAN = sanParser.GenerateSAN(ref currentPosition, move, availableMoves.ToList());

                    moveCount++;
                    totalCount++;
                    Console.WriteLine(moveSAN + ": 1");
                }
            }
            else
            {
                availableMoves = moveGenerator.GenerateMoves(ref currentPosition);


                foreach (Move move in availableMoves)
                {
                    string moveSAN = sanParser.GenerateSAN(ref currentPosition, move, availableMoves.ToList());

                    moveCount++;

                    currentPosition.Make(move);

                    UInt64 count = Perft(depth - 1);

                    totalCount += count;
                    Console.WriteLine(moveSAN + ": " + count.ToString());
                    currentPosition.Unmake(move);
                }
            }

            Console.WriteLine();
            Console.WriteLine("Moves: " + moveCount);
            Console.WriteLine("Total nodes: " + totalCount);
        }
    }
}