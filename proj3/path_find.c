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

void bfs(dungeon_t *d, path_t *p, pair_t *start){
  //printf("Starting at (%d, %d)\n", start->x, start->y);
  bfs_rec(d, p, start, 0);  
}

int bfs_rec(dungeon_t *d, path_t *p, pair_t *pair, int count){
  int i, j;
  int x, y;
  
  int kill = d->hardness[pair->y][pair->x] == 255;
  //int kill = pair->x >= X_LENGTH - 1 || pair->y >= Y_LENGTH - 1;
  //kill |= pair->x <= 0 || pair->y <= 0;
    
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

void test(){
  int i, j, count;
  queue_t *q = init_queue();
  for(j = 0; j < Y_LENGTH; j++){
    for(i = 0; i < X_LENGTH; i++){
      count++;
      pair_t *p = create_pair(i, j);
      node_t *n = create_node((void*)p);
      enqueue(q, n);
      //printf("count: %d\n", count);
    }
  }
  printf("Size of queue: %d\n", q->size);
  
  node_t *n;
  while(n = dequeue(q)){
    pair_t *p = (pair_t*)n->value;
    free(p);
    free(n);
  }
  printf("Size of queue: %d\n", q->size);
}

void test2(dungeon_t *d, path_t *p, pair_t *start){
  int i, j;
  
  for(j = 0; j < Y_LENGTH; j++){
    for(i = 0; i < X_LENGTH; i++){
      int weight = d->grid[j][i] == WALL ? d->hardness[j][i] / 85 : 1;
      p->wall[j][i] = weight;  
    }
  }
  p->wall[start->y][start->x] = 0;
  
}

void dijkstra(dungeon_t *d, path_t *p, pair_t *start){
  int i, j, y, x;
  p->wall[start->y][start->x] = 0;
  printf("Setting starting position (%d, %d)\n", start->x, start->y);
  queue_t *q = init_queue();
  printf("Created Queue\n");
  enqueue(q, (void*)start);
  printf("Put start into queue\n");
  node_t *n;
  
  int count = 0;
  while(n = dequeue(q)){
    printf("Dequeued node #%d\n", count++);
    printf("Size of queue: %d\n", q->size);
    pair_t *pair = (pair_t*)n->value;
    printf("Extracted pair from node\n");
    int this_weight = p->wall[pair->y][pair->x];
    printf("The weight of this position is %d\n", this_weight);
    for(j = -1; j < 2; j++){
      for(i = -1; i < 2; i++){
        
        x = pair->x + i;
        y = pair->y + j;
        
        int weight = d->hardness[y][x] == 0 ? 1 : d->hardness[y][x] / 85;
        int in_bounds = d->hardness[y][x] < 255;
        
        if(in_bounds && this_weight + weight < p->wall[y][x]){
          printf("\tgot (%d, %d)\n", x, y);
          printf("\tWeight at (%d, %d) is %d\n", x, y, weight);
          printf("\tGrid (%d, %d) in bounds: %d\n", x, y, in_bounds);
          p->wall[y][x] = this_weight + weight;
          pair_t *temp = create_pair(x, y);
          enqueue(q, (void*)temp);
          printf("\tpushed (%d, %d) to queue\n\n", x, y);
        }
      }
    }
    free(pair);
    free(n);
  }
}

void print_path_wall(dungeon_t *d, path_t *p){
  int i, j;
  
  for(j = 0; j < Y_LENGTH; j++){
    for(i = 0; i < X_LENGTH; i++){
      if(p->wall[j][i] == 3){
        printf("X");
      }else if(p->wall[j][i] == 0 && d->grid[j][i] == ROOM){
        printf("@");
      }else if(p->wall[j][i] == 1 && (d->grid[j][i] == ROOM || d->grid[j][i] == HALL)){
        //printf(" ");
        printf("%d", p->wall[j][i]);
      }else if(d->grid[j][i] == WALL){
        printf("%d", p->wall[j][i]);
      }else{
        printf("?");
      }
    }
    printf("\n");
  }
}



