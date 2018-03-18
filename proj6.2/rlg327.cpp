#include "rlg327.h"

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



