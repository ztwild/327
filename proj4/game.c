#include <ncurses.h>
#include <unistd.h>
#include "dungeon.c"
#include "load_save.c"
#include "path_find.c"


dungeon_t *dungeon;

void move_pc(){
  int ch = getch();
  pair_t *p = dungeon->pc;
  switch(ch){
  case KEY_LEFT:
    p->x = p->x - 1;
    break;
  case KEY_RIGHT:
    p->x = p->x + 1;
    break;
  case KEY_UP:
    p->y = p->y + 1;
    break;
  case KEY_DOWN:
    p->y = p->y - 1;
    break;
  }
  dungeon->pc = p;
}

void init_game(uint8_t load, uint8_t nummon){
  dungeon = load ? load_dungeon() : new_dungeon();
  dungeon->nummon = nummon;
  printf("There should be %d monsters\n", nummon);
  
  /** path_find **/
  dungeon->pc = rand_start(dungeon, PC);
  init_monsters(dungeon);
  init_path(dungeon);
  bfs(dungeon);
  dijkstra(dungeon);
}

int run_game(uint8_t save){
  uint8_t done = 0;
  //initscr();
  //noecho();
  //curs_set(FALSE);
  //while(!done){
    
    print_grid(dungeon);
    //i++;
    //printf("%d", i);
    //done = 1;
    //usleep(1000);
  //}
  //endwin();
  
  if(save){
    save_dungeon(dungeon);
  }
  return 0;
}

void print_test(){
  print_grid(dungeon);
  print_path_open(dungeon);
  print_path_wall(dungeon);
}
