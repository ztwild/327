#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include <stdlib.h>
#include <ncurses.h>

#include <iostream>

#include "game.cpp"

int main(int argc, char *argv[]){
  srand(time(NULL));

  init_game();
  start_game();
  
  return 0;
}
