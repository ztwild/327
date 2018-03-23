#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "room.cpp"
#include "character.cpp"

#define HARDNESS 255
#define ROOM_MIN 5

typedef enum grid_t{
  HALL,
  WALL,
  ROOM,
  STRS_UP,
  STRS_DOWN,
  MON_1, MON_2, MON_3, MON_4,
  MON_5, MON_6, MON_7, MON_8,
  MON_9, MON_a, MON_b, MON_c,
  MON_d, MON_e, MON_f, PC_,
}grid_t;


class dungeon_t{
  public:
    uint8_t hardness[Y_LENGTH][X_LENGTH];
    grid_t grid[Y_LENGTH][X_LENGTH];
    char memory[Y_LENGTH][X_LENGTH];
    room_t **rooms;
    uint8_t room_count;
    
    uint8_t open[Y_LENGTH][X_LENGTH];
    uint8_t wall[Y_LENGTH][X_LENGTH];
    character_t *pc;
    
    character_t **monsters;
    uint8_t nummon;
    
    pair_t *rand_start(dungeon_t *, type_t);
    void init_grid(dungeon_t *);
    void init_rooms(dungeon_t *);
    void init_pc(dungeon_t *);
    void init_monsters(dungeon_t *);
    void clear_rooms(dungeon_t *);
    int rooms_valid(dungeon_t *);
    void insert_rooms(dungeon_t *);
    void insert_halls(dungeon_t *);
    void print_grid(dungeon_t *);
    void hardness_to_grid(dungeon_t *);
    void room_info(dungeon_t *);
    void place_stairs(dungeon_t *, type_t);
    dungeon_t *new_dungeon();
};





/**
  * Added this from solution 1
 **/

typedef struct queue_node queue_node_t;

static int smooth_hardness(dungeon_t *);

#endif


