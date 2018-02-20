#ifndef PATH_FIND_H
#define PATH_FIND_H

#include <stdio.h>
#include <stdlib.h>
#include "queue.c"

void init_path(dungeon_t *);

void bfs(dungeon_t *);

int bfs_rec(dungeon_t *, pair_t *, int);

void print_path_open(dungeon_t *);

void dijkstra(dungeon_t *);

void print_path_wall(dungeon_t *);

#endif
