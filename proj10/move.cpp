
void handle_pickup(environment *e){
  obj_type obj = objpair(e->PC->pos);
  e->object_map[e->PC->pos[dim_y]][e->PC->pos[dim_x]] = NONE;
  switch(obj){
    case COIN:
      e->PC->coin++;
      if(e->PC->coin == 100){
        e->PC->coin = 0;
        e->PC->life++;
      }
      break;
    case INVINCIBLE:
      e->PC->invincible = INVINCIBLE_TIME;
      break;
    case LIFE:
      e->PC->life++;
      break;
    case CHECKPOINT:
      if(e->checkpoint[dim_x] < e->PC->pos[dim_x]){
        e->checkpoint[dim_x] = e->PC->pos[dim_x];
        e->checkpoint[dim_y] = e->PC->pos[dim_y];
      }
      break;
    case FIRE:
      e->PC->fire = FIRE_BALLS;
      break;
    default:
      break;
  }
}

void move_pc(environment *e, uint32_t y, uint32_t x){
  e->character_map[e->PC->pos[dim_y]][e->PC->pos[dim_x]] = NULL;
  e->character_map[y][x] = e->PC;
  e->PC->pos[dim_x] = x;
  e->PC->pos[dim_y] = y;
  handle_pickup(e);
}

void respawn_pc(environment *e){
  e->PC->life--;
  if(e->PC->life >= 0){
    move_pc(e, e->checkpoint[dim_y], e->checkpoint[dim_x]);
  }
}

bool enemy_win(environment *e, uint32_t y, uint32_t x, bool condition){
  character *enemy;
  if(!inbounds(y, x) || !(enemy = charxy(x, y)) ) return false;
  if(enemy->type == NPC){ 
    if(condition){
      respawn_pc(e); 
      return true;
    }
    else {
      enemy->alive = false;
      e->character_map[y][x] = NULL;
    }
  }
  else if(enemy->type == FIRE_BALL){
    respawn_pc(e); 
    enemy->alive = false;
    e->character_map[y][x] = NULL;
    return true;
  }
  return false;
}

void handle_pc(environment *e, int dir){
  envr_type next, cur;
  
  uint32_t x = e->PC->pos[dim_x];
  uint32_t y = e->PC->pos[dim_y];
  
  switch(dir){
    case KEY_RIGHT:  //move right
      if(!inbounds(y , x + 1)) return;
      if(enemy_win(e, y, x + 1, e->PC->invincible == 0)) return;
      next = e->environment_map[y][x + 1];
      if(empty_area(next)) move_pc(e, y, x + 1);
      break;
    case KEY_LEFT:  //move left
      if(!inbounds(y , x - 1)) return;
      if(enemy_win(e, y, x - 1, e->PC->invincible == 0)) return;
      next = e->environment_map[y][x - 1];
      if(empty_area(next)) move_pc(e, y, x - 1); 
      break;
    case KEY_UP: //climbing
      if(!inbounds(y + 1 , x)) return;
      if(enemy_win(e, y + 1, x, e->PC->invincible == 0)) return;
      next = e->environment_map[y + 1][x];
      cur = e->environment_map[y][x];
      if(climbable_area(cur) || climbable_area(next)) move_pc(e, y + 1, x);
      break;
    case KEY_DOWN: //dropdown
      if(!inbounds(y - 1 , x)) return;
      if(charxy(x, y - 1)) {
        if(enemy_win(e, y - 1, x, false)) return; //should always win
        e->PC->jump = JUMP_TIME;
      }
      else { e->PC->jump = 0; }
      next = e->environment_map[y - 1][x];
      if(dropdown_area(next)) {
        move_pc(e, y - 1, x); 
        e->PC->air_time = true; 
      }; 
      break;
  }
}

void handle_jump(environment *e){
  uint32_t x = e->PC->pos[dim_x];
  uint32_t y = e->PC->pos[dim_y];
  envr_type above, below, right, left;
  
  if(inbounds(y + 1, x) && inbounds(y - 1, x)){
    above = e->environment_map[y + 1][x];
    below = e->environment_map[y - 1][x];
    if(above != BOX && above != GROUND && below != EMPTY && below != END){
      e->PC->jump = JUMP_TIME;
      return;
    }
  }

  if(inbounds(y , x + 1)){
    right = e->environment_map[y][x + 1];
    if(solid_area(right)){
      e->PC->jump = JUMP_TIME;
      if(enemy_win(e, y, x - 1, e->PC->invincible == 0)) return;
      move_pc(e, y, x - 1);
      return;     
    }
  }
  if(inbounds(y , x - 1)){
    left = e->environment_map[y][x - 1];
    if(solid_area(left)){
      e->PC->jump = JUMP_TIME;
      if(enemy_win(e, y, x + 1, e->PC->invincible == 0)) return;
      move_pc(e, y, x + 1);
      return;
    }
  }
}

obj_type random_object(){
  uint32_t roll;
  obj_type obj;
  bool valid = false;
  do{
    roll = rand() % 100;
    obj = static_cast<obj_type>(rand() % 4 + 2);
    valid = obj == INVINCIBLE && roll > INVINCIBLE_RARITY;
    valid |= obj == LIFE && roll > LIFE_RARITY;
    valid |= obj == FIRE && roll > FIRE_RARITY;
    valid |= obj == COIN && roll > COIN_RARITY; 
  }while(!valid);
  return obj;
}



void gravity_update(environment *e){
  uint32_t x = e->PC->pos[dim_x];
  uint32_t y = e->PC->pos[dim_y];
  envr_type next;
  if(e->PC->invincible > 0) e->PC->invincible--;
  if(e->PC->jump > 0) { 
    e->PC->jump--; 
    if(e->PC->jump == 0){
      e->PC->air_time = true;
    }
    
    next = e->environment_map[y + 1][x];
    if(next == CLOUD || next == EMPTY){
      if(enemy_win(e, y + 1, x, e->PC->invincible == 0)) return;
      move_pc(e, y + 1, x); 
    }
    else{ 
      e->PC->jump = 0;
      if(next == BOX){
        if(enemy_win(e, y + 2, x, false)) return; //should always win
        e->environment_map[y + 1][x] = EMPTY;
        e->object_map[y + 1][x] = random_object();
      } 
    }
    
  }
  else if(e->PC->air_time){
    e->PC->air_time = false;
  }
  else if(y > 0){
    if(charxy(x, y - 1)) {
      if(enemy_win(e, y - 1, x, false)) return; //should always win
      e->PC->jump = JUMP_TIME - 1;
      move_pc(e, y + 1, x);
    }
    else{
      next = e->environment_map[y - 1][x];
      if(next == EMPTY) move_pc(e, y - 1, x);
    }
  }
  else if(y == 0){
    respawn_pc(e);
  }
}

void move_npc(environment *e, npc *n){
  uint32_t dir = n->right ? 1 : -1;
  
  e->character_map[n->pos[dim_y]][n->pos[dim_x]] = NULL;
  e->character_map[n->pos[dim_y]][n->pos[dim_x] + dir] = n;
  n->pos[dim_x] += dir;

}

void handle_npc(environment *e, npc *n){
  uint32_t dir = n->right ? 1 : -1;
  envr_type next, below_next;
  character *c;
  if(!inbounds(n->pos[dim_y], n->pos[dim_x] + dir)){
    n->right = !n->right;
    return;
  }
  next = envrxy(n->pos[dim_x] + dir, n->pos[dim_y]);
  below_next = envrxy(n->pos[dim_x] + dir, n->pos[dim_y] - 1);
  c = charxy(n->pos[dim_x] + dir, n->pos[dim_y]);
  if(npc_area(next) && solid_area(below_next)){
    if(c == NULL){
      move_npc(e, n);
    }
    else if(c->type == PC){
      respawn_pc(e);
      move_npc(e, n);
    }
    else if(c->type == FIRE_BALL){
      n->alive = false;
      e->character_map[n->pos[dim_y]][n->pos[dim_x]] = NULL;
    }
    else{
      n->right = !n->right;
    }
  }
  else{
    n->right = !n->right;
  }
}


direction_type create_fire(environment *e, char key){
  
  uint32_t x = e->PC->pos[dim_x];
  uint32_t y = e->PC->pos[dim_y];
  fire_ball *fb = new fire_ball(key);
  
  switch(fb->direction){
    case RIGHT: x++; 
    case LEFT:  x--;
    case UP:    y++;
    case DOWN:  y--;
  }
  if(!inbounds(y, x)){
    delete fb;
    return DOWN;
  }
  fb->pos[dim_x] = x;
  fb->pos[dim_y] = y;
  e->character_map[y][x] = fb;
  
  e->q->enqueue((void*)fb);
  return fb->direction;
}


void handle_fire(environment *e, fire_ball *fb){
  uint32_t x = fb->pos[dim_x];
  uint32_t y = fb->pos[dim_y];
  character *c;
  
  e->character_map[y][x] = NULL; 
  switch(fb->direction){
    case RIGHT: x++; 
    case LEFT:  x--;
    case UP:    y++;
    case DOWN:  y--;
  }
  
  if(inbounds(y, x)) {
    fb->alive = false;
  }
  else if( (c = charxy(x, y)) ){
    //c = charxy(x, y);
    if(c == e->PC){ respawn_pc(e); }
    else{ c->alive = false; }
    fb->alive = false;
  }
  else if(envrxy(x, y) == BOX){
    e->environment_map[y][x] = EMPTY;
    e->object_map[y][x] = random_object();
    if(inbounds(y ,x) && (c = charxy(x, y + 1)) ){
      c->alive = false;
      e->character_map[y + 1][x] = NULL;
    }
    fb->alive = false;
  }
  else{
    fb->pos[dim_x] = x;
    fb->pos[dim_y] = y;
    e->character_map[y][x] = fb;
  }
  e->temp = fb->alive ? e->temp : e->temp - 1;
}


