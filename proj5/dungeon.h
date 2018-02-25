#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "room.c"
#include "character.c"

#define HARDNESS 255
#define ROOM_MIN 5

typedef enum grid_t{
  HALL,
  WALL,
  ROOM,
  STRS_UP,
  STRS_DOWN
}grid_t;


typedef struct dungeon_t dungeon_t;

pair_t *create_pair(int, int);

pair_t *rand_start(dungeon_t *, type_t);

void init_grid(dungeon_t *);

void init_rooms(dungeon_t *);

void init_characters(dungeon_t *);

void clear_rooms(dungeon_t *);

int rooms_valid(dungeon_t *);

void insert_rooms(dungeon_t *);

void insert_halls(dungeon_t *);
  
void print_grid(dungeon_t *);

void hardness_to_grid(dungeon_t *);

void room_info(dungeon_t *);

void place_stairs(dungeon_t *, type_t);

dungeon_t *new_dungeon();


/**
  * Added this from solution 1
 **/

typedef struct queue_node queue_node_t;

static int smooth_hardness(dungeon_t *);

#endif


