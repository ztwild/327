
#define JUMP_TIME         4
#define INVINCIBLE_TIME   50
#define PC_START_X        5
#define PC_START_Y        5
#define LIVES             3
#define FIRE_BALLS        25

using namespace std;

class pc: public character{
  public:
    uint16_t jump;
    uint16_t invincible;
    int32_t life;
    uint32_t coin;
    bool air_time;
    bool jump_ready;
    uint32_t fire;
    pc(): jump(0), invincible(0), life(LIVES), coin(0), 
          air_time(false), jump_ready(true), fire(0) 
    {
      color = COLOR_WHITE;
      type = PC;
      alive = true;
    };
    ~pc(){ };
    inline void remove_life(){
      life--;
    }
};

typedef enum direction_type{
  LEFT  = 'a',
  DOWN  = 's',
  RIGHT = 'd',
  UP    = 'w',
  
} direction_type;

class fire_ball: public character{
  public:
    direction_type direction;
    fire_ball(int d){
      type = FIRE_BALL;
      color = COLOR_RED;
      alive = true;
      direction = static_cast<direction_type>(d);
    }
    ~fire_ball(){ };
};




