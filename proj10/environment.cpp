#include "environment.h"

using namespace std;

bool npc_area(envr_type next){ return next == EMPTY || next == LADDER; }
bool empty_area(envr_type next){
  return next == EMPTY || next == LADDER || next == CLOUD || next == END;
}
bool solid_area(envr_type next){ 
  return next == BOX || next == GROUND || next == CLOUD || next == LADDER; 
}
bool walkable_area(envr_type next){ return next == CLOUD || next == LADDER || next == BOX; }
bool climbable_area(envr_type next){ return next == LADDER; }
bool dropdown_area(envr_type next){ return next == LADDER || next == CLOUD; }
bool inbounds(int32_t y, int32_t x){ 
  return x >= 0 && y >= 0 && x < ENVIRONMENT_X && y < ENVIRONMENT_Y; 
}

void gen_pits(environment *e){
  uint32_t x, y, i;
  uint32_t start, end, total; 
  start = PC_START_X + 1;
  end = JUMP_TIME;
  
  total = rand() % (10 + 3 * e->level); //Higher levels, greater chance of more pits
  for(i = 0; i < total; i++){
    start = rand() % (ENVIRONMENT_X - PC_START_X) + PC_START_X + 1;
    end = rand() % (JUMP_TIME * 2 * e->level) + start;  //Higher levels, greater chance of big pits
    for(x = start; x < end; x++){
      for(y = 0; y < PC_START_Y; y++){
        e->environment_map[y][x] = EMPTY;
      }
    }
  }
  
}

void environment::gen_npcs(){
  uint32_t total, i, x , y;
  bool created;
  
  total = (rand() % (10 * level)) + 15 * level;
  for(i = 0; i < total; i++){
    y = rand() % (ENVIRONMENT_Y - PC_START_Y) + PC_START_Y;
    x = rand() % (END_X - PC_START_X) + PC_START_X + 1;
    created = false;
    while(!created){
      while(y > PC_START_Y - 1 && !created){
        character *c = character_map[y][x];
        envr_type envr = environment_map[y][x];
        envr_type below = environment_map[y - 1][x];
        if(!c && envr == EMPTY && solid_area(below)){
          npc *n = new npc();
          n->color = (rand() % 7) + 1;
          n->pos[dim_x] = x;
          n->pos[dim_y] = y;
          character_map[y][x] = n;
          q->enqueue((void*)n);
          created = true;
        }
        y--;
      }
      y = ENVIRONMENT_Y - 1;
      x = x + 1 < END_X ? x + 1 : PC_START_X + 1;
    }
  
  }

}

void place_checkpoint(environment *e, uint32_t x){
  uint32_t y;
  envr_type envr;
  
  for(x = x; x < ENVIRONMENT_X; x++){
    for(y = PC_START_Y; y < ENVIRONMENT_Y; y++){
      envr = e->environment_map[y][x];
      if(envr == EMPTY){
        e->object_map[y][x] = CHECKPOINT;
        return;
      }
    }
  }
}

void gen_boxes_clouds_ladders(environment *e){
  uint32_t i, j, y, x, len;
  envr_type envr;
  uint32_t total = rand() % (30 * e->level) + 120; // Higher level, greater chance
  for(i = 0; i < total; i++){
    x = rand() % END_X;
    y = rand() % (ENVIRONMENT_Y - PC_START_Y) + PC_START_Y;
    len = (rand() % 4) + 3;
    len = len + x < END_X ? len : END_X - (x + 1);
    envr = rand() % 5 > 2 ? BOX : CLOUD;
    for(j = 0; j < len; j++){
      e->environment_map[y][x + j] = envr;
    }
    
    bool ladder, right;
    ladder = (rand() % 100) < 2 * e->level; // Higher level, greater chance
    right = (x == 0) || (rand() % 2 && x + len < END_X - 1);
    x = right ? x + len : x - 1;
    while(ladder){
      e->environment_map[y][x] = LADDER;
      y--;
      ladder = inbounds(y, x) && envrxy(x, y) == EMPTY;
    }
  }
}

void gen_coins(environment *e){
  uint32_t y, x, i, total;
  uint32_t dist;
  int len;
  total = rand() % (200 - (e->level * 20)) + 200;
  for(i = 0; i < total; i++){
    len = rand() % 15 + 1;
    do{
      y = rand() % (ENVIRONMENT_Y - PC_START_Y) + PC_START_Y;
      x = rand() % (END_X - PC_START_X) + PC_START_X; 
    }while(envrxy(x, y) && objxy(x, y));
    dist = x + len;
    for(x = x; x < dist && !envrxy(x, y) && !objxy(x, y); x++){
      if(inbounds(y, x)){
        e->object_map[y][x] = COIN;
        i++;
      }
    }
    
  }
  
}

void environment::gen_environment(){
  uint32_t x, y;
  
  for(x = 0; x < ENVIRONMENT_X; x++){
    for(y = 0; y < PC_START_Y; y++){
      environment_map[y][x] = GROUND;
    }
  }
  
  for(y = PC_START_Y; y < ENVIRONMENT_Y; y++){
    environment_map[y][END_X] = END;
  }
  
  gen_pits(this);
  gen_boxes_clouds_ladders(this);
  place_checkpoint(this, ENVIRONMENT_X / 3);
  place_checkpoint(this, 2 * ENVIRONMENT_X / 3);
  gen_coins(this);

}

void environment::place_pc(){
  character_map[PC_START_Y][PC_START_X] = PC;
  PC->pos[dim_y] = PC_START_Y;
  PC->pos[dim_x] = PC_START_X;
}

void environment::init(){
  gen_environment();
  gen_npcs();
  place_pc();
  checkpoint[dim_x] = PC_START_X;
  checkpoint[dim_y] = PC_START_Y;
}

void environment::redraw(){
  uint32_t x, y;
  for(x = 0; x < ENVIRONMENT_X; x++){
    for(y = 0; y < ENVIRONMENT_Y; y++){
      object_map[y][x] = NONE;
      environment_map[y][x] = EMPTY;
      character_map[y][x] = NULL;;
    }
  }
  while(q->size > 0){
    character *c = (character*)q->dequeue();
    if(c != PC) delete c;
  }
  init();
}








