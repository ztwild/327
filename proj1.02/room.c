#include <stdint.h>
#include "room.h"


typedef struct room_t{
 uint8_t pos_y;
 uint8_t pos_x;
 uint8_t size_y;
 uint8_t size_x;
}room_t;

room_t *create_room(){
  room_t *r = (room_t*)malloc(sizeof(room_t));
  // r->size_x = (rand() % (X_LENGTH - X_MIN)) + X_MIN;
  // r->size_y = (rand() % (Y_LENGTH - Y_MIN)) + Y_MIN;
  r->size_x = (rand() % MAX_LENGTH) + X_MIN;
  r->size_y = (rand() % MAX_LENGTH) + Y_MIN;

  r->pos_x = rand() % (X_LENGTH - r->size_x);
  r->pos_y = rand() % (Y_LENGTH - r->size_y);
  return r;
}

int rooms_intersect(room_t *r1, room_t *r2){
  int x1 = (r1->pos_x - 1) > (r2->pos_x + r2->size_x + 1);
  int x2 = (r2->pos_x - 1) > (r1->pos_x + r1->size_x + 1);
  int y1 = (r1->pos_y - 1) > (r2->pos_y + r2->size_y + 1);
  int y2 = (r2->pos_y - 1) > (r1->pos_y + r1->size_y + 1);
  return !(x1 || x2 || y1 || y2);
}

