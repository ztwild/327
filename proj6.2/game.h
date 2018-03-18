#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "queue.cpp"
#include "dungeon.cpp"
#include "load_save.cpp"
#include "path_find.cpp"

#define TIME 500000
#define SAVE "--save"
#define LOAD "--load"
#define NUMMON "--nummon"

void get_switches(int, char *[]);

void draw_new();

void init_game(int, char *[]);

int run_game();

int move_pc();

int get_dir(int, int);

void print_mon_list();

int next_pos(character_t *);

int cell_taken(int, int);

queue_t *init_turn_queue();

int end_game();

void print_queue(queue_t *);

void print_test();

#endif
