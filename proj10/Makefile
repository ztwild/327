LDFLAGS = -lncurses

ssg: ssg.cpp character.cpp game.cpp pc.cpp environment.*
	g++ -Wall -o $@ ssg.cpp $(LDFLAGS)

clean:
	rm -f *.o ssg 

open: 
	gedit ssg.cpp game.cpp environment.cpp environment.h character.cpp pc.cpp Makefile &


