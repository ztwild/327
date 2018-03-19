#ifndef ROOM_H
#define ROOM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define X_MIN 3
#define Y_MIN 2
#define X_LENGTH 80
#define Y_LENGTH 21
#define MAX_LENGTH 12

class room_t{
 public:
   uint8_t pos_y;
   uint8_t pos_x;
   uint8_t size_y;
   uint8_t size_x;
   
   room_t *create_room();
   int rooms_intersect(room_t *, room_t *);
};



#endif

