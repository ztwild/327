#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "queue.c"
#include "dungeon.c"
#include "load_save.c"
#include "path_find.c"

void init_game(uint8_t, uint8_t);

int run_game(uint8_t);

int get_dir(int, int);

void next_pos(character_t *);

int cell_taken(int, int);

queue_t *init_turn_queue();

int end_game();

void print_queue(queue_t *);

void print_test();

#endif
