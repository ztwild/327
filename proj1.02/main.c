#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load_save.c"

#define MAX 100



int main(int argc, char **argv){
  
  if(argc != 2){
    fprintf(stderr, "Wrong number of parameters\n");
    return -1;
  }
  
  if(!is_equal(argv[1], "--save") && !is_equal(argv[1], "--load")){
    fprintf(stderr, "Do not recognize command. Use '--save' or '--load'\n");
    return -1;
  }
  
  srand(time(NULL));
  
  int count = 0;
  dungeon_t *dungeon = (dungeon_t*)malloc(sizeof(dungeon_t));
  init_grid(dungeon);
  init_rooms(dungeon);
  int valid = rooms_valid(dungeon);
  while(!valid){
    count++;
    clear_rooms(dungeon);
    init_rooms(dungeon);
    valid = rooms_valid(dungeon);
  }

  insert_rooms(dungeon);
  insert_halls(dungeon);
  print_grid(dungeon);
  printf("tried %d combinations\n", count);
  
  save(dungeon);
  
  clear_rooms(dungeon);
  init_grid(dungeon);
  load(dungeon);
  print_grid(dungeon);
	
  return 0;
}
