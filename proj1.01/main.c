#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dungeon.c"
#include <windows.h>

int main(){
  srand(time(NULL));
  while(1){
    int x, y;
    int count = 0;
    dungeon_t *dungeon = (dungeon_t*)malloc(sizeof(dungeon_t));
    init_grid(dungeon);
    // srand(time(NULL));
    init_rooms(dungeon);
    int valid = rooms_valid(dungeon);
    while(!valid){
      count++;
      init_rooms(dungeon);
      valid = rooms_valid(dungeon);
    }

    insert_rooms(dungeon);
    print_grid(dungeon);
    printf("tried %d combinations\n", count);
    Sleep(500);
  }
  
  return 0;
}