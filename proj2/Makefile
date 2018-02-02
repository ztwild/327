rlg327: rlg327.c
	gcc -Wall -o rlg327 rlg327.c

clean:
	rm -f *.o rlg327

open: Makefile rlg327.c load_save.c dungeon.c room.c
	gedit Makefile rlg327.c load_save.c dungeon.c room.c &

test: test.c
	gcc -o test test.c
	./test > out
	rm test
