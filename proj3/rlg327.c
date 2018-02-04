#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dungeon.c"
#include "load_save.c"
#include "path_find.c"

#define MAX 100
#define SAVE "--save"
#define LOAD "--load"


int main(int argc, char **argv){
  int load = 0, save = 0;
  dungeon_t *dungeon;
  srand(time(NULL));
  
  //int t = time(NULL);
  //int t = 1517710594;
  //srand(t);
  //printf("Time seed is %d\n", t);
  
  if(argc > 3){
    fprintf(stderr, "Too many arguments\n");
    return 1;
  }
  else if(argc > 1){
    if(argc == 3){
      save = is_equal(argv[2], SAVE);
      load = is_equal(argv[2], LOAD);
    }
    save |= is_equal(argv[1], SAVE);
    load |= is_equal(argv[1], LOAD);
    if(!save && !load){
      fprintf(stderr, "Commands not recognized\n");
      return 1;
    }
    
    dungeon = load ? load_dungeon() : new_dungeon();
    
  }
  else{
    dungeon = new_dungeon();
  }
  
  print_grid(dungeon);
  
  /**
    * Path in open area
   **/
  path_t *path = (path_t*)malloc(sizeof(path_t));
  pair_t *start = pc_start(dungeon);
  init_path(path);
  bfs(dungeon, path, start);
  print_path_open(path);
  
  
  dijkstra(dungeon, path, start);
  print_path_wall(path, start);
  
  free(start);
  
  //print_hardness(dungeon);
  
  ///** Tests
  //test();
  
  //test2(dungeon, path, start);
  //print_path_wall(dungeon, path);
  
  //test3();
  //**/
  
  if(save){
    save_dungeon(dungeon);    
  }
  
	
  return 0;
}
