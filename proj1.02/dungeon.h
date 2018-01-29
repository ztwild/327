#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "room.c"

#define hardness 250
#define ROOM_MIN 5
#define ROOM 0
#define HALL 0

typedef struct dungeon_t dungeon_t;

void init_grid(dungeon_t *);

void init_rooms(dungeon_t *);

int rooms_valid(dungeon_t *);
  
void insert_rooms(dungeon_t *);

void insert_halls(dungeon_t *);
  
void print_grid(dungeon_t *);

void room_info(dungeon_t *);
 
