#include "game.h"

dungeon_t *dungeon;
int print_time;

void init_game(uint8_t load, uint8_t nummon){
  dungeon = load ? load_dungeon() : new_dungeon();
  dungeon->nummon = nummon;
  print_time = TIME / nummon;
  
  /** path_find **/
  init_characters(dungeon);
  init_path(dungeon);
  bfs(dungeon);
  dijkstra(dungeon);
}

int run_game(uint8_t save){
  queue_t *turn = init_turn_queue();
  
  while(!end_game()){
    character_t *c = (character_t*)dequeue(turn);
    c->turn += 1000 / c->speed;
    if(c->type == PC){
      move_pc();
      init_path(dungeon);
      bfs(dungeon);
      dijkstra(dungeon);
    }
    else{
      next_pos(c);
    }
    enqueue_sort(turn, c, compare_characters);
    print_grid(dungeon);
    usleep(print_time);
  }
  
  print_grid(dungeon);
    
  if(!dungeon->pc->alive){
    printf("!!!!!Player is Dead!!!!!\n");
  }
   
  if(save){
    save_dungeon(dungeon);
  }
  return 0;
}

int move_pc(){
  int x, y, i;
  uint8_t hard;
  character_t *pc = dungeon->pc; 
  
  for(i = 0; i < dungeon->nummon; i++){
    character_t *m = dungeon->monsters[i];
    x = abs(m->pos->x - pc->pos->x);
    y = abs(m->pos->y - pc->pos->y);
    if(m->alive && x < 2 && y < 2){
      m->alive = 0;
      pc->pos->x = m->pos->x;
      pc->pos->y = m->pos->y;
      return 0;
    }
  }
  
  x = ((rand() % 3) - 1) + pc->pos->x;
  y = ((rand() % 3) - 1) + pc->pos->y;
  hard =  dungeon->open[y][x];
  while(hard == 255){
    x = ((rand() % 3) - 1) + pc->pos->x;
    y = ((rand() % 3) - 1) + pc->pos->y;
    hard = dungeon->open[y][x];
  }
  
  pc->pos->x = x;
  pc->pos->y = y;
  return 0;
}

int get_dir(int des, int init){
  int dir;
  if(des == init){
    dir = 0;
  }else{
    dir = des > init ? 1 : -1;
  }
  return init + dir;
}

int next_pos(character_t *c){
  int i, j, move;
  uint8_t x, y, hard;
  pair_t *pc_pos = dungeon->pc->pos;
  
  if(!c->alive){
    return 0;
  }
  
  if(c->attr & 0x08 && rand() % 2){ //Erratic
    x = ((rand() % 3) - 1) + c->pos->x;
    y = ((rand() % 3) - 1) + c->pos->y;
    hard = c->attr & 0x04 ? dungeon->wall[y][x] : dungeon->open[y][x];
    while(hard == 255){
      x = ((rand() % 3) - 1) + c->pos->x;
      y = ((rand() % 3) - 1) + c->pos->y;
      hard = c->attr & 0x04 ? dungeon->wall[y][x] : dungeon->open[y][x];
    }
    move = 1;
  }
  else if(c->attr & 0x01){  //Intelligent
    uint8_t cheapest = 255;
    for(i = -1; i < 2; i++){
      for(j = -1; j < 2; j++){
        uint8_t temp_x = c->pos->x + i;
        uint8_t temp_y = c->pos->y + j;
        uint8_t path = c->attr & 0x04 ? 
            dungeon->wall[temp_y][temp_x] : dungeon->open[temp_y][temp_x];
        if(path < cheapest){
          x = temp_x;
          y = temp_y;
          cheapest = path;
        }
      }
    }
    move = 1;
  }
  else{ // Unintelligent
    x = get_dir(pc_pos->x, c->pos->x);
    y = get_dir(pc_pos->y, c->pos->y);
    hard = c->attr & 0x04 ? dungeon->wall[y][x] : dungeon->open[y][x];
    move = hard < 255;
  }
  
  if(dungeon->pc->pos->x == x && dungeon->pc->pos->y == y){
    dungeon->pc->alive = 0;
  }
  else if(move && dungeon->grid[y][x] == WALL){
    if(dungeon->hardness[y][x] <= 85){
      dungeon->hardness[y][x] = 0;
      dungeon->grid[y][x] = HALL;
      
      init_path(dungeon);
      bfs(dungeon);
      dijkstra(dungeon);
    }
    else{
      dungeon->hardness[y][x] -= 85;
      move = 0;
    }
    
  }
  if(move){
    c->pos->x = x;
    c->pos->y = y;
  }
  return 0;
}

int cell_taken(int x, int y){
  int i;
  for(i = 0; i < dungeon->nummon; i++){
    character_t *m = dungeon->monsters[i];
    if(m->pos->x == x && m->pos->y == y){
      return 1;
    }
  }
  return 0;
}


queue_t *init_turn_queue(){
  int i;
  queue_t *q = init_queue();
  enqueue(q, dungeon->pc);
  for(i = 0; i < dungeon->nummon; i++){
    enqueue(q, dungeon->monsters[i]);
  }
  return q;
}

int end_game(){
  int dead = !dungeon->pc->alive;
  int mons_dead = 1;
  int i;
  for(i = 0; i < dungeon->nummon; i++){
    mons_dead &= !dungeon->monsters[i]->alive;
  }
  return dead || mons_dead;
}

void print_queue(queue_t *q){
  node_t *temp = q->first;
  while(temp){
    character_t *c = (character_t*)temp->value;
    printf("Time: %d, Speed: %d, Turn: %d\n", c->time, c->speed, c->turn);
    temp = temp->next;
  }
  printf("\n");
}




void print_test(){
  print_grid(dungeon);
  print_path_open(dungeon);
  print_path_wall(dungeon);
}
