#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <iostream>
#include <pthread.h>

#include "environment.cpp"
#include "print.cpp"
#include "move.cpp"

void init_game();
void start_game();

environment *e;

int main(int argc, char *argv[]){
  srand(time(NULL));
  
  init_game();
  start_game();
  
  return 0;
}


void init_game(){
  e = new environment();
  e->init();
  e->q->enqueue((void*)e->PC);
  io_init_terminal();
}


void start_game(){
  int key, fail_code = 1;
  //int n = '\0';
  

  do{
    print_screen(e);
    //usleep(100000);
    character *next_char = (character*)e->q->dequeue();
    
    if(next_char->type == PC){
      switch(key = getch()){
      //switch(n){
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
        case 'a':
        case 's':
        case 'd':
        case 'w':
          //create_fire(e, key);
          break;
      }
      
      gravity_update(e);
      
      envr_type behind = e->environment_map[e->PC->pos[dim_y]][e->PC->pos[dim_x]];
      if(behind == END){
        e->character_map[e->PC->pos[dim_y]][e->PC->pos[dim_x]] = NULL;
        e->redraw();
        e->level++;
        if(e->level == 6) break;
        char *mess = (char*)malloc(sizeof(char) * 10);
        sprintf(mess, "LEVEL %d", e->level);
        print_message(mess);
        getch();
        free(mess);
      }
    }
    else if(next_char->alive && next_char->type == NPC){
      handle_npc(e, (npc*)next_char);
    }
    else if(next_char->alive && next_char->type == FIRE_BALL){
      handle_fire(e, (fire_ball*)next_char);
    }
  
    
    
    if(next_char->alive){
       e->q->enqueue((void*)next_char);
    }
    else{
      delete next_char;
    }
    if(e->PC->life < 0) fail_code = 0;
    
  }
  while(fail_code);
  
  
  if(e->PC->life < 0){
    print_message("GAME OVER!");
  }
  else{
    print_message("YOU WIN!");
  }
  getch();
  
  endwin();
  delete e;
}




