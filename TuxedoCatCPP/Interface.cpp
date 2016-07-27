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

using namespace TuxedoCat;

extern struct Board currentPosition;

void UI::Run()
{
	bool isRunning = true;
	std::string command;
	std::string input;
	std::stringstream ss;

	Engine::InitializeEngine();

	while (isRunning)
	{
		command = "";
		input = "";
		ss.clear();

		std::cout << "tuxedocat: ";
		std::getline(std::cin, input);
		ss.str(input);
		std::getline(ss, command, ' ');

		if (command == "quit")
		{
			isRunning = false;
		}
		else if (command == "setboard")
		{
			std::string fen;

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
	}
}