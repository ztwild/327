
#define JUMP_TIME         4
#define INVINCIBLE_Time   50
#define PC_START_X        5
#define PC_START_Y        5

using namespace std;

class pc: public character{
  public:
    uint16_t jump;
    uint16_t invincible;
    uint32_t life;
    uint32_t coin;
    bool air_time;
    bool jump_ready;
    bool alive;
    pc(): jump(0), invincible(0), life(3), coin(0), 
          air_time(false), jump_ready(true), alive(true) 
    {
      color = COLOR_WHITE;
    };
    ~pc(){ };
    inline void remove_life(){
      life--;
      if(life == 0) alive = false;
    }
};




