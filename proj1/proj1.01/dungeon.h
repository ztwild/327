#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "room.c"

#define hardness 250
#define ROOM_MIN 5
#define ROOM 0
#define HALL -1

typedef struct dungeon_t dungeon_t;

void init_grid(dungeon_t *d);

void init_rooms(dungeon_t *d);

int rooms_valid(dungeon_t *d);
  
void insert_rooms(dungeon_t *d);

void insert_halls(dungeon_t *d);
  
void print_grid(dungeon_t *d);

void room_info(dungeon_t *d);
 