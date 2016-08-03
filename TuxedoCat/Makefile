CC=g++
CFLAGS=-std=c++14 -Werror -Wall -Wextra -O3
LDFLAGS=-pthread
OBJECTS=Controller.o Engine.o Interface.o Main.o Move.o \
	MoveGenerator.o PieceColor.o PieceRank.o Position.o Test.o Utility.o

all: $(OBJECTS)
		$(CC) -o TuxedoCat $(OBJECTS) $(LDFLAGS)

$(OBJECTS): %.o: %.cpp TuxedoCat.h
		$(CC) -c $(CFLAGS) $< -o $@

clean:
		rm -f *.o
		rm -f TuxedoCat
