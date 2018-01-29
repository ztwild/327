#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load_save.c"

#define MAX 100



int main(int argc, char **argv){
  dungeon_t *dungeon;
  
  if(argc == 2 && !(is_equal(argv[1], "--save") || is_equal(argv[1], "--load"))){
    fprintf(stderr, "Do not recognize command. Use '--save' or '--load'\n");
    return -1;
  }
  
  if(argc == 3 && !(is_equal(argv[1], "--save") || is_equal(argv[1], "--load"))){
    fprintf(stderr, "\n");
    return -1;
  }
  srand(time(NULL));
  
  if(is_equal(argv[1], "--save")){
    dungeon = new_dungeon();
    save(dungeon);
  }
  else{
    load(dungeon);
  }
  
  print_grid(dungeon);
  //printf("tried %d combinations\n", count);
  
	
  return 0;
}
