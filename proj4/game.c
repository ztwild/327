#include <ncurses.h>
#include <unistd.h>
#include "dungeon.c"
#include "load_save.c"
#include "path_find.c"


dungeon_t *dungeon;


void init_game(uint8_t load, uint8_t nummon){
  dungeon = load ? load_dungeon() : new_dungeon();
  dungeon->nummon = nummon;
  printf("There should be %d monsters\n", nummon);
  
  /** path_find **/
  init_characters(dungeon);
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
