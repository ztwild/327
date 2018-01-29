#include <stdint.h>
#include "dungeon.h"

typedef struct dungeon_t{
  uint8_t grid[X_LENGTH][Y_LENGTH];
  room_t **rooms;
  uint8_t room_count;
}dungeon_t;

void init_grid(dungeon_t *d){
  int x, y;
  for(x = 0; x < X_LENGTH; x++){
    for(y = 0; y < Y_LENGTH; y++){
      d->grid[x][y] = hardness;
    }
  }
}

void init_rooms(dungeon_t *d){
  int i;
  d->rooms = (room_t**)malloc(sizeof(room_t*) * ROOM_MIN);
  for(i = 0; i < ROOM_MIN; i++){
    d->rooms[i] = create_room();
  }
  
  while(rand() % 2){
    i++;
    d->rooms = (room_t**)realloc(d->rooms, sizeof(room_t*) * i);
    d->rooms[i-1] = create_room();
  }
  d->room_count = i;
}

void clear_rooms(dungeon_t *d){
  int i;
  for(i = 0; i < d->room_count; i++){
    free(d->rooms[i]);
  }
  free(d->rooms);
}

void clear_grid(dungeon_t *d){
  free(d->grid);
}

int rooms_valid(dungeon_t *d){
  int i, j;
  for(i = 0; i < d->room_count - 1; i++){
    for(j = i + 1; j < d->room_count; j++){
      int intersect = rooms_intersect(d->rooms[i], d->rooms[j]);
      if(intersect){
        // printf("Rooms %d & %d intersect\n", i, j);
        // room_info(d);
        return 0;
      }

    }
  }
  return 1;
}

void insert_rooms(dungeon_t *d){
  int a;
  for(a = 0; a < d->room_count; a++){
    room_t *r = d->rooms[a];
    int i = r->pos_x;
    int x = i + r->size_x;
    
    for(i; i < x; i++){
      int j = r->pos_y;
      int y = j + r->size_y;
      for(j; j < y; j++){
        d->grid[i][j] = ROOM;
      }
    }
  }
}

void insert_halls(dungeon_t *d){
  int i = 0;
  for(i = 0; i < d->room_count - 1; i++){
    room_t *r1 = d->rooms[i];
    room_t *r2 = d->rooms[i + 1];
    int x1 = r1->pos_x + rand() % r1->size_x;
    int y1 = r1->pos_y + rand() % r1->size_y;
    int x2 = r2->pos_x + rand() % r2->size_x;
    int y2 = r2->pos_y + rand() % r2->size_y;

    // printf("Starting from (%d, %d) to (%d, %d)\n", x1,y1, x2, y2);

    int x = x1 < x2 ? 1 : -1;
    int y = y1 < y2 ? 1 : -1;

    while(x1 != x2 || y1 != y2){
      if((rand() % 2 && x1 != x2) || y1 == y2){
        x1 = x1 + x;
      }else{
        y1 = y1 + y;
      }
      if(d->grid[x1][y1] != ROOM){
        d->grid[x1][y1] = HALL;
      }
    }
  }
}

void print_grid(dungeon_t *d){
  int x, y;
  for(y = 0; y < Y_LENGTH; y++){
    for(x = 0; x < X_LENGTH; x++){
      int n = d->grid[x][y];
      char c;
      if(n == 250){
        c = ' ';
      }
      else if(n == ROOM){
        c = '.';
      }
      else if(n == HALL){
        c = '#';
      }
      printf("%c", c);
    }
    printf("\n");
  }
}

void room_info(dungeon_t *d){
  int i;
  for(i = 0; i < d->room_count; i++){
    room_t *r = d->rooms[i];
    printf("Room %d at (%d, %d), size of %d x %d\n", i, r->pos_x, r->pos_y, r->size_x, r->size_y);
  }
}

dungeon_t *new_dungeon(){
  dungeon_t *dungeon = (dungeon_t*)malloc(sizeof(dungeon_t));
  init_grid(dungeon);
  init_rooms(dungeon);
  int valid = rooms_valid(dungeon);
  while(!valid){
    init_rooms(dungeon);
    valid = rooms_valid(dungeon);
  }

  insert_rooms(dungeon);
  insert_halls(dungeon);
  return dungeon;
}
