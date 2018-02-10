#include "path_find.h"

typedef struct path{
  uint8_t open[Y_LENGTH][X_LENGTH];
  uint8_t wall[Y_LENGTH][X_LENGTH];
}path_t;

typedef struct pair{
  uint8_t x;
  uint8_t y;
}pair_t;

pair_t *create_pair(int x, int y){
  pair_t *p = (pair_t*)malloc(sizeof(pair_t));
  p->x = x;
  p->y = y;
  return p;
}

void init_path(path_t *p){
  int x, y;
  for(x = 0; x < X_LENGTH; x++){
    for(y = 0; y < Y_LENGTH; y++){
      p->open[y][x] = 255;
      p->wall[y][x] = 255;
    }
  }
}

pair_t *pc_start(dungeon_t *d){
  uint8_t n;
  room_t *r;
  pair_t *p = (pair_t*)malloc(sizeof(pair_t));
  
  n = rand() % d->room_count;
  r = d->rooms[n];
  p->x = (rand() % r->size_x) + r->pos_x;
  p->y = (rand() % r->size_y) + r->pos_y;
  
  return p;
}

/**
  * Path finding through open areas
 **/

void bfs(dungeon_t *d, path_t *p, pair_t *start){
  bfs_rec(d, p, start, 0);  
}

int bfs_rec(dungeon_t *d, path_t *p, pair_t *pair, int count){
  int i, j;
  int x, y;
  
  int kill = d->hardness[pair->y][pair->x] == 255;
    
  if(kill){
    return 1;
  }
  p->open[pair->y][pair->x] = count;
  
  for(i = -1; i < 2; i++){
    for(j = -1; j < 2; j++){
      x = pair->x + i;
      y = pair->y + j;
      if(count + 1 < p->open[y][x] && d->grid[y][x] != WALL){
        
        pair_t *temp_pair = create_pair(x, y);
        int t = count + 1;
        bfs_rec(d, p, temp_pair, t);
        free(temp_pair);
      }
    }
  }
  return 0;
  
}

void print_path_open(path_t *p){
  int i, j;
  
  for(j = 0; j < Y_LENGTH; j++){
    for(i = 0; i < X_LENGTH; i++){
      if(p->open[j][i] == 255){
        printf(" ");
      }else if(p->open[j][i] == 0){
        printf("@");
      }else{
        printf("%d", p->open[j][i] % 10);
      }
    }
    printf("\n");
  }
}

/**
  * Path finding with walls
 **/


void dijkstra(dungeon_t *d, path_t *p, pair_t *start){
  int i, j, y, x;
  int unvisited = (X_LENGTH - 2) * (Y_LENGTH - 2) - 1;
  p->wall[start->y][start->x] = 0;
  pair_t *temp_pair = create_pair(start->x, start->y);
  queue_t *q = init_queue();
  enqueue(q, (void*)temp_pair);
  
  while(unvisited != 0){
    pair_t *pair = (pair_t*)dequeue(q);
    for(j = -1; j < 2; j++){
      for(i = -1; i < 2; i++){
        x = i + pair->x;
        y = j + pair->y;
        if(d->hardness[y][x] < 255){ //in bounds
          int weight = d->grid[y][x] != WALL ? 1 : d->hardness[y][x] / 85;
          weight += p->wall[pair->y][pair->x];
          if(p->wall[y][x] == 255){
            unvisited--;
            temp_pair = create_pair(x, y);
            enqueue(q, (void*)temp_pair);
          }
          p->wall[y][x] = min(weight, p->wall[y][x]);
        }
      }
    }
    free(pair);
  }
}

void print_path_wall(path_t *p, pair_t *pc){
  int i, j;
  
  for(j = 0; j < Y_LENGTH; j++){
    for(i = 0; i < X_LENGTH; i++){
      if(p->wall[j][i] == 255){
        printf(" ");
      }else if(pc->x == i && pc->y == j){
        printf("@");
      }else{
        printf("%d", p->wall[j][i] % 10);
      }
    }
    printf("\n");
  }
}


