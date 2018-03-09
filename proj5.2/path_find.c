#include "path_find.h"


void init_path(dungeon_t *d){
  int x, y;
  for(x = 0; x < X_LENGTH; x++){
    for(y = 0; y < Y_LENGTH; y++){
      d->open[y][x] = 255;
      d->wall[y][x] = 255;
    }
  }
}



/**
 * Path finding through open areas
 **/

void bfs(dungeon_t *d){
  pair_t *pair = d->pc->pos;
  bfs_rec(d, pair, 0);  
}

int bfs_rec(dungeon_t *d, pair_t *pair, int count){
  int i, j;
  int x, y;
  
  int kill = d->hardness[pair->y][pair->x] == 255;
    
  if(kill){
    return 1;
  }
  d->open[pair->y][pair->x] = count;
  
  for(i = -1; i < 2; i++){
    for(j = -1; j < 2; j++){
      x = pair->x + i;
      y = pair->y + j;
      if(count + 1 < d->open[y][x] && d->grid[y][x] != WALL){
        
        pair_t *temp_pair = create_pair(x, y);
        int t = count + 1;
        bfs_rec(d, temp_pair, t);
        free(temp_pair);
      }
    }
  }
  return 0;
  
}

void print_path_open(dungeon_t *d){
  int i, j;
  
  for(j = 0; j < Y_LENGTH; j++){
    for(i = 0; i < X_LENGTH; i++){
      if(d->open[j][i] == 255){
        printf(" ");
      }else if(d->open[j][i] == 0){
        printf("@");
      }else{
        printf("%d", d->open[j][i] % 10);
      }
    }
    printf("\n");
  }
}

/**
  * Path finding with walls
 **/


void dijkstra(dungeon_t *d){
  int i, j, y, x;
  int unvisited = (X_LENGTH - 2) * (Y_LENGTH - 2) - 1;
  //pair_t *start = d->pc->pos;
  x = d->pc->pos->x;
  y = d->pc->pos->y;
  d->wall[y][x] = 0;
  pair_t *temp_pair = create_pair(x, y);
  queue_t *q = init_queue();
  enqueue(q, (void*)temp_pair);
  
  while(unvisited != 0){
    pair_t *pair = (pair_t*)dequeue(q);
    for(j = -1; j < 2; j++){
      for(i = -1; i < 2; i++){
        x = i + pair->x;
        y = j + pair->y;
        if(d->hardness[y][x] < 255){ //in bounds
          int weight = (d->hardness[y][x] / 85) + 1;
          weight += d->wall[pair->y][pair->x];
          if(d->wall[y][x] == 255){
            unvisited--;
            temp_pair = create_pair(x, y);
            enqueue(q, (void*)temp_pair);
          }
          d->wall[y][x] = min(weight, d->wall[y][x]);
        }
      }
    }
    free(pair);
  }
}

void print_path_wall(dungeon_t *d){
  int i, j;
  
  for(j = 0; j < Y_LENGTH; j++){
    for(i = 0; i < X_LENGTH; i++){
      if(d->wall[j][i] == 255){
        printf(" ");
      }else if(d->wall[j][i] == 0){
        printf("@");
      }else{
        printf("%d", d->wall[j][i] % 10);
      }
    }
    printf("\n");
  }
}


