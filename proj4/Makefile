LDFLAGS = -lncurses

rlg327: rlg327.c
	gcc -Wall -o $@ rlg327.c $(LDFLAGS)

clean:
	rm -f *.o rlg327 *.pgm

open: 
	gedit rlg327.c path_find.c load_save.c dungeon.c room.c Makefile &


