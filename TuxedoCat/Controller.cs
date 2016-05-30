using System;
using System.Linq;

namespace TuxedoCat
{
    public class Controller
    {
        public void Run()
        {
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
                    else if (inputTokens[0].ToLower() == "perft")
                    {
                        if (inputTokens.Length < 3)
                        {
                            Console.WriteLine("Usage: perft <depth> <FEN>");
                            continue;
                        }

                        int depth = 0;

                        if (Int32.TryParse(inputTokens[1], out depth))
                        {
                            string fen = string.Join(" ", inputTokens, 2, inputTokens.Length - 2);
                            Position position = new Position(fen);
                            MoveGenerator moveGenerator = new MoveGenerator();

                            int result = moveGenerator.Perft(depth, position);

                            Console.WriteLine(result.ToString());
                        }
                        else
                        {
                            Console.WriteLine("Error: could not parse depth argument");
                            continue;
                        }
                    }
                    else if (inputTokens[0].ToLower() == "divide")
                    {
                        if (inputTokens.Length < 3)
                        {
                            Console.WriteLine("Usage: divide <depth> <FEN>");
                            continue;
                        }

                        int depth = 0;

                        if (Int32.TryParse(inputTokens[1], out depth))
                        {
                            string fen = string.Join(" ", inputTokens, 2, inputTokens.Length - 2);
                            Position position = new Position(fen);
                            MoveGenerator moveGenerator = new MoveGenerator();

                            moveGenerator.Divide(depth, position);
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