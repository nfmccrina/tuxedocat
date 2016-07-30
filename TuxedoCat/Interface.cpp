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

#include "TuxedoCat.h"
#include <iostream>
#include <sstream>
#include <thread>

using namespace TuxedoCat;

extern struct Board currentPosition;
static bool winboardMode;
static bool shouldQuitProgram;
static bool readyForInput;
static bool inputAvailable;
static bool showPrompt;
static bool forceMode;
static bool computerIsBlack;

static std::string input;

void Interface::ReadInput()
{
	while (true)
	{
		if (readyForInput)
		{
			readyForInput = false;

			if (showPrompt)
			{
				std::cout << "tuxedocat: ";
			}

			std::getline(std::cin, input);

			inputAvailable = true;
		}

		if (shouldQuitProgram)
		{
			break;
		}
	}
}

void Interface::OutputFeatures()
{
	std::cout << "feature done=0" << std::endl;
	std::cout << "feature setboard=1" << std::endl;
	std::cout << "feature usermove=1" << std::endl;
	std::cout << "feature playother=1" << std::endl;
	std::cout << "feature analyze=0" << std::endl;
	std::cout << "feature sigint=0" << std::endl;
	std::cout << "feature sigterm=0" << std::endl;
	std::cout << "feature time=0" << std::endl;
	std::cout << "feature san=0" << std::endl;
	std::cout << "feature ping=1" << std::endl;
	std::cout << "feature myname=\"TuxedoCat\"" << std::endl;
	std::cout << "feature variants=\"normal\"" << std::endl;
	std::cout << "feature done=1" << std::endl;
}

void Interface::Run()
{
	shouldQuitProgram = false;
	readyForInput = true;
	inputAvailable = false;
	winboardMode = false;
	showPrompt = true;
	forceMode = false;
	static bool computerIsBlack = true;
	std::string command;
	std::stringstream ss;

	Engine::InitializeEngine();

	std::thread inputThread(ReadInput);

	while (!shouldQuitProgram)
	{
		if (inputAvailable)
		{
			inputAvailable = false;

			ss.str(input);

			ss >> command;

			if (command == "quit")
			{
				Utility::WriteLog("received " + input);
				shouldQuitProgram = true;
			}
			else if (command == "xboard")
			{
				Utility::WriteLog("received " + input);
				
				winboardMode = true;
				showPrompt = false;

				std::cout << std::endl;

				readyForInput = true;
			}
			else if (command == "protover")
			{
				Utility::WriteLog("received " + input);

				OutputFeatures();

				readyForInput = true;
			}
			else if (command == "new")
			{
				Engine::InitializeEngine();
				forceMode = false;
				readyForInput = true;
			}
			else if (command == "go")
			{
				forceMode = false;

				if (currentPosition.ColorToMove == PieceColor::WHITE)
				{
					computerIsBlack = false;
				}
				else
				{
					computerIsBlack = true;
				}

				std::string move = Engine::GetMove(currentPosition);

				if (move == "")
				{
					if (Engine::IsGameOver(currentPosition))
					{
						std::cout << Engine::GetGameResult(currentPosition) << std::endl;
					}
				}
				else
				{
					if (Engine::IsGameOver(currentPosition))
					{
						std::cout << Engine::GetGameResult(currentPosition) << std::endl;
					}
					else
					{
						std::cout << "move " << move << std::endl;
					}
				}

				readyForInput = true;
			}
			else if (command == "playother")
			{
				forceMode = false;

				if (currentPosition.ColorToMove == PieceColor::WHITE)
				{
					computerIsBlack = true;
				}
				else
				{
					computerIsBlack = false;
				}

				readyForInput = true;
			}
			else if (command == "ping")
			{
				int value;

				Utility::WriteLog("received " + input);

				if (ss >> value)
				{
					std::cout << "pong " << value << std::endl;
				}

				readyForInput = true;
			}
			else if (command == "perft")
			{
				int depth;
				uint64_t result;

				Utility::WriteLog("received " + input);

				if (ss >> depth)
				{
					result = Engine::Perft(currentPosition, depth);
					std::cout << "Perft (" << depth << "): " << result << std::endl;
				}
				else
				{
					std::cout << "Error: depth parameter required" << std::endl;
				}

				readyForInput = true;
			}
			else if (command == "divide")
			{
				int depth;

				Utility::WriteLog("received " + input);

				if (ss >> depth)
				{
					Engine::Divide(currentPosition, depth);
				}
				else
				{
					std::cout << "Error: depth parameter required" << std::endl;
				}

				readyForInput = true;
			}
			else if (command == "setboard")
			{
				std::string fenPart;
				std::stringstream fen;
				int fenPartCount = 0;

				Utility::WriteLog("received: " + ss.str());

				while (fenPartCount < 6)
				{
					if (ss >> fenPart)
					{
						fen << fenPart;

						if (fenPartCount < 5)
						{
							fen << " ";
						}

						fenPartCount++;
					}
					else
					{
						break;
					}
				}

				if (fenPartCount < 6)
				{
					if (winboardMode)
					{
						Utility::WriteLog("received invalid position");
					}
					else
					{
						std::cout << "Error: Invalid FEN received" << std::endl;
					}
				}
				else
				{
					Position::SetPosition(currentPosition, fen.str());
				}

				readyForInput = true;
			}
			else if (command == "usermove")
			{
				std::string moveNotation;
				TuxedoCat::Move move;

				Utility::WriteLog("received " + input);

				if (ss >> moveNotation)
				{
					if (moveNotation.length() < 4)
					{
						std::cout << "Error (ambiguous move): " << moveNotation << std::endl;
					}
					else
					{
						move = Utility::GetMoveFromXBoardNotation(currentPosition, moveNotation);

						if (move.TargetLocation == 0)
						{
							std::cout << "Illegal move: " << moveNotation << std::endl;
						}
						else
						{
							Position::Make(currentPosition, move);

							if (!forceMode)
							{
								std::string move = Engine::GetMove(currentPosition);

								if (move == "")
								{
									if (Engine::IsGameOver(currentPosition))
									{
										std::cout << Engine::GetGameResult(currentPosition) << std::endl;
									}
								}
								else
								{
									if (Engine::IsGameOver(currentPosition))
									{
										std::cout << Engine::GetGameResult(currentPosition) << std::endl;
									}
									else
									{
										std::cout << "move " << move << std::endl;
									}
								}
							}
						}
					}
				}

				readyForInput = true;
			}
			else if (command == "force")
			{
				Utility::WriteLog("received " + input);

				forceMode = true;
				readyForInput = true;
			}
			else if (command == "test")
			{
				Utility::WriteLog("received " + input);

				Test::TestPerft();
			}
			else
			{
				Utility::WriteLog("received " + input);

				readyForInput = true;
			}

			ss.clear();
			input = "";
			command = "";
		}
	}

	if (inputThread.joinable())
	{
		inputThread.join();
	}
}