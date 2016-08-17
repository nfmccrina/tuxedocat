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
#include <queue>
#include <mutex>

using namespace TuxedoCat;

extern struct Board currentPosition;
int maxSearchDepth;
bool randomMode;
bool showThinkingOutput;

static bool winboardMode;
static bool forceMode;
static bool computerIsBlack;
static std::queue<std::string> inputQueue;

static std::mutex inputQueueMutex;

void Interface::ReadInput()
{
	std::string userInput;

	while (true)
	{
		std::getline(std::cin, userInput);

		inputQueueMutex.lock();
		inputQueue.push(userInput);
		inputQueueMutex.unlock();

		if (userInput == "quit")
		{
			break;
		}
	}
}

void Interface::OutputFeatures()
{
	std::cout << "feature done=0" << std::endl;
	Utility::WriteLog("engine -> interface: feature done = 0");

	std::cout << "feature debug=1" << std::endl;
	Utility::WriteLog("engine -> interface: feature debug=1");

	std::cout << "feature setboard=1" << std::endl;
	Utility::WriteLog("engine -> interface: feature setboard=1");

	std::cout << "feature usermove=1" << std::endl;
	Utility::WriteLog("engine -> interface: feature usermove=1");

	std::cout << "feature playother=1" << std::endl;
	Utility::WriteLog("engine -> interface: feature playother=1");

	std::cout << "feature analyze=0" << std::endl;
	Utility::WriteLog("engine -> interface: feature analyze=0");

	std::cout << "feature sigint=0" << std::endl;
	Utility::WriteLog("engine -> interface: feature sigint=0");

	std::cout << "feature sigterm=0" << std::endl;
	Utility::WriteLog("engine -> interface: feature sigterm=0");

	std::cout << "feature nps=0" << std::endl;
	Utility::WriteLog("engine -> interface: feature nps=0");

	std::cout << "feature san=0" << std::endl;
	Utility::WriteLog("engine -> interface: feature san=0");

	std::cout << "feature ping=1" << std::endl;
	Utility::WriteLog("engine -> interface: feature ping=1");

	std::cout << "feature myname=\"TuxedoCat\"" << std::endl;
	Utility::WriteLog("engine -> interface: feature myname=\"TuxedoCat\"");

	std::cout << "feature variants=\"normal\"" << std::endl;
	Utility::WriteLog("engine -> interface: feature variants=\"normal\"");

	std::cout << "feature done=1" << std::endl;
	Utility::WriteLog("engine -> interface: feature done=1");
}

void Interface::Run()
{
	bool inputAvailable = false;
	winboardMode = false;
	forceMode = false;
	computerIsBlack = true;
	randomMode = false;
	showThinkingOutput = false;
	
	std::string input;
	std::string command;
	std::stringstream output;
	std::stringstream ss;

	Engine::InitializeEngine();

	std::thread inputThread(ReadInput);

	while (true)
	{
		inputAvailable = false;

		inputQueueMutex.lock();
		if (!inputQueue.empty())
		{
			inputAvailable = true;
			ss.str(inputQueue.front());
			inputQueue.pop();
		}
		inputQueueMutex.unlock();

		if (!inputAvailable)
		{
			continue;
		}

		input = ss.str();
		ss >> command;

		if (command == "quit")
		{
			output << "interface -> engine: " << input;
			Utility::WriteLog(output.str());
			output.clear();
			output.str("");

			break;
		}
		else if (command == "post")
		{
			output << "interface -> engine: " << input;
			Utility::WriteLog(output.str());
			output.clear();
			output.str("");

			showThinkingOutput = true;
		}
		else if (command == "nopost")
		{
			output << "interface -> engine: " << input;
			Utility::WriteLog(output.str());
			output.clear();
			output.str("");

			showThinkingOutput = false;
		}
		else if (command == "xboard")
		{
			output << "interface -> engine: " << input;
			Utility::WriteLog(output.str());
			output.clear();
			output.str("");

			winboardMode = true;

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
		else if (command == "random")
		{
			output << "interface -> engine: " << input;
			Utility::WriteLog(output.str());
			output.clear();
			output.str("");

			randomMode = !randomMode;
		}
		else if (command == "sd")
		{
			int depth;

			output << "interface -> engine: " << input;
			Utility::WriteLog(output.str());
			output.clear();
			output.str("");

			if (ss >> depth)
			{
				maxSearchDepth = depth;
			}
			else
			{
				output << "Could not parse depth argument " << input;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");
			}
		}
		else if (command == "time")
		{
			uint32_t timeValue;

			output << "interface -> engine: " << input;
			Utility::WriteLog(output.str());
			output.clear();
			output.str("");

			if (ss >> timeValue)
			{
				currentClock.remainingTime = timeValue;
			}
			else
			{
				Utility::WriteLog("Error: could not parse time value");
			}
		}
		else if (command == "otim")
		{
			output << "interface -> engine: " << input;
			Utility::WriteLog(output.str());
			output.clear();
			output.str("");
		}
		else if (command == "level")
		{
			uint32_t mpc;
			std::string remainingTime;
			uint32_t inc;
			bool invalidCommand = false;

			output << "interface -> engine: " << input;
			Utility::WriteLog(output.str());
			output.clear();
			output.str("");

			if (ss >> mpc)
			{
				currentClock.movesPerControl = mpc;
			}
			else
			{
				invalidCommand = true;
			}

			if (ss >> remainingTime)
			{
				if (remainingTime.find(':') == std::string::npos)
				{
					currentClock.remainingTime = std::stoi(remainingTime) * 6000;
				}
				else
				{
					uint32_t minutes = 0;
					uint32_t seconds = 0;

					minutes = std::stoi(remainingTime.substr(0, remainingTime.find(':')));
					seconds = std::stoi(remainingTime.substr(remainingTime.find(':') + 1));

					currentClock.remainingTime = ((minutes * 60) + seconds) * 100;
				}
			}
			else
			{
				invalidCommand = true;
			}

			if (ss >> inc)
			{
				if (inc == 0)
				{
					currentClock.timeIncrement = 0;
					currentClock.type = TimeControlType::CONVENTIONAL;
				}
				else
				{
					currentClock.timeIncrement = inc;
					currentClock.type = TimeControlType::INCREMENTAL;
				}
			}
			else
			{
				invalidCommand = true;
			}

			if (invalidCommand)
			{
				output << "Error (unknown command)";
				std::cout << output.str() << std::endl;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				currentClock.movesPerControl = 40;
				currentClock.remainingTime = 30000;
				currentClock.timeIncrement = 0;
				currentClock.type = TimeControlType::CONVENTIONAL;
			}
		}
		else if (command == "st")
		{
			uint32_t spm;

			output << "interface -> engine: " << input;
			Utility::WriteLog(output.str());
			output.clear();
			output.str("");

			if (ss >> spm)
			{
				currentClock.movesPerControl = 0;
				currentClock.remainingTime = spm;
				currentClock.timeIncrement = 0;
				currentClock.type = TimeControlType::TIME_PER_MOVE;
			}
			else
			{
				output << "Error (unknown command)";
				std::cout << output.str() << std::endl;
				Utility::WriteLog(output.str());
				output.clear();
				output.str("");

				currentClock.movesPerControl = 40;
				currentClock.remainingTime = 30000;
				currentClock.timeIncrement = 0;
				currentClock.type = TimeControlType::CONVENTIONAL;
			}
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

			std::string move = Engine::GetMove(currentPosition, currentClock);

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
							std::string move = Engine::GetMove(currentPosition, currentClock);

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
								else
								{
									Utility::WriteLog("entered wormhole");
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
		else if (command == "result")
		{
			output << "interface -> engine: " << input;
			Utility::WriteLog(output.str());
			output.clear();
			output.str("");

			forceMode = true;
			Engine::InitializeEngine();
		}
		else if (command == "test")
		{
			output << "interface -> engine: " << input;
			Utility::WriteLog(output.str());
			output.clear();
			output.str("");

			Test::RunTests();
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
	}

	if (inputThread.joinable())
	{
		inputThread.join();
	}

	Utility::WriteLog("Engine closing...");
}
