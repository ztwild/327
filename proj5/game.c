#include "game.h"

dungeon_t *dungeon;
queue_t *turn;
int print_time;
uint8_t load, save, nummon, quit;
int mon_index, mon_toggle;

void get_switches(int argc, char *argv[]){
  int i;
  for(i = 0; i < argc; i++){
    if(is_equal(argv[i], SAVE)){
      save = 1;
    }
    else if(is_equal(argv[i], LOAD)){
      load = 1;
    }
    else if(is_equal(argv[i], NUMMON) && atoi(argv[i+1])){
      nummon = atoi(argv[++i]);
    }
  } 
}

void clear_turn_queue(){
  while(turn->first){
    dequeue(turn);
  }
  free(turn);
}

void draw_new(){
  place_stairs(dungeon, STRS_UP);
  place_stairs(dungeon, STRS_DOWN);
  
  dungeon->nummon = nummon;
  print_time = TIME / nummon;
  
  /** path_find **/
  init_characters(dungeon);
  init_path(dungeon);
  bfs(dungeon);
  dijkstra(dungeon);
  turn = init_turn_queue();
}

void init_game(int argc, char *argv[]){
  nummon = 10;
  quit = 0;
  mon_toggle = 0;
  mon_index = 0;
  get_switches(argc, argv);
  dungeon = load ? load_dungeon() : new_dungeon();
  if(save){
    save_dungeon(dungeon);
  }
  
  draw_new();
}

int run_game(){
  
  print_grid(dungeon);
  while(!end_game()){
    character_t *c = (character_t*)dequeue(turn);
    
    if(c->type == PC){
      move_pc();
      init_path(dungeon);
      bfs(dungeon);
      dijkstra(dungeon);
    }
    else{
      next_pos(c);
      c->turn += 1000 / c->speed;
    }
    enqueue_sort(turn, c, compare_characters);
    print_grid(dungeon);
    if(mon_toggle)
      print_mon_list();
    usleep(print_time);
  }
  
  print_grid(dungeon);
  
    
  if(!dungeon->pc->alive){
    mvprintw(22, 0, "!!!!!Player is Dead!!!!!  press key to exit\n");
    getch();
  }
  else
  clear_turn_queue();
  free_dungeon(dungeon);
  return 0;
}

int move_pc(){
  pair_t *pos = dungeon->pc->pos;
  int c = getch();
  mvprintw(0, 23, "Key was pressed: %d\n", c);
  switch(c){
  
  case '1': case 'b':
    if(!dungeon->hardness[pos->y+1][pos->x-1]){
      pos->y++;
      pos->x--;
      dungeon->pc->turn += 1000 / dungeon->pc->speed;
    }
    break;
  case '2': case 'j':
    if(!dungeon->hardness[pos->y+1][pos->x]){
      pos->y++;
      dungeon->pc->turn += 1000 / dungeon->pc->speed;
    }
    break;
  case '3': case 'n':
    if(!dungeon->hardness[pos->y+1][pos->x+1]){
      pos->y++;
      pos->x++;
      dungeon->pc->turn += 1000 / dungeon->pc->speed;
    }
    break;
  case '4': case 'h':
    if(!dungeon->hardness[pos->y][pos->x-1]){
      pos->x--;
      dungeon->pc->turn += 1000 / dungeon->pc->speed;
    }
    break;  
  case '5': case ' ':
    dungeon->pc->turn += 1000 / dungeon->pc->speed;
    break;  
  case '6': case 'l':
    if(!dungeon->hardness[pos->y][pos->x+1]){
      pos->x++;
      dungeon->pc->turn += 1000 / dungeon->pc->speed;
    }
    break;
  case '7': case 'y':
    if(!dungeon->hardness[pos->y-1][pos->x-1]){
      pos->y--;
      pos->x--;
      dungeon->pc->turn += 1000 / dungeon->pc->speed;
    }
    break;
  case '8': case 'k':
    if(!dungeon->hardness[pos->y-1][pos->x]){
      pos->y--;
      dungeon->pc->turn += 1000 / dungeon->pc->speed;
    }
    break;
  case '9': case 'u':
    
    if(!dungeon->hardness[pos->y-1][pos->x+1]){
      pos->y--;
      pos->x++;
      dungeon->pc->turn += 1000 / dungeon->pc->speed;
    }
    break;
  case '<':
    if(dungeon->grid[pos->y][pos->x] == STRS_UP){
      clear_turn_queue();
      mvprintw(23, 0, "Queue cleared");
      free_dungeon(dungeon);
      mvprintw(23, 0, "Dungeon cleared");
      draw_new();
      mvprintw(23, 0, "Dungeon redrawn");
    }
    break;
  case '>':
    if(dungeon->grid[pos->y][pos->x] == STRS_DOWN){
      clear_turn_queue();
      mvprintw(23, 0, "Queue cleared");
      free_dungeon(dungeon);
      mvprintw(23, 0, "Dungeon cleared");
      draw_new();
      mvprintw(23, 0, "Dungeon redrawn");
    }
    break;
  case 'm':
    mon_toggle = 1;
    mon_index = 0;
    break;
  case KEY_UP:
    mon_index--;
    break;
  case KEY_DOWN:
    mon_index++;
    break;
  case 27:
    mon_toggle = 0;
    break;
  case 'Q':
    quit = 1;
    break;
  //default:
    //mvprintw(0, 23, "Key not recognized\n");
  
  }
  
  int x, y, i;
  for(i = 0; i < dungeon->nummon; i++){
    character_t *m = dungeon->monsters[i];
    x = m->pos->x == pos->x;
    y = m->pos->y == pos->y;
    if(m->alive && x && y){
      m->alive = 0;
    }
  }
  
  return 0;
}

void print_mon_list(){
  int i, len;
  character_t *pc = dungeon->pc;

  mon_index = mon_index >= dungeon->nummon ? 
      dungeon->nummon - 1 : 
      mon_index < 0 ? 0 : mon_index;
  len = min(8 + mon_index, dungeon->nummon);
  
  for(i = mon_index; i < len; i++){
    character_t *mon = dungeon->monsters[i];
    int i_vert = mon->pos->y - pc->pos->y;
    int i_horz = mon->pos->x - pc->pos->x;
    char *c_vert = i_vert > 0 ? "South" : "North";
    char *c_horz = i_horz > 0 ? "East" : "West";
    char type = mon->attr > 9 ? (mon->attr - 10) + 'a' : mon->attr + '0';
    
    mvprintw(8+(i - mon_index), 22, "Monster %c, %2d %s and %2d %s", 
          type, abs(i_vert), c_vert, abs(i_horz), c_horz);
  }
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
  return dead || mons_dead || quit;
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
