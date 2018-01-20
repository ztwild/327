#include <stdio.h>
#include <stdlib.h>
#include "dungeon.c"

room_t *set_room(int pos_x, int pos_y, int size_x, int size_y){
  room_t *r = create_room();
  r->pos_x = pos_x;
  r->pos_y = pos_y;
  r->size_x = size_x;
  r->size_y = size_y;
  return r;
}

int main(){
  room_t *r1 = set_room(0, 0, 5, 10);
  room_t *r2 = set_room(1, 7, 8, 5);
  
  int valid = !rooms_intersect(r1, r2);
  printf("the rooms positions are valid: %d\n", valid);

  r1 = set_room(1, 0, 9, 11);
  r2 = set_room(0, 7, 3, 2);
  
  valid = !rooms_intersect(r1, r2);
  printf("the rooms positions are valid: %d\n", valid);


  return 0;
}