rlg327: rlg327.c
	gcc -Wall -o rlg327 rlg327.c

clean:
	rm -f *.o rlg327 *.pgm

open: 
	gedit rlg327.c path_find.c load_save.c dungeon.c room.c Makefile &

test: test.c
	gcc -o test test.c

run: rlg327
	./rlg327
