/*
* The MIT License (MIT)
*
* Copyright (c) 2016 Nathan McCrina
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation 
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/

#include "Interface.hpp"
#include "MessageQueue.hpp"
#include "Utility.hpp"
#include "Messages.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace TuxedoCat;

Interface::Interface(MessageQueue& mq)
    : messages(mq)
{
}

void Interface::start(MessageQueue& mq)
{
    Interface i {mq};

    i.run();
}

void Interface::run()
{
    while (true)
    {
        std::string input;
        std::vector<std::string> cmdParts;
        
        std::getline(std::cin, input);

        if (input == "")
        {
            continue;
        }

        Utility::split(input, ' ', cmdParts);

        if (cmdParts[0] == "quit")
        {
            messages.addMessage(QuitMessage());
            break;
        }
        else if (cmdParts[0] == "test")
        {
            messages.addMessage(TestMessage());
        }
        else if (cmdParts[0] == "divide")
        {
            if (cmdParts.size() > 1)
            {
                try
                {
                    messages.addMessage(DivideMessage(
                        std::stoi(cmdParts[1])));
                }
                catch (std::invalid_argument)
                {
                    std::cout << "Invalid argument to divide command: "
                        << cmdParts[1] << std::endl;
                }
                catch (std::out_of_range)
                {
                    std::cout << "Invalid argument to divide command: "
                        << cmdParts[1] << std::endl;
                }
            }
            else
            {
                std::cout <<
                    "Error (Not enough arguments to divide command): "
                    << input << std::endl;
            }
        }
        else if (cmdParts[0] == "setboard")
        {
            if (cmdParts.size() > 6)
            {
                std::stringstream ss;

                ss << cmdParts[1] << " ";
                ss << cmdParts[2] << " ";
                ss << cmdParts[3] << " ";
                ss << cmdParts[4] << " ";
                ss << cmdParts[5] << " ";
                ss << cmdParts[6];

                messages.addMessage(SetBoardMessage(ss.str()));
            }
            else
            {
                std::cout <<
                    "Error (Not enough arguments to setboard command): "
                    << input << std::endl;
            }
        }
        else if (cmdParts[0] == "print")
        {
            messages.addMessage(PrintMessage());
        }
        else if (cmdParts[0] == "perft")
        {
            if (cmdParts.size() > 1)
            {
                try
                {
                    messages.addMessage(PerftMessage(
                        std::stoi(cmdParts[1])));
                }
                catch (std::invalid_argument)
                {
                    std::cout << "Invalid argument to perft command: "
                        << cmdParts[1] << std::endl;
                }
                catch (std::out_of_range)
                {
                    std::cout << "Invalid argument to perft command: "
                        << cmdParts[1] << std::endl;
                }
            }
            else
            {
                std::cout <<
                    "Error (Not enough arguments to perft command): "
                    << input << std::endl;
            }
        }
        else if (cmdParts[0] == "usermove")
        {
            if (cmdParts.size() > 1)
            {
                messages.addMessage(UserMoveMessage(cmdParts[1]));
            }
            else
            {
                std::cout <<
                    "Error (Not enough arguments to usermove command): "
                    << input << std::endl;
            }
        }
        else if (cmdParts[0] == "undo")
        {
            messages.addMessage(UndoMoveMessage());
        }
        else
        {
            messages.addMessage(UserMoveMessage(cmdParts[0]));
        }
    }
}
