#include "environment.h"

using namespace std;

bool empty_area(envr_type next){
  return next == EMPTY || next == LADDER || next == CLOUD || next == END;
}
bool solid_area(envr_type next){ return next == BOX || next == GROUND || next == CLOUD; }
bool walkable_area(envr_type next){ return next == CLOUD || next == LADDER || next == BOX; }
bool climbable_area(envr_type next){ return next == LADDER; }
bool dropdown_area(envr_type next){ return next == LADDER || next == CLOUD; }
bool inbounds(int y, int x){ return x >= 0 && y >= 0 && x < ENVIRONMENT_X && y < ENVIRONMENT_Y; }

void move_pc(environment *e, int y, int x){
  e->character_map[e->PC->pos[dim_y]][e->PC->pos[dim_x]] = NULL;
  e->character_map[y][x] = e->PC;
  e->PC->pos[dim_x] = x;
  e->PC->pos[dim_y] = y;
}

void respawn_pc(environment *e){
  e->PC->remove_life();
  if(e->PC->alive){
    move_pc(e, e->checkpoint[dim_y], e->checkpoint[dim_x]);
  }
}

void handle_pc(environment *e, int dir){
  envr_type next;
  int16_t x = e->PC->pos[dim_x];
  int16_t y = e->PC->pos[dim_y];
  
  switch(dir){
    case KEY_RIGHT:  //move right
      if(x + 1 == ENVIRONMENT_X) return;
      next = e->environment_map[y][x + 1];
      if(empty_area(next)) move_pc(e, y, x + 1); 
      break;
    case KEY_LEFT:  //move left
      if(x == 0) return;
      next = e->environment_map[y][x - 1];
      if(empty_area(next)) move_pc(e, y, x - 1); 
      break;
    case KEY_UP: //climbing
      if(y + 1 == ENVIRONMENT_Y) return;
      next = e->environment_map[y + 1][x];
      if(climbable_area(next)) move_pc(e, y + 1, x);
      break;
    case KEY_DOWN: //dropdown
      if(y == 0) return;
      next = e->environment_map[y - 1][x];
      if(dropdown_area(next)) {
        move_pc(e, y - 1, x); 
        e->PC->air_time = true; 
      }; 
      break;
  }
}

void handle_jump(environment *e){
  int x = e->PC->pos[dim_x];
  int y = e->PC->pos[dim_y];
  envr_type above, below, right, left;
  
  if(inbounds(y + 1, x) && inbounds(y - 1, x)){
    above = e->environment_map[y + 1][x];
    below = e->environment_map[y - 1][x];
    if(above != BOX && above != GROUND && below != EMPTY && below != END){
      e->PC->jump = JUMP_TIME;
    }
  }

  if(inbounds(y , x + 1)){
    right = e->environment_map[y][x + 1];
    if(solid_area(right)){
      e->PC->jump = JUMP_TIME;
      move_pc(e, y, x - 1);      
    }
  }
  else if(inbounds(y , x - 1)){
    left = e->environment_map[y][x - 1];
    if(solid_area(left)){
      e->PC->jump = JUMP_TIME;
      move_pc(e, y, x + 1);
    }
  }
}

void gravity_update(environment *e){
  int x = e->PC->pos[dim_x];
  int y = e->PC->pos[dim_y];
  envr_type next;
  if(e->PC->jump > 0) { 
    e->PC->jump--; 
    if(e->PC->jump == 0){
      e->PC->air_time = true;
    }
    
    next = e->environment_map[y + 1][x];
    if(next == CLOUD || next == EMPTY){ 
      move_pc(e, y + 1, x); 
    }
    else{ 
      e->PC->jump = 0;
      if(next == BOX){
        e->environment_map[y + 1][x] = EMPTY;
        //e->object_map[y + 1][x] = random_object();
      } 
    }
    
  }
  else if(e->PC->air_time){
    e->PC->air_time = false;
  }
  else if(y > 0){
    next = e->environment_map[y - 1][x];
    if(next == EMPTY) move_pc(e, y - 1, x);
  }
  else if(y == 0){
    respawn_pc(e);
  }
}

void environment::gen_pits(){
  int x, y;
  int start, end;
  start = 0;
  end = JUMP_TIME;
  
  for(x = start; x < end; x++){
    for(y = 0; y < PC_START_Y; y++){
      environment_map[y][x] = EMPTY;
    }
  }
}

void environment::gen_environment(){
  int x, y;
  
  for(x = 0; x < ENVIRONMENT_X; x++){
    for(y = 0; y < PC_START_Y; y++){
      environment_map[y][x] = GROUND;
    }
  }
  
  for(y = PC_START_Y; y < ENVIRONMENT_Y; y++){
    environment_map[y][END_X] = END;
  }
  
  gen_pits();
  
  
  // testing
  for(y = PC_START_Y; y < ENVIRONMENT_Y; y++){
    environment_map[y][40] = LADDER;
  }
  
  for(x = 0; x < 4; x++){
    environment_map[5][x + 45] = BOX;
  }
  
  for(x = 0; x < 4; x++){
    environment_map[12][x + 55] = CLOUD;
  }
  
  
}

void environment::place_pc(){
  character_map[PC_START_Y][PC_START_X] = PC;
  PC->pos[dim_y] = PC_START_Y;
  PC->pos[dim_x] = PC_START_X;
}

void environment::init(){
  gen_environment();
  place_pc();
}



void get_view(environment *e, pair_t pos){
  if(e->PC->pos[dim_x] < VIEW_X/2){
    pos[dim_x] = 0;
  } 
  else if(ENVIRONMENT_X - e->PC->pos[dim_x] < VIEW_X/2){
    pos[dim_x] = ENVIRONMENT_X - VIEW_X;
  }
  else{
    pos[dim_x] = e->PC->pos[dim_x] - VIEW_X/2;
  }
  
  if(e->PC->pos[dim_y] < VIEW_Y/2){
    pos[dim_y] = 0;
  } 
  else if(ENVIRONMENT_Y - e->PC->pos[dim_y] < VIEW_Y/2){
    pos[dim_y] = ENVIRONMENT_Y - VIEW_Y;
  }
  else{
    pos[dim_y] = e->PC->pos[dim_y] - VIEW_Y/2;
  }
  
}

void print_screen(environment *e){
  char x, y;
  uint32_t color;
  
  pair_t view;
  get_view(e, view);
  string end = "END OF LEVEL ";
  
  for(x = 0; x < VIEW_X; x++){
    for(y = 0; y < VIEW_Y; y++){
      
      if(charxy(view[dim_x] + x, view[dim_y] + y)){
        character *c = charxy(view[dim_x] + x, view[dim_y] + y);
        color = c->color;
        attron(COLOR_PAIR(color));
        mvaddch(VIEW_Y + 2 - y, x, '@');
      }
      else{
        switch(e->environment_map[view[dim_y] + y][view[dim_x] + x]){
          case GROUND:
            attron(COLOR_PAIR(COLOR_GREEN));
            mvaddch(VIEW_Y + 2 - y, x, '#');
            break;
          case BOX:
            attron(COLOR_PAIR(COLOR_MAGENTA));
            mvaddch(VIEW_Y + 2 - y, x, '?');
            break;
          case LADDER:
            attron(COLOR_PAIR(COLOR_YELLOW));
            mvaddch(VIEW_Y + 2 - y, x, 'H');
            break;
          case EMPTY:
            mvaddch(VIEW_Y + 2 - y, x, ' ');
            break;
          case CLOUD:
            attron(COLOR_PAIR(COLOR_WHITE));
            mvaddch(VIEW_Y + 2 - y, x, '&');
            break;
          case TUBE:
            attron(COLOR_PAIR(COLOR_GREEN));
            mvaddch(VIEW_Y + 2 - y, x, '|');
            break;
          case END:
            int index = end.size() - ((view[dim_y] + y + 5) % end.size()) - 1;
            attron(COLOR_PAIR(rand() % 8));
            mvaddch(VIEW_Y + 2 - y, x, end[index]);
            break;
        }
      }
      
    }
  }
}


