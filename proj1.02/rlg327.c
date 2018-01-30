#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load_save.c"

#define MAX 100
#define SAVE "--save"
#define LOAD "--load"

int main(int argc, char **argv){
  int load, save = 0;
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
  if(save){
    save_dungeon(dungeon);    
  }
	
  return 0;
}
