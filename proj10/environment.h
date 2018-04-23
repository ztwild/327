#include <string>
#include "character.cpp"
#include "pc.cpp"
#include "npc.cpp"
#include "queue.cpp"

#define VIEW_X              80
#define VIEW_Y              21
#define ENVIRONMENT_X       250
#define ENVIRONMENT_Y       82
#define END_X               240
#define OBJECT_COUNT        4

#define COIN_RARITY         5
#define FIRE_RARITY         75
#define LIFE_RARITY         85
#define INVINCIBLE_RARITY   95

#define charpair(pair) (e->character_map[pair[dim_y]][pair[dim_x]])
#define charxy(x, y) (e->character_map[y][x])
#define objpair(pair) (e->object_map[pair[dim_y]][pair[dim_x]])
#define objxy(x, y) (e->object_map[y][x])
#define envrpair(pair) (e->environment_map[pair[dim_y]][pair[dim_x]])
#define envrxy(x, y) (e->environment_map[y][x])


using namespace std;

typedef enum envr_type{
  EMPTY = 0,
  GROUND,
  BOX,
  LADDER,
  CLOUD,
  END,
} envr_type;

typedef enum obj_type{
  NONE = 0,
  CHECKPOINT,
  COIN,
  LIFE,
  FIRE,
  INVINCIBLE,
} obj_type;

class environment{
  public:
    pc *PC;
    uint16_t level;
    uint32_t temp;
    pair_t checkpoint;
    queue *q;
    envr_type environment_map[ENVIRONMENT_Y][ENVIRONMENT_X];
    obj_type object_map[ENVIRONMENT_Y][ENVIRONMENT_X];
    character *character_map[ENVIRONMENT_Y][ENVIRONMENT_X];
    environment(): level(1) {
      PC = new pc();
      q = new queue();
      temp = 0;
    }
    ~environment(){
      delete PC;
      delete q;
    }
    void gen_environment();
    void gen_npcs();
    void place_pc();
    void init();
    void redraw();
    
};

void print_screen(environment *);


