/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 Nathan McCrina
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

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

        public ulong Perft(int depth)
        {
            if (depth <= 1)
            {
                return (ulong)moveGenerator.GenerateMoves(ref currentPosition).Length;
            }
            else
            {
                ulong count = 0;
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
            ulong totalCount = 0;
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

                    ulong count = Perft(depth - 1);

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