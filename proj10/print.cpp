
#define COLOR_COUNT   7

void print_message(const char *mess){
  clear();
  int y = (VIEW_Y + 2) / 2;
  int x = VIEW_X / 2 - strlen(mess) / 2;
  mvprintw(y, x, "%s", mess);
}

void display_header(environment *e){
  attron(COLOR_PAIR(COLOR_WHITE));
  mvprintw(0, 2, "LEVEL  %d", e->level);
  mvprintw(1, 2, "LIFE x %d", e->PC->life);
  mvprintw(2, 2, "COIN x %d", e->PC->coin);
  mvprintw(2, VIEW_X / 3, "PC Position (%3d, %3d)", e->PC->pos[dim_x], e->PC->pos[dim_y]);
}

int random_color(){
  return (rand() % COLOR_COUNT) + 1;
}

int object_color(obj_type obj){
  switch(obj){
    case COIN:  
      return COLOR_YELLOW;
    case FIRE:
      return COLOR_RED;
    case INVINCIBLE:
      return random_color();
    default:
      return COLOR_WHITE;
  }
}

char object_char(obj_type obj){
  switch(obj){
    case COIN:
      return '$';
    case LIFE:
      return '^';
    case FIRE: case INVINCIBLE:
      return '*';
    case CHECKPOINT:
      return 'v';
    default:
      return 'X';
  }
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
  uint32_t x, y;
  uint32_t color;
  char symbol;
  
  clear();
  display_header(e);
  pair_t view;
  get_view(e, view);
  string end = "END OF LEVEL ";
  
  for(x = 0; x < VIEW_X; x++){
    for(y = 0; y < VIEW_Y; y++){
      obj_type obj = objxy(view[dim_x] + x, view[dim_y] + y);
      
      if(charxy(view[dim_x] + x, view[dim_y] + y)){
        character *c = charxy(view[dim_x] + x, view[dim_y] + y);
        switch(c->type){
          case PC:
            color = c->color;
            color = e->PC->fire > 0 ? COLOR_RED : color;
            color = e->PC->invincible > 0 ? random_color() : color;
            attron(COLOR_PAIR(color));
            mvaddch(VIEW_Y + 2 - y, x, '@');
            break;
          case NPC:
            color = c->color;
            attron(COLOR_PAIR(color));
            symbol = ((npc*)c)->right ? '>' : '<';
            mvaddch(VIEW_Y + 2 - y, x, symbol);
            break;
          case FIRE_BALL:
            color = c->color;
            attron(COLOR_PAIR(color));
            mvaddch(VIEW_Y + 2 - y, x, '*');
            break;
        }
        
      }
      else if(obj){
        color = object_color(obj);
        attron(COLOR_PAIR(color));
        mvaddch(VIEW_Y + 2 - y, x, object_char(obj));
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
          case END:
            int index = end.size() - ((view[dim_y] + y + 5) % end.size()) - 1;
            attron(COLOR_PAIR(random_color()));
            mvaddch(VIEW_Y + 2 - y, x, end[index]);
            break;
        }
      }
      
    }
  }
}

void io_init_terminal(){
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}
