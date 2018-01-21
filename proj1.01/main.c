#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dungeon.c"

int main(){
  srand(time(NULL));
  int x, y;
  int count = 0;
  dungeon_t *dungeon = (dungeon_t*)malloc(sizeof(dungeon_t));
  init_grid(dungeon);
  init_rooms(dungeon);
  int valid = rooms_valid(dungeon);
  while(!valid){
    count++;
    init_rooms(dungeon);
    valid = rooms_valid(dungeon);
  }

  insert_rooms(dungeon);
  insert_halls(dungeon);
  print_grid(dungeon);
  printf("tried %d combinations\n", count);
  
  return 0;
}