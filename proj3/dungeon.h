#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "room.c"

#define HARDNESS 255
#define ROOM_MIN 5

typedef enum grid_t{
  HALL,
  WALL,
  ROOM,
  PC
}grid_t;

typedef struct dungeon_t dungeon_t;

void init_grid(dungeon_t *);

void init_rooms(dungeon_t *);

void clear_rooms(dungeon_t *);

int rooms_valid(dungeon_t *);

void insert_rooms(dungeon_t *);

void insert_halls(dungeon_t *);

void print_grid(dungeon_t *);

void hardness_to_grid(dungeon_t *);

void room_info(dungeon_t *);

dungeon_t *new_dungeon();

static int smooth_hardness(dungeon_t *);
