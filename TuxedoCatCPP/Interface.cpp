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
	std::cout << "setboard=1" << std::endl;
	std::cout << "myname=TuxedoCat" << std::endl;
	std::cout << "variants=normal" << std::endl;
	std::cout << "done=1" << std::endl;
}

void Interface::Run()
{
	shouldQuitProgram = false;
	readyForInput = true;
	inputAvailable = false;
	winboardMode = false;
	showPrompt = true;
	std::string command;
	std::stringstream ss;

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
				Position::SetPosition(currentPosition, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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

		/*std::getline(std::cin, input);
		ss.str(input);
		std::getline(ss, command, ' ');

		if (command == "quit")
		{
			Utility::WriteLog("received: " + ss.str());
			isRunning = false;
		}
		else if (command == "xboard")
		{
			Utility::WriteLog("received: " + ss.str());

			winboardMode = true;
			std::cout << std::endl;
		}
		else if (command == "protover")
		{
			Utility::WriteLog("received: " + ss.str());

			std::cout << "feature setboard=1 myname=\"TuxedoCat\" done=1" << std::endl;
		}
		else if (command == "setboard")
		{
			std::string fen;

			Utility::WriteLog("received: " + ss.str());

			if (!ss.eof())
			{
				std::getline(ss, fen);
				Position::SetPosition(currentPosition, fen);
			}
			else
			{
				std::cout << "error: no position provided" << std::endl;
			}
		}
		else if (command == "perft")
		{
			int depth;

			if (!ss.eof())
			{
				ss >> depth;
			}

			uint64_t result = Engine::Perft(currentPosition, depth);

			std::cout << "Perft (" << depth << "): " << result << std::endl;
		}
		else if (command == "divide")
		{
			int depth;

			if (!ss.eof())
			{
				ss >> depth;
			}

			Engine::Divide(currentPosition, depth);
		}
		else if (command.substr(0, 4) == "test")
		{
			Test::TestPerft();
		}
		else if (ss.str().length() > 0)
		{
			Utility::WriteLog("received: " + ss.str());
		}*/
	}

	if (inputThread.joinable())
	{
		inputThread.join();
	}
}