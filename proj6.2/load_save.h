#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <endian.h>
#include <string.h>
#include <unistd.h>
#include "utils.cpp"

#define FILE_PATH "/.rlg327/dungeon"
#define SEMANTIC "RLG327-S2018"

char *get_path();

dungeon_t *load_dungeon();

int save_dungeon(dungeon_t *);


