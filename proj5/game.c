#include "game.h"

dungeon_t *dungeon;
queue_t *turn;
int print_time;
uint8_t load, save, nummon, quit;

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
    }
    c->turn += 1000 / c->speed;
    enqueue_sort(turn, c, compare_characters);
    print_grid(dungeon);
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
  switch(c){
  case KEY_UP:
    mvprintw(0, 23, "Key was pressed: UP\n", c);
    break;
  case KEY_DOWN:
    mvprintw(0, 23, "Key was pressed: DOWN\n", c);
    break;
  case '1': case 'b':
    mvprintw(0, 23, "Key was pressed: 1\n", c);
    if(!dungeon->hardness[pos->y+1][pos->x-1])
      pos->y++;
      pos->x--;
    break;
  case '2': case 'j':
    mvprintw(0, 23, "Key was pressed: 2\n", c);
    if(!dungeon->hardness[pos->y+1][pos->x])
      pos->y++;
    break;
  case '3': case 'n':
    mvprintw(0, 23, "Key was pressed: 3\n", c);
    if(!dungeon->hardness[pos->y+1][pos->x+1])
      pos->y++;
      pos->x++;
    break;
  case '4': case 'h':
    mvprintw(0, 23, "Key was pressed: 4\n", c);
    if(!dungeon->hardness[pos->y][pos->x-1])
      pos->x--;
    break;  
  case '5': case ' ':
    mvprintw(0, 23, "Key was pressed: 5\n", c);
    break;  
  case '6': case 'l':
    mvprintw(0, 23, "Key was pressed: 6\n", c);
    if(!dungeon->hardness[pos->y][pos->x+1])
      pos->x++;
    break;
  case '7': case 'y':
    mvprintw(0, 23, "Key was pressed: 7\n", c);
    if(!dungeon->hardness[pos->y-1][pos->x-1])
      pos->y--;
      pos->x--;
    break;
  case '8': case 'k':
    mvprintw(0, 23, "Key was pressed: 8\n", c);
    if(!dungeon->hardness[pos->y-1][pos->x])
      pos->y--;
    break;
  case '9': case 'u':
    mvprintw(0, 23, "Key was pressed: 9\n", c);
    if(!dungeon->hardness[pos->y-1][pos->x+1])
      pos->y--;
      pos->x++;
    break;
  case '<':
    mvprintw(0, 23, "Key was pressed: <\n", c);
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
    mvprintw(0, 23, "Key was pressed: >\n", c);
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
    mvprintw(0, 23, "Key was pressed: m\n", c);
    break;
  case 'Q':
    quit = 1;
    break;
  default:
    mvprintw(0, 23, "Key not recognized\n");
  
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
