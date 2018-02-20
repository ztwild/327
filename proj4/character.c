#define MAX_SP 20
#define MIN_SP 5
#define MAX_HEALTH 10

typedef enum char_type{
  PC,
  NPC
}type_t;

typedef struct pair{
  uint8_t x;
  uint8_t y;
}pair_t;

pair_t *create_pair(int x, int y){
  pair_t *p = (pair_t*)malloc(sizeof(pair_t));
  p->x = x;
  p->y = y;
  return p;
}

typedef struct character{
  type_t type;
  uint32_t turn;
  uint32_t health;
  uint8_t speed;
  pair_t *pos;
  uint32_t time;
  char attr;
}character_t;

character_t *create_pc(){
  character_t *pc = (character_t*)malloc(sizeof(character_t));
  pc->type = PC;
  pc->turn = 0;
  pc->health = MAX_HEALTH;
  pc->speed = 10;
  pc-> time = 0;
  pc->attr = '\0';
  return pc;
}

character_t *create_monster(uint32_t time){
  character_t *m = (character_t*)malloc(sizeof(character_t));
  m->type = NPC;
  m->turn = 0;
  m->speed = (rand() % (MAX_SP - MIN_SP)) + MIN_SP;
  m->time = time;
  char I = rand() % 2;
  char T = rand() % 2;
  char A = rand() % 2;
  char E = rand() % 2;
  //printf("E: %d, A: %d, T: %d, I: %d\n", E, A, T, I);
  m->attr = I;
  m->attr |= (T << 1);
  m->attr |= (A << 2);
  m->attr |= (E << 3);
  /**
  m->attr = rand() % 2;
  m->attr |= (rand() % 2) << 1;
  m->attr |= (rand() % 2) << 2;
  m->attr |= (rand() % 2) << 3;
  **/
  /**
  char c = m->attr > 9 ? (m->attr - 10) + 'a' : m->attr + '0';
  printf("Monster type %2d  ", m->attr);
  printf("%1x  ", m->attr);
  printf("%c\n", c);
  **/
  return m;
}

int compare(character_t *c1, character_t *c2){
  int turn = c1->turn < c2->turn;
  int time = c1->turn == c2->turn && c1->time < c2->time;
  return turn || time;
}
