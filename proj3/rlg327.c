#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dungeon.c"
#include "load_save.c"
#include "path_find.c"

#define MAX 100
#define SAVE "--save"
#define LOAD "--load"

/**
void print_hardness(dungeon_t *d){
  int y, x;
  for(x = 0; x < X_LENGTH; x++){
    for(y = 0; y < Y_LENGTH; y++){
      printf("%3d ", d->hardness[y][x]);
    }
    printf("\n");
  }
}
**/

int main(int argc, char **argv){
  int load = 0, save = 0;
  dungeon_t *dungeon;
  srand(time(NULL));
  
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
  
  ///**
  path_t *path = (path_t*)malloc(sizeof(path_t));
  init_path(path);
  pair_t *start = pc_start(dungeon);
  bfs(dungeon, path, start);
  print_path_open(path);
  //**/
  
  //dijkstra(dungeon, path, start);
  //print_path_wall(path);
  
  //free(start);
  //print_hardness(dungeon);
  
  ///** Tests
  //test();
  test2(dungeon, path, start);
  print_path_wall(dungeon, path);
  //**/
  
  if(save){
    save_dungeon(dungeon);    
  }
  
	
  return 0;
}
