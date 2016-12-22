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
much as possible to facilitate portability. My objective is to write
"best-practice" code rather than quick and dirty hacks to get it working.
Also, given a choice between code readability and program performance I will
choose readability.

## License

See the COPYING file.

## Building

### Windows

I no longer intend to actively support building TuxedoCat for Windows, as
I don't have any Windows computers at home and it was becoming annoying
having to test every change on two different operating systems. However,
my plan is to stick with standard C++ so it should continue to be possible
to build Windows versions of TuxedoCat if you are sufficiently ambitious. :)

### Linux

cd into the toplevel directory and run

`tup init`
`tup`

The executable (named "tuxedocat") will be in the build/ directory.

## Usage

`tuxedocat`

The only thing this does right now (because of the refactor) is run the
unit tests.

## Logging

Temporarily unimplemented due to refactor.

## Bugs

No bugs per se, but in the middle of a refactor so nothing really "works".

## TODO
	
* Logging
* Move Generation
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
