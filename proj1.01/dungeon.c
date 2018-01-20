#include "dungeon.h"

typedef struct dungeon_t{
  int grid[X_LENGTH][Y_LENGTH];
  room_t **rooms;
  int room_count;
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
        d->grid[i][j] = 0;
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
      else if(n == 0){
        c = '.';
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