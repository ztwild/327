LDFLAGS = -lncurses

rlg327: rlg327.c character.c game.c queue.c
	gcc -Wall -o $@ rlg327.c $(LDFLAGS)

clean:
	rm -f *.o rlg327 *.pgm

open: 
	gedit rlg327.c game.c game.h Makefile &


