#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define X_MIN 3
#define Y_MIN 2
#define X_LENGTH 80
#define Y_LENGTH 21
#define MAX_LENGTH 12

typedef struct room_t room_t;

room_t *create_room();

int rooms_intersect(room_t*, room_t*);
