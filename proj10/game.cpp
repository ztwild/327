
#include "environment.cpp"
#include <string.h>

environment *e;

void print_message(const char *mess){
  clear();
  int y = (VIEW_Y + 2) / 2;
  int x = VIEW_X / 2 - strlen(mess) / 2;
  mvprintw(y, x, "%s", mess);
}

void io_init_terminal(){
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

void init_game(){
  e = new environment();
  e->init();
  io_init_terminal();
}



void start_game(){
  int key, fail_code = 1;

  do{
    print_screen(e);
    
    switch(key = getch()){
      case KEY_RIGHT:
      case KEY_LEFT:
      case KEY_UP:
      case KEY_DOWN:
        handle_pc(e, key);
        break;
      case 'Q':
        fail_code = 0;
        break;
      case ' ':
        handle_jump(e);
        break;
    }
    
    
    gravity_update(e);
    if(!e->PC->alive) fail_code = 0;
  }
  while(fail_code);
  
  
  if(!e->PC->alive){
    print_message("GAME OVER!");
  }
  else{
    print_message("YOU WIN!");
  }
  getch();
  
  endwin();
  delete e;
  
  cout << "\tend game" << endl;
}



