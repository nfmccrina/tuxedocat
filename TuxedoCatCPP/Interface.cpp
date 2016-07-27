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
#include <mutex>

using namespace TuxedoCat;
using namespace TuxedoCat::Engine;

std::mutex inputAvailableMutex;
std::mutex stopMutex;

bool inputAvailable;
bool stopPolling;

void UI::Run()
{
	bool isRunning = true;
	stopPolling = false;
	inputAvailable = false;
	std::string command;

	InitializeEngine();

	std::thread t1{ Poll, std::ref(inputAvailable), std::ref(stopPolling) };

	while (isRunning)
	{
		inputAvailableMutex.lock();
		if (inputAvailable)
		{
			inputAvailable = false;
			inputAvailableMutex.unlock();

			std::getline(std::cin, command);

			if (command.substr(0, 4) == "quit")
			{
				isRunning = false;
			}
			else if (command.substr(0, 7) == "setboard")
			{
				SetPositionHandler(command);
			}
			else if (command.substr(0, 5) == "perft")
			{
				PerftHandler(command);
			}
		}
		else
		{
			inputAvailableMutex.unlock();
		}
	}

	stopMutex.lock();
	stopPolling = true;
	stopMutex.unlock();

	t1.join();
}

void UI::Poll(bool& inputStatus, bool& terminate)
{
	std::string str;
	bool quit = true;

	while (true)
	{
		if (std::cin.peek() != EOF)
		{
			inputAvailableMutex.lock();
			inputStatus = true;
			inputAvailableMutex.unlock();

			stopMutex.lock();
			if (terminate)
			{
				stopMutex.unlock();
				break;
			}
			stopMutex.unlock();
		}
	}
}