LDFLAGS = -lncurses

rlg327: rlg327.cpp character.cpp game.cpp queue.cpp
	g++ -Wall -o $@ rlg327.cpp $(LDFLAGS)

clean:
	rm -f *.o rlg327 *.pgm

open: 
	gedit rlg327.cpp game.cpp game.h Makefile &


