#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "game.c"

#define MAX 100

int main(int argc, char *argv[]){
  srand(time(NULL));
  
  /** initialize ncurses **/
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(FALSE);

  init_game(argc, argv);
  run_game();
  
  endwin();
  	
  return 0;
}



