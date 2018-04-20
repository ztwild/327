#include "environment.h"

int empty_area(envr_type next){
  return next == EMPTY || next == LADDER || next == CLOUD;
}

void move_pc(environment *e, int y, int x){
  e->character_map[e->PC->pos[dim_y]][e->PC->pos[dim_x]] = NULL;
  e->character_map[y][x] = e->PC;
  e->PC->pos[dim_x] = x;
  e->PC->pos[dim_y] = y;
}

void handle_pc(environment *e, int dir){
  envr_type next;
  int16_t x = e->PC->pos[dim_x];
  int16_t y = e->PC->pos[dim_y];
  
  switch(dir){
    case KEY_RIGHT:
      if(x + 1 == ENVIRONMENT_X) return;
      next = e->environment_map[y][x + 1];
      if(empty_area(next)) move_pc(e, y, x + 1); 
      break;
    case KEY_LEFT:
      if(x == 0) return;
      next = e->environment_map[y][x - 1];
      if(empty_area(next)) move_pc(e, y, x - 1); 
      break;
    case KEY_UP:
      if(y + 1 == ENVIRONMENT_Y) return;
      next = e->environment_map[y + 1][x];
      if(next == LADDER) move_pc(e, y + 1, x);
      break;
    case KEY_DOWN:
      if(y == 0) return;
      next = e->environment_map[y - 1][x];
      if(next == LADDER || next == CLOUD) {
        move_pc(e, y - 1, x); 
        e->PC->air_time = true; 
      }; 
      break;
  }
}

void handle_jump(environment *e){
  int x = e->PC->pos[dim_x];
  int y = e->PC->pos[dim_y];
  envr_type above = e->environment_map[y + 1][x];
  envr_type below = e->environment_map[y - 1][x];
  if(above != LADDER && below != EMPTY){
    e->PC->jump = JUMP_TIME;
  }
  envr_type right = e->environment_map[y][x + 1];
  envr_type left = e->environment_map[y][x - 1];
  if(right == BOX || right == GROUND){
    e->PC->jump = JUMP_TIME;
    move_pc(e, y, x - 1);
  }
  else if(left == BOX || left == GROUND){
    e->PC->jump = JUMP_TIME;
    move_pc(e, y, x + 1);
  }
}

void gravity_update(environment *e){
  int x = e->PC->pos[dim_x];
  int y = e->PC->pos[dim_y];
  envr_type next;
  if(e->PC->jump > 0) { 
    e->PC->jump--; 
    next = e->environment_map[y + 1][x];
    if(next == CLOUD || next == EMPTY) move_pc(e, y + 1, x);
    if(e->PC->jump == 0) e->PC->air_time = true;
  }
  else if(e->PC->air_time){
    e->PC->air_time = false;
  }
  else{
    next = e->environment_map[y - 1][x];
    if(next == EMPTY) move_pc(e, y - 1, x);
  }
}

void environment::gen_environment(){
  int x, y;
  
  for(x = 0; x < ENVIRONMENT_X; x++){
    for(y = 0; y < PC_START_Y; y++){
      environment_map[y][x] = GROUND;
    }
  }
  
  for(x = 0; x < ENVIRONMENT_X; x++){
    for(y = y; y < ENVIRONMENT_Y; y++){
      environment_map[y][x] = EMPTY;
    }
  }
  
  for(y = 0; y < ENVIRONMENT_Y; y++){
    environment_map[y][40] = LADDER;
  }
  
  for(x = 0; x < 4; x++){
    environment_map[12][x + 45] = BOX;
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
    pos[dim_x] = ENVIRONMENT_X - VIEW_X/2;
  }
  else{
    pos[dim_x] = e->PC->pos[dim_x] - VIEW_X/2;
  }
  
  if(e->PC->pos[dim_y] < VIEW_Y/2){
    pos[dim_y] = 0;
  } 
  else if(ENVIRONMENT_Y - e->PC->pos[dim_y] < VIEW_Y/2){
    pos[dim_y] = ENVIRONMENT_Y - VIEW_Y/2;
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
        }
      }
      
    }
  }
}


