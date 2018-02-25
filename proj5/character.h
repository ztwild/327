#ifndef CHARACTER_H
#define CHARACTER_H

#define MAX_SP 20
#define MIN_SP 5
#define MAX_HEALTH 10

//#include "queue.c"

typedef enum char_type{
  PC,
  NPC
}type_t;

typedef struct pair pair_t;

pair_t *create_pair(int, int);

typedef struct character character_t;

character_t *create_pc();

character_t *create_monster(uint32_t);

int compare(character_t *, character_t *);

#endif
