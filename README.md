# TuxedoCat

## About

TuxedoCat is a chess engine. I am developing it as a hobby; it isn't crazy
awesome like Crafty or Stockfish. The main purpose is simply the
entertainment value of creating a moderately complex system which does
something more or less impressive. TuxedoCat is compatible with version 2 of
the Winboard protocol; to download Winboard and read the documentation on
using an engine with it see
[http://www.tim-mann.org/xboard.html](http://www.tim-mann.org/xboard.html).

## Technical Goals

TuxedoCat is written in C++. The intention is to stick with standard C++ as
much as possible to facilitate portability. Simplicity and readability are also
priorities, hence there are no classes or other object-oriented programming
overhead. Also, TuxedoCat is not a clone so I try to avoid using other people's
code even if it is open source (of course, most of the search algorithms and
other logic, such as NegaMax and alpha-beta pruning, will be the standard
algorithms used by most other chess engines; the implementation
will be my own). So far the lone exception is some bit-twiddling code from the
chess programming wiki, which is acknowledged in the source files. Also, I may
eventually utilise freely available endgame tablebases or opening books, if it
doesn't seem worthwhile to develop my own. In the end, the goal for TuxedoCat is
to be educational and something I can be proud of, which isn't achievable by
copying large swaths of code from other engines.

## License

See the COPYING file.

## Building

The goal for TuxedoCat is to be portable; I've tried to use only standard C++
so this should be very possible but thus far I have only built it on Windows.
A .sln file is included in the repository so building the code should be as
simple as opening the solution in Visual Studio and building. Visual Studio
2015 is recommended, any version of 2015 should work however (I use the free
edition).

A Makefile for compiling on *nix should be coming soon.

## Usage

`tuxedocat`

The program doesn't accept any commandline parameters at the moment, so simply
invoke the executable from a shell or point Winboard to it. If you run the
engine on its own, you can use Winboard commands to interact with it. For
documentation on the Winboard protocal read the protocol
[description](http://www.open-aurec.com/wbforum/WinBoard/engine-intf.html),
particularly the section "Commands from xboard to the engine". Currently
TuxedoCat only reacts to the most essential commands, specifically

* `xboard`
* `quit`
* `protover`
* `new`
* `go`
* `playother`
* `ping`
* `setboard`
* `usermove`
* `force`

Notably, none of the commands dealing with clocks are implemented, as
TuxedoCat has no concept of time yet.

In addition to the regular Winboard commands, several other commands are
available if you run the engine separately. The commands and their usage are
as follows:

* `perft <depth>`

    perft measures the number of possible chess positions that can be reached
    after &lt;depth&gt; number of moves from the current position. For example, from
    the standard starting position there are 20 possible moves for White, so
    `perft 1` would be 20. For each of those 20 White moves Black has 20 possible
    responses, so `perft 2` (still from the starting position) would be 400
    (20 x 20). For depths greater than 2 it starts getting more complicated as
    there are different numbers of possible responses for each different move.
    Of course, perft can be calculated from positions other than the start
    position, too. 
    
    When you start TuxedoCat the board initializes to the standard starting
    position, so running perft immediately will calculate perft for that
    position. To calculate perft for another position, use the `setboard`
    command to setup a position and then run perft.
    
* `divide <depth>`

    Similar to perft, except that instead of simply returning the number of
    possible positions after &lt;depth&gt; moves, it lists all the possible moves at
    the top level and then calls `perft <depth - 1>` for each of those
    positions, thus "dividing" the total count into separate sums for each
    original move. Mainly useful for debugging perft.
    
* search

    Returns the best move found by the engine for the current position.

## Logging

TuxedoCat logs all communication between itself and Winboard/the user. The log
can be accessed by opening log.txt, located in the same folder as the
executable.

## Contact

To report bugs or provide other feedback you can contact me at
nathan.mccrina@outlook.com.