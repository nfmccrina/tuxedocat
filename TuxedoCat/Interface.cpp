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
#include <condition_variable>
#include <mutex>

using namespace TuxedoCat;

extern struct Board currentPosition;
static bool quitProgram;
static bool winboardMode;
static bool readyForInput;
static bool inputAvailable;
static bool showPrompt;
static bool forceMode;
static bool computerIsBlack;

static std::mutex readyForInputMutex;
static std::mutex inputAvailableMutex;
static std::mutex quitProgramMutex;

static std::string input;

void Interface::ReadInput()
{
	bool shouldGo;
	bool killThread;

	while (true)
	{
		readyForInputMutex.lock();
		shouldGo = readyForInput;
		readyForInput = false;
		readyForInputMutex.unlock();

		if (shouldGo)
		{
			if (showPrompt)
			{
				std::cout << "tuxedocat: ";
			}

			std::getline(std::cin, input);

			inputAvailableMutex.lock();
			inputAvailable = true;
			inputAvailableMutex.unlock();
		}

		quitProgramMutex.lock();
		killThread = quitProgram;
		quitProgramMutex.unlock();

		if (killThread)
		{
			break;
		}
	}
}

void Interface::OutputFeatures()
{
	std::stringstream features;

	features << "feature done=0" << std::endl;
	features << "feature setboard=1" << std::endl;
	features << "feature usermove=1" << std::endl;
	features << "feature playother=1" << std::endl;
	features << "feature analyze=0" << std::endl;
	features << "feature sigint=0" << std::endl;
	features << "feature sigterm=0" << std::endl;
	features << "feature time=0" << std::endl;
	features << "feature san=0" << std::endl;
	features << "feature ping=1" << std::endl;
	features << "feature myname=\"TuxedoCat\"" << std::endl;
	features << "feature variants=\"normal\"" << std::endl;
	features << "feature done=1" << std::endl;

	std::cout << features.str();
	Utility::WriteLog("engine -> interface: " + features.str());
}

void Interface::Run()
{
	readyForInput = true;
	inputAvailable = false;
	winboardMode = false;
	showPrompt = true;
	forceMode = false;
	bool computerIsBlack = true;
	bool shouldGo;
	std::string command;
	std::stringstream output;
	std::stringstream ss;

	Engine::InitializeEngine();

	std::thread inputThread(ReadInput);

	while (true)
	{
		inputAvailableMutex.lock();
		shouldGo = inputAvailable;
		inputAvailable = false;
		inputAvailableMutex.unlock();

		if (shouldGo)
		{
			ss.str(input);

			ss >> command;

			if (command == "quit")
			{
				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				quitProgramMutex.lock();
				quitProgram = true;
				quitProgramMutex.unlock();

				break;
			}
			else if (command == "xboard")
			{
				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				winboardMode = true;
				showPrompt = false;

				std::cout << std::endl;
			}
			else if (command == "protover")
			{
				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				OutputFeatures();
			}
			else if (command == "new")
			{
				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				Engine::InitializeEngine();
				forceMode = false;
			}
			else if (command == "go")
			{
				forceMode = false;

				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

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
						output << Engine::GetGameResult(currentPosition);
						std::cout << output.str() << std::endl;

						Utility::WriteLog("engine -> interface: " + output.str());
						output.clear();
						output.str("");
					}
				}
				else
				{
					if (Engine::IsGameOver(currentPosition))
					{
						output << Engine::GetGameResult(currentPosition);
						std::cout << output.str() << std::endl;

						Utility::WriteLog("engine -> interface: " + output.str());
						output.clear();
						output.str("");
					}
					else
					{
						output << "move " << move;
						std::cout << output.str() << std::endl;

						Utility::WriteLog("engine -> interface: " + output.str());
						output.clear();
						output.str("");
					}
				}
			}
			else if (command == "playother")
			{
				forceMode = false;

				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				if (currentPosition.ColorToMove == PieceColor::WHITE)
				{
					computerIsBlack = true;
				}
				else
				{
					computerIsBlack = false;
				}
			}
			else if (command == "ping")
			{
				int value;

				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				if (ss >> value)
				{
					output << "pong " << value;
					std::cout << output.str() << std::endl;

					Utility::WriteLog("engine -> interface: " + output.str());
					output.clear();
					output.str("");
				}
			}
			else if (command == "perft")
			{
				int depth;

				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				if (ss >> depth)
				{
					output << "Perft (" << depth << "): " << Engine::Perft(currentPosition, depth);
					std::cout << output.str() << std::endl;

					Utility::WriteLog("engine -> interface: " + output.str());
					output.clear();
					output.str("");
				}
				else
				{
					output << "Error: depth parameter required";
					std::cout << output.str() << std::endl;

					Utility::WriteLog("engine -> interface: " + output.str());
					output.clear();
					output.str("");
				}
			}
			else if (command == "divide")
			{
				int depth;

				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				if (ss >> depth)
				{
					Engine::Divide(currentPosition, depth);
				}
				else
				{
					output << "Error: depth parameter required";
					std::cout << output.str() << std::endl;

					Utility::WriteLog("engine -> interface: " + output.str());
					output.clear();
					output.str("");
				}
			}
			else if (command == "setboard")
			{
				std::string fenPart;
				std::stringstream fen;
				int fenPartCount = 0;

				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

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
						Utility::WriteLog("Error: Invalid FEN received");
					}
					else
					{
						output << "Error: Invalid FEN received";
						std::cout << output.str() << std::endl;

						Utility::WriteLog("engine -> interface: " + output.str());
						output.clear();
						output.str("");
					}
				}
				else
				{
					Position::SetPosition(currentPosition, fen.str());
				}
			}
			else if (command == "usermove")
			{
				std::string moveNotation;
				TuxedoCat::Move move;

				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				if (ss >> moveNotation)
				{
					if (moveNotation.length() < 4)
					{
						output << "Error (ambiguous move): " << moveNotation;
						std::cout << output.str() << std::endl;

						Utility::WriteLog("engine -> interface: " + output.str());
						output.clear();
						output.str("");
					}
					else
					{
						move = Utility::GetMoveFromXBoardNotation(currentPosition, moveNotation);

						if (move.TargetLocation == 0)
						{
							output << "Illegal move: " << moveNotation;
							std::cout << output.str() << std::endl;

							Utility::WriteLog("engine -> interface: " + output.str());
							output.clear();
							output.str("");
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
										output << Engine::GetGameResult(currentPosition);
										std::cout << output.str() << std::endl;

										Utility::WriteLog("engine -> interface: " + output.str());
										output.clear();
										output.str("");
									}
								}
								else
								{
									if (Engine::IsGameOver(currentPosition))
									{
										output << "move " << move;
										std::cout << output.str() << std::endl;

										Utility::WriteLog("engine -> interface: " + output.str());
										output.clear();
										output.str("");

										output << Engine::GetGameResult(currentPosition);
										std::cout << output.str() << std::endl;

										Utility::WriteLog("engine -> interface: " + output.str());
										output.clear();
										output.str("");
									}
									else
									{
										output << "move " << move;
										std::cout << output.str() << std::endl;

										Utility::WriteLog("engine -> interface: " + output.str());
										output.clear();
										output.str("");
									}
								}
							}
						}
					}
				}
			}
			else if (command == "force")
			{
				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				forceMode = true;
			}
			else if (command == "test")
			{
				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				Test::TestPerft();
			}
			else
			{
				output << "interface -> engine: " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");
			}

			ss.clear();
			ss.str("");
			output.clear();
			output.str("");
			input = "";
			command = "";

			readyForInputMutex.lock();
			readyForInput = true;
			readyForInputMutex.unlock();
		}
	}

	if (inputThread.joinable())
	{
		inputThread.join();
	}
}