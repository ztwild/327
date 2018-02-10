#include "dungeon.h"

typedef struct dungeon_t{
  uint8_t hardness[Y_LENGTH][X_LENGTH];
  grid_t grid[Y_LENGTH][X_LENGTH];
  room_t **rooms;
  uint8_t room_count;
}dungeon_t;

void init_grid(dungeon_t *d){
  smooth_hardness(d);
  int x, y;
  for(x = 0; x < X_LENGTH; x++){
    for(y = 0; y < Y_LENGTH; y++){
      if(x == 0 || y == 0){
        d->hardness[y][x] = 255;
      }
      else if(x == (X_LENGTH - 1) || y == (Y_LENGTH - 1)){
        d->hardness[y][x] = 255;
      }
      d->grid[y][x] = WALL;
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

void *set_pc(dungeon_t *d, int x, int y){
  d->grid[y][x] = PC;
}

void clear_rooms(dungeon_t *d){
  int i;
  for(i = 0; i < d->room_count; i++){
    free(d->rooms[i]);
  }
  free(d->rooms);
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
    
    for(i = i; i < x; i++){
      int j = r->pos_y;
      int y = j + r->size_y;
      for(j = j; j < y; j++){
        d->grid[j][i] = ROOM;
        d->hardness[j][i] = 0;
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
      if(d->grid[y1][x1] != ROOM){
        d->grid[y1][x1] = HALL;
        d->hardness[y1][x1] = 0;
      }
    }
  }
}

void print_grid(dungeon_t *d){
  int x, y;
  for(y = 0; y < Y_LENGTH; y++){
    for(x = 0; x < X_LENGTH; x++){
      switch(d->grid[y][x]){
      case PC:
        printf("@");
        break;
      case ROOM:
        printf(".");
        break;
      case HALL:
        printf("#");
        break;
      case WALL:
        printf(" ");
        break;
      }
    }
    printf("\n");  
  }
}

void hardness_to_grid(dungeon_t *d){
  int x, y;
  for(x = 0; x < X_LENGTH; x++){
    for(y = 0; y < Y_LENGTH; y++){
      d->grid[y][x] = d->hardness[y][x] > 0 ? WALL : HALL; 
    }
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
  dungeon_t *d = (dungeon_t*)malloc(sizeof(dungeon_t));
  init_grid(d);
  init_rooms(d);
  int valid = rooms_valid(d);
  while(!valid){
    init_rooms(d);
    valid = rooms_valid(d);
  }

  insert_rooms(d);
  insert_halls(d);
  return d;
}


/**
  * Added this from solution 1
 **/
int gaussian[5][5] = {
  {  1,  4,  7,  4,  1 },
  {  4, 16, 26, 16,  4 },
  {  7, 26, 41, 26,  7 },
  {  4, 16, 26, 16,  4 },
  {  1,  4,  7,  4,  1 }
};

typedef struct queue_node {
  int x, y;
  struct queue_node *next;
} queue_node_t;

static int smooth_hardness(dungeon_t *d)
{
  int32_t i, x, y;
  int32_t s, t, p, q;
  queue_node_t *head, *tail, *tmp;
  FILE *out;
  uint8_t hardness[Y_LENGTH][X_LENGTH];

  memset(&hardness, 0, sizeof (hardness));

  /* Seed with some values */
  for (i = 1; i < 255; i += 20) {
    do {
      x = rand() % X_LENGTH;
      y = rand() % Y_LENGTH;
    } while (hardness[y][x]);
    hardness[y][x] = i;
    if (i == 1) {
      head = tail = malloc(sizeof (*tail));
    } else {
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
    }
    tail->next = NULL;
    tail->x = x;
    tail->y = y;
  }

  out = fopen("seeded.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", X_LENGTH, Y_LENGTH);
  fwrite(&hardness, sizeof (hardness), 1, out);
  fclose(out);

  /* Diffuse the vaules to fill the space */
  while (head) {
    x = head->x;
    y = head->y;
    i = hardness[y][x];

    if (x - 1 >= 0 && y - 1 >= 0 && !hardness[y - 1][x - 1]) {
      hardness[y - 1][x - 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x - 1;
      tail->y = y - 1;
    }
    if (x - 1 >= 0 && !hardness[y][x - 1]) {
      hardness[y][x - 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x - 1;
      tail->y = y;
    }
    if (x - 1 >= 0 && y + 1 < Y_LENGTH && !hardness[y + 1][x - 1]) {
      hardness[y + 1][x - 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x - 1;
      tail->y = y + 1;
    }
    if (y - 1 >= 0 && !hardness[y - 1][x]) {
      hardness[y - 1][x] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x;
      tail->y = y - 1;
    }
    if (y + 1 < Y_LENGTH && !hardness[y + 1][x]) {
      hardness[y + 1][x] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x;
      tail->y = y + 1;
    }
    if (x + 1 < X_LENGTH && y - 1 >= 0 && !hardness[y - 1][x + 1]) {
      hardness[y - 1][x + 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x + 1;
      tail->y = y - 1;
    }
    if (x + 1 < X_LENGTH && !hardness[y][x + 1]) {
      hardness[y][x + 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x + 1;
      tail->y = y;
    }
    if (x + 1 < X_LENGTH && y + 1 < Y_LENGTH && !hardness[y + 1][x + 1]) {
      hardness[y + 1][x + 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x + 1;
      tail->y = y + 1;
    }

    tmp = head;
    head = head->next;
    free(tmp);
  }

  /* And smooth it a bit with a gaussian convolution */
  for (y = 0; y < Y_LENGTH; y++) {
    for (x = 0; x < X_LENGTH; x++) {
      for (s = t = p = 0; p < 5; p++) {
        for (q = 0; q < 5; q++) {
          if (y + (p - 2) >= 0 && y + (p - 2) < Y_LENGTH &&
              x + (q - 2) >= 0 && x + (q - 2) < X_LENGTH) {
            s += gaussian[p][q];
            t += hardness[y + (p - 2)][x + (q - 2)] * gaussian[p][q];
          }
        }
      }
      d->hardness[y][x] = t / s;
    }
  }
  /* Let's do it again, until it's smooth like Kenny G. */
  for (y = 0; y < Y_LENGTH; y++) {
    for (x = 0; x < X_LENGTH; x++) {
      for (s = t = p = 0; p < 5; p++) {
        for (q = 0; q < 5; q++) {
          if (y + (p - 2) >= 0 && y + (p - 2) < Y_LENGTH &&
              x + (q - 2) >= 0 && x + (q - 2) < X_LENGTH) {
            s += gaussian[p][q];
            t += hardness[y + (p - 2)][x + (q - 2)] * gaussian[p][q];
          }
        }
      }
      d->hardness[y][x] = t / s;
    }
  }


  out = fopen("diffused.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", X_LENGTH, Y_LENGTH);
  fwrite(&hardness, sizeof (hardness), 1, out);
  fclose(out);

  out = fopen("smoothed.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", X_LENGTH, Y_LENGTH);
  fwrite(&d->hardness, sizeof (d->hardness), 1, out);
  fclose(out);

  return 0;
}
