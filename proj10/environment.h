#include <string>
#include "character.cpp"
#include "pc.cpp"
#include "queue.cpp"

#define VIEW_X              80
#define VIEW_Y              21
#define ENVIRONMENT_X       250
#define ENVIRONMENT_Y       150
#define END_X               240

#define charpair(pair) (e->character_map[pair[dim_y]][pair[dim_x]])
#define charxy(x, y) (e->character_map[y][x])
#define objpair(pair) (e->objmap[pair[dim_y]][pair[dim_x]])
#define objxy(x, y) (e->objmap[y][x])
#define envrpair(pair) (e->environment_map[pair[dim_y]][pair[dim_x]])
#define envrxy(x, y) (e->environment_map[y][x])

using namespace std;

typedef enum envr_type{
  EMPTY,
  GROUND,
  BOX,
  LADDER,
  TUBE,
  CLOUD,
  END,
} envr_type;

typedef enum obj_type{
  NONE,
  COIN,
  LIFE,
  FIRE,
  INVINCIBLE,
  CHECKPOINT,
} obj_type;

class environment{
  public:
    pc *PC;
    pair_t checkpoint;
    envr_type environment_map[ENVIRONMENT_Y][ENVIRONMENT_X];
    obj_type object_map[ENVIRONMENT_Y][ENVIRONMENT_Y];
    character *character_map[ENVIRONMENT_Y][ENVIRONMENT_X];
    environment(){
      PC = new pc();
      checkpoint[dim_x] = PC_START_X;
      checkpoint[dim_y] = PC_START_Y;
    }
    ~environment(){
      delete PC;
    }
    void gen_pits();
    void gen_environment();
    void place_pc();
    void init();
    
};

void print_screen(environment *);


