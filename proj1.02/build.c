#include <stdio.h>
#include <stdlib.h>
#include "load_save.c"

dungeon_t *new_dungeon(){
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
  
  return 1;
}


