#define MAX_SP 20
#define MIN_SP 5

typedef struct monster{
  char attr;
  uint8_t speed;
  uint32_t health;
  pair_t *pos;
}monster_t;

monster_t *create_monster(){
  monster_t *m = (monster_t*)malloc(sizeof(monster_t));
  m->speed = (rand() % (MAX_SP - MIN_SP)) + MIN_SP;
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
