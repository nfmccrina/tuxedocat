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

#include "../include/Interface.hpp"
#include "../include/MessageQueue.hpp"
#include "../include/Utility.hpp"
#include "../include/Messages.hpp"
#include <iostream>
#include <vector>

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

        Utility::split(input, ' ', cmdParts);

        if (cmdParts[0] == "quit")
        {
            messages.addMessage(QuitMessage());
            break;
        }

        if (cmdParts[0] == "test")
        {
            messages.addMessage(TestMessage());
        }
    }
}
