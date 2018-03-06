#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "game.c"


#define MAX 100
#define SAVE "--save"
#define LOAD "--load"
#define NUMMON "--nummon"

uint8_t load, save, nummon;

void get_switches(int, char*[]);

int main(int argc, char *argv[]){
  srand(time(NULL));
  nummon = 10;
  get_switches(argc, argv);
  
  /** initialize ncurses **/
  initsrc();
  noecho();
  

  init_game(load, nummon);
  
  run_game(save);

  endwin();
  	
  return 0;
}

void get_switches(int argc, char *argv[]){
  int i;
  for(i = 0; i < argc; i++){
    if(is_equal(argv[i], SAVE)){
      save = 1;
    }
    else if(is_equal(argv[i], LOAD)){
      load = 1;
    }
    else if(is_equal(argv[i], NUMMON) && atoi(argv[i+1])){
      nummon = atoi(argv[++i]);
    }
  }
  
}

