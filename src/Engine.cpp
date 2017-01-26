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

#include "Engine.hpp"
#include "Move.hpp"
#include "MoveList.hpp"
#include "MoveSearchCriteria.hpp"
#include "SANEngine.hpp"
#include <sstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

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

                std::chrono::time_point<std::chrono::system_clock> start;
                std::chrono::time_point<std::chrono::system_clock> end;
                start = std::chrono::system_clock::now();

                std::cout << divide(depth);

                end = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = end - start;

                std::cout << " [" << elapsed_seconds.count()
                    << "s]" << std::endl;
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

                std::chrono::time_point<std::chrono::system_clock> start;
                std::chrono::time_point<std::chrono::system_clock> end;
                start = std::chrono::system_clock::now();

                std::cout << "Nodes searched: " << perft(depth);

                end = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = end - start;

                std::cout << " [" << elapsed_seconds.count()
                    << "s]" << std::endl;
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
    uint64_t count {0};
    position.generateMoves(Rank::NONE, availableMoves);

    if (depth <= 1)
    {
        return availableMoves.size();
    }
    else
    {
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

            output << availableMoves[index].notation << ": 1"
                << std::endl;
        }
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

            output <<
                SANEngine::calculateNotation(position, availableMoves[index])
                << ": " << localCount << std::endl;
        }
    }

    output << "Nodes searched: " << totalCount;

    return output.str();
}

void Engine::setboard(std::string fen)
{
    position = Position(fen);
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
                position.generateMoves(m.movingPiece.rank, ml);

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
