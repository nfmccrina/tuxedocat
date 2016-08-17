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
overhead. 

## License

See the COPYING file.

## Building

### Windows

A .sln file is included in the repository so building the code should be as
simple as opening the solution in Visual Studio and building. Visual Studio
2015 is recommended, any version of 2015 should work however (I use the free
edition).


### Linux

Simply cd into the TuxedoCat directory and run `make`. Tested with gcc 6.1 on
Fedora 24.

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
* `time`
* `otim`
* `level`
* `st`
* `sd`
* `random`
* `post`
* `nopost`
* `result`

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

## Logging

TuxedoCat logs all communication between itself and Winboard/the user. The log
can be accessed by opening log.txt, located either in the same directory as the
executable or in the working directory. There might be a difference between
Linux and Windows here.

## Bugs

None that I know of right now, except that the engine sucks. :)

## TODO
	
* Improve search

	Currently the search is pure brute-force; the next step is to implement
	alpha-beta pruning and halfway-intelligent move ordering to reduce the
	search space.


## Acknowledgements

Thanks to Norbert Leisner for his work on the amazing engine logos, and Tamás
Bajusz for providing a Linux build.

Some bit-twiddling code was copied from the chess programming wiki; all of this
code is clearly outlined in comments in the source files.

## Contact

To report bugs or provide other feedback you can contact me at
nathan.mccrina@outlook.com.