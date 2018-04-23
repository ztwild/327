LDFLAGS = -lncurses

ssg: ssg.cpp character.cpp pc.cpp environment.* move.cpp print.cpp
	g++ -Wall -o $@ ssg.cpp $(LDFLAGS)

clean:
	rm -f *.o ssg 

open: 
	gedit ssg.cpp  environment.cpp environment.h character.cpp pc.cpp Makefile &


