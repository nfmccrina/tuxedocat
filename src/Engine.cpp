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

#include "../include/Engine.hpp"
#include "../include/Move.hpp"
#include "../include/MoveList.hpp"
#include "../include/MoveSearchCriteria.hpp"
#include "../test_include/TuxedoCatTest.hpp"
#include <sstream>
#include <iostream>

using namespace TuxedoCat;

Engine::Engine(Position p, MessageQueue& mq)
    : position(p), messages(mq)
{
}

void Engine::start(MessageQueue& mq)
{
    Engine e {{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"},
        mq};

    e.run();
}

void Engine::run()
{
    while (true)
    {
        if (!messages.isQueueEmpty())
        {
            Message msg = messages.getNextMessage();
            MessageType msgType = msg.getType();

            if (msgType == MessageType::QUIT)
            {
                break;
            }
            else if (msgType == MessageType::TEST)
            {
                test();
            }
            else if (msgType == MessageType::DIVIDE)
            {
                const std::vector<MessageArgument>& args =
                    msg.getArguments();
                int depth = 0;

                for (auto it = args.cbegin(); it != args.cend(); it++)
                {
                    if (it->getName() == "depth")
                    {
                        depth = std::stoi(it->getValue());
                        break;
                    }
                }

                std::cout << divide(depth) << std::endl;
            }
            else if (msgType == MessageType::SETBOARD)
            {
                const std::vector<MessageArgument>& args =
                    msg.getArguments();

                for (auto it = args.cbegin(); it != args.cend(); it++)
                {
                    if (it->getName() == "fen")
                    {
                        setboard(it->getValue());
                        break;
                    }
                }
            }
            else if (msgType == MessageType::PRINT)
            {
                std::cout << print() << std::endl;
            }
            else if (msgType == MessageType::PERFT)
            {
                const std::vector<MessageArgument>& args =
                    msg.getArguments();
                int depth = 0;

                for (auto it = args.cbegin(); it != args.cend(); it++)
                {
                    if (it->getName() == "depth")
                    {
                        depth = std::stoi(it->getValue());
                        break;
                    }
                }

                std::cout << "Leaf nodes: " << perft(depth) << std::endl;
            }
            else if (msgType == MessageType::USERMOVE)
            {
                handleUserMoveMessage(msg);
            }
            else if (msgType == MessageType::UNDO)
            {
                position.unmakeMove();
            }
        }
    }
}

uint64_t Engine::perft(int depth)
{
    MoveList availableMoves;
    position.generateMoves(Rank::NONE, availableMoves);

    if (depth <= 1)
    {
        return availableMoves.size();
    }
    else
    {
        uint64_t count = 0;

        for (int moveIndex = 0; moveIndex < availableMoves.size();
            moveIndex++)
        {
            position.makeMove(availableMoves[moveIndex]);
            count += perft(depth - 1);
            position.unmakeMove();
        }

        return count;
    }
}

std::string Engine::divide(int depth)
{
    uint64_t totalCount = 0;
    int moveCount = 0;
    MoveList availableMoves;

    position.generateMoves(Rank::NONE, availableMoves);
    std::stringstream output;

    if (depth <= 1)
    {
        for (int index = 0; index < availableMoves.size(); index++)
        {
            totalCount++;
            moveCount++;

            output << availableMoves[index].getNotation() << ": 1"
                << std::endl;
        }

        output << "Leaf nodes: " << moveCount << std::endl;
    }
    else
    {
        for (int index = 0; index < availableMoves.size(); index++)
        {
            moveCount++;
            
            position.makeMove(availableMoves[index]);

            uint64_t localCount = perft(depth - 1);

            position.unmakeMove();

            totalCount += localCount;

            output << availableMoves[index].getNotation() << ": "
                << localCount << std::endl;
        }
    }

    return output.str();
}

void Engine::setboard(std::string fen)
{
    position = Position(fen);
}

void Engine::test() const
{
    int argc {1};
    char* argv[1];

    int result;

    argv[0] = new char[10];

    argv[0][0] = 't';
    argv[0][1] = 'u';
    argv[0][2] = 'x';
    argv[0][3] = 'e';
    argv[0][4] = 'd';
    argv[0][5] = 'o';
    argv[0][6] = 'c';
    argv[0][7] = 'a';
    argv[0][8] = 't';
    argv[0][9] = '\0';

    ::testing::InitGoogleTest(&argc, argv);
    result = RUN_ALL_TESTS();

    delete argv[0];
}

std::string Engine::print() const
{
    return position.toString();
}

void Engine::handleUserMoveMessage(const Message& msg)
{
    const std::vector<MessageArgument>& args = msg.getArguments();

    for (auto it = args.cbegin(); it != args.cend(); it++)
    {
        if (it->getName() == "move")
        {
            MoveList ml;
            Move m = position.getMoveFromString(it->getValue());


            if (!m.isValid())
            {
                std::cout << "Illegal move: " << it->getValue()
                    << std::endl;
            }
            else
            {
                position.generateMoves(m.getMovingPiece().getRank(), ml);

                if (ml.contains(m,
                    MoveSearchCriteria::MOVING_PIECE_COLOR |
                    MoveSearchCriteria::MOVING_PIECE_RANK |
                    MoveSearchCriteria::MOVING_PIECE_SQUARE |
                    MoveSearchCriteria::TARGET_SQUARE |
                    MoveSearchCriteria::PROMOTED_RANK, false))
                {
                    position.makeMove(m);
                }
                else
                {
                    std::cout << "Illegal move: " << it->getValue()
                        << std::endl;
                }
            }

            break;
        }
    }
}
