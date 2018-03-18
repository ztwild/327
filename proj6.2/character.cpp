#include "character.h"

pair_t *create_pair(uint8_t x, uint8_t y){
  pair_t *p = (pair_t*)malloc(sizeof(pair_t));
  p->x = x;
  p->y = y;
  return p;
}

pair_t *copy_pos(pair_t *p){
  pair_t *copy = (pair_t*)malloc(sizeof(pair_t));
  copy->x = p->x;
  copy->y = p->y;
  return copy;
}

character_t *create_pc(){
  character_t *pc = (character_t*)malloc(sizeof(character_t));
  pc->type = PC;
  pc->alive = 1;
  pc->turn = 0;
  pc->health = MAX_HEALTH;
  pc->speed = 10;
  pc->time = 0;
  pc->attr = '\0';
  return pc;
}

character_t *create_monster(uint32_t time){
  character_t *m = (character_t*)malloc(sizeof(character_t));
  m->type = NPC;
  m->alive = 1;
  m->turn = 0;
  m->speed = (rand() % (MAX_SP - MIN_SP)) + MIN_SP;
  m->time = time;
  char I = rand() % 2;
  char T = rand() % 2;
  char A = rand() % 2;
  //char A = 0;
  char E = rand() % 2;
  //printf("E: %d, A: %d, T: %d, I: %d\n", E, A, T, I);
  m->attr = I;
  m->attr |= (T << 1);
  m->attr |= (A << 2);
  m->attr |= (E << 3);
  
  return m;
}

int compare_characters(const void *c1, const void *c2){
  character_t *char1 = (character_t*) c1;
  character_t *char2 = (character_t*) c2;
  
  int turn = char1->turn < char2->turn;
  int time = char1->turn == char2->turn && char1->time < char2->time;
  return char1->alive && (turn || time);
}






