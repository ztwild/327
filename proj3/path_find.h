#include <stdio.h>
#include <stdlib.h>
#include "queue.c"

typedef struct path path_t;

typedef struct pair pair_t;

void init_path(path_t *);

pair_t *pc_start(dungeon_t *);

void bfs(dungeon_t *, path_t *, pair_t *);

int bfs_rec(dungeon_t *, path_t *, pair_t *, int);

void print_path_open(path_t *);



