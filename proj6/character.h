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

class pair_t{
  public:
    uint8_t x;
    uint8_t y;
    
    pair_t *create_pair(uint8_t, uint8_t);
};

class character_t{
  public:  
    type_t type;
    uint8_t alive;
    uint32_t turn;
    uint32_t health;
    uint8_t speed;
    pair_t *pos;
    uint32_t time;
    char attr;
    
    character_t *create_pc();
    character_t *create_monster(uint32_t);
    int compare(character_t *, character_t *);
    
};

#endif
