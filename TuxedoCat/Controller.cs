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
using System.Diagnostics;

namespace TuxedoCat
{
    public class Controller
    {
        public void Run()
        {
            Engine engine = new Engine();
            bool keepRunning = true;
            string input;
            string[] inputTokens;

            while (keepRunning)
            {
                Console.Write("TuxedoCat: ");
                input = Console.ReadLine();
                inputTokens = input.Split(new Char[] { ' ' });

                if (inputTokens.Length > 0)
                {
                    if (inputTokens[0].ToLower() == "quit"
                        || inputTokens[0].ToLower() == "exit")
                    {
                        keepRunning = false;
                    }
                    else if (inputTokens[0].ToLower() == "setboard")
                    {
                        if (inputTokens.Length < 2)
                        {
                            Console.WriteLine("Usage: setboard <FEN>");
                            continue;
                        }

                        try
                        {
                            engine.SetBoard(string.Join(" ", inputTokens, 1, inputTokens.Length - 1));
                        }
                        catch (ArgumentException)
                        {
                            Console.WriteLine("Error: Invalid FEN");

                            engine.SetBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                        }
                    }
                    else if (inputTokens[0].ToLower() == "perft")
                    {
                        Stopwatch stopwatch = new Stopwatch();

                        if (inputTokens.Length < 2)
                        {
                            Console.WriteLine("Usage: perft <depth>");
                            continue;
                        }

                        int depth = 0;

                        if (Int32.TryParse(inputTokens[1], out depth))
                        {
                            stopwatch.Reset();
                            stopwatch.Start();

                            ulong result = engine.Perft(depth);

                            stopwatch.Stop();

                            Console.WriteLine("Elapsed time: " + (stopwatch.ElapsedMilliseconds / 1000.0).ToString("N1"));
                            Console.WriteLine("perft (" + depth + "): " + result.ToString());
                        }
                        else
                        {
                            Console.WriteLine("Error: could not parse depth argument");
                            continue;
                        }
                    }
                    else if (inputTokens[0].ToLower() == "divide")
                    {
                        if (inputTokens.Length < 2)
                        {
                            Console.WriteLine("Usage: divide <depth>");
                            continue;
                        }

                        int depth = 0;

                        if (Int32.TryParse(inputTokens[1], out depth))
                        {
                            engine.Divide(depth);
                        }
                        else
                        {
                            Console.WriteLine("Error: could not parse depth argument");
                            continue;
                        }
                    }
                }
            }
        }
    }
}