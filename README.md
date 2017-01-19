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
much as possible to facilitate portability. I'm experimenting with C++11 and
trying to stick with an object-oriented approach as long as it doesn't hurt
the performance too much.

## License

See the COPYING file.

## Building

### Windows

There is a Visual Studio solution which will build the code, but Windows is
a second-class citizen so it is not always up to date when I'm making lots of
changes. Any time the code is release-worthy though I will make sure it can be
built on Windows.

### Linux

TuxedoCat used Tup rather than make as a build system, so you'll need that
unless you want to compile it manually.

Instructions for installing Tup can be found at http://gittup.org/tup/index.html

To build TuxedoCat, cd into the toplevel directory and run

`tup init` (if building for the first time)

`tup`

The executable (named "tuxedocat") will be in the build/ directory.

## Usage

`tuxedocat`

### Commands

TuxedoCat responds to commands from the CECP (Chess Engine Communication
Protocol, or 'Winboard' protocol) protocol. Only a couple of the important ones
are implemented right now.

perft <depth>: Runs a move generation test from the current position, giving
the number of unique "games" that can be played from the position to the given
depth.

divide <depth>: Similar to perft, but prints a list of the root moves and how
many of the perft branches originate from each move.

## Logging

Temporarily unimplemented due to refactor.

## Bugs

No bugs per se, but in the middle of a refactor so nothing really "works".

## TODO
	
* Logging
* Move Generation optimization
* protocol implementation


## Acknowledgements

Thanks to Norbert Leisner for his work on the amazing engine logos,
and Tamás Bajusz for providing a Linux build.

Some bit-twiddling code was copied from the chess programming wiki; all of
this code is clearly outlined in comments in the source files.

TuxedoCat uses GoogleTest for unit tests. I build it as a static library
and included that and the header files in the TuxedoCat repository.

## Contact

To report bugs or provide other feedback you can contact me at
nathan.mccrina@outlook.com.
