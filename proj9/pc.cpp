#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#include "dungeon.h"
#include "pc.h"
#include "utils.h"
#include "move.h"
#include "path.h"
#include "io.h"
#include "object.h"

using namespace std;

bool pickup_object(dungeon *d, pair_t p){
  int i = 0;
  object *obj = objpair(p);
  while(i < CARRY_NUM){
    if(!d->PC->carry[i]){ //if spot is empty
      d->PC->carry[i] = obj;
      d->objmap[p[dim_y]][p[dim_x]] = NULL;
      io_queue_message("Added to slot %d", i);
      return true;
    }
    i++;
  }
  return false;
}


/** PC Carry List **/
void display_carry(dungeon *d){
  uint32_t i;
  for(i = 0; i < CARRY_NUM; i++){
    mvprintw(6+i, 20, "%d  ", i);
    if(d->PC->carry[i]){
      attron(COLOR_PAIR(d->PC->carry[i]->get_color()));
      mvprintw(6+i, 23, "%c  ", d->PC->carry[i]->get_symbol());
      attroff(COLOR_PAIR(d->PC->carry[i]->get_color()));
      mvprintw(6+i, 26, "%s", d->PC->carry[i]->get_name());
    }
    else{
      mvprintw(6+i, 23, "                ");
    }
  }
  mvprintw(6+i, 20, "Press ESC to exit");
}

void wear_item(uint32_t index, dungeon *d){
  uint32_t i;
  if(d->PC->carry[index]){
    i = d->PC->carry[index]->get_type() - 1;
    if(!d->PC->equip[i]){
      d->PC->equip[i] = d->PC->carry[index];
      d->PC->carry[index] = NULL;
    }
    else if(i == 10 && !d->PC->equip[i+1]){
      d->PC->equip[i+1] = d->PC->carry[index];
      d->PC->carry[index] = NULL;
    }
    else{
      object *temp = d->PC->equip[i];
      d->PC->equip[i] = d->PC->carry[index];
      d->PC->carry[index] = temp;
    }
  }
}

void drop_item(uint32_t index, dungeon *d){
  if(d->PC->carry[index]){
    object *obj = d->PC->carry[index];
    d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = obj;
    d->PC->carry[index] = NULL;
  }
}

bool compare_objects(object *o1, object *o2){
  if(!o1 || !o2) return false;
  bool type = o1->get_type() == o2->get_type();
  bool name = strcmp(o1->get_name(), o2->get_name()) == 0;
  bool color = o1->get_color() == o2->get_color();
  bool sym = o1->get_symbol() == o2->get_symbol();
  
  return type && name && color && sym;
}

bool compare_objects(object *o, object_description *des){
  if(!o || !des) return false;
  bool type = o->get_type() == des->get_type();
  bool name = strcmp(o->get_name(), des->get_name().c_str()) == 0;
  bool color = o->get_color() == des->get_color();
  
  return type && name && color;
}

void expunge_item(uint32_t index, dungeon *d){
  uint32_t i, x, y;
  object *obj = d->PC->carry[index];
  d->PC->carry[index] = NULL;
  if(obj){
    //traverse carry
    for(i = 0; i < CARRY_NUM; i++){
      if(compare_objects(obj, d->PC->carry[i])){
        delete d->PC->carry[i];
        d->PC->carry[i] = NULL;
      }
    }
    //traverse equip
    for(i = 0; i < EQUIP_NUM; i++){
      if(compare_objects(obj, d->PC->equip[i])){
        delete d->PC->equip[i];
        d->PC->equip[i] = NULL;
      }
    }
    //traverse map
    for (y = 0; y < DUNGEON_Y; y++) {
      for (x = 0; x < DUNGEON_X; x++) {
        if (compare_objects(obj, d->objmap[y][x])) {
          delete d->objmap[y][x];
          d->objmap[y][x] = 0;
        }
      }
    }
    //traverse vector
    /**
    vector<object_description> &v = d->object_descriptions->be;
    for(i = 0; i < v.size(); i++){
      if(compare_objects(obj, &v[i])){
        mvprintw(30, 30, "removed %s", v[i].get_name());
        v.erase(v.begin() + i);
      }
    }
    **/
    
    delete obj;
  }
}

void description_item(uint32_t index, dungeon *d){
  object *obj = d->PC->carry[index];
  io_display(d);
  mvprintw(5, 25, "%s", obj->get_name());
  if(obj){
    //char buff[50];
    int i = 0;
    const char *desc = obj->get_description();
    mvprintw(6+i, 0, "%s", desc);
  }
  getch();
}

void carry_list(dungeon *d, item_action ia){
  uint32_t fail_code, i;
  char sym;
  
  do{
    display_carry(d);
    sym = getch();
    fail_code = 1;
    if(sym >= '0' && sym <= '9'){
      i = sym - '0';
      switch(ia){
      case LIST:
        mvprintw(5, 25, "CARRY LIST");
        break;
      case DROP:
        mvprintw(5, 25, "DROP AN ITEM");
        drop_item(i, d);
      case WEAR:
        mvprintw(5, 25, "WEAR AN ITEM");
        wear_item(i, d);
      case EXPUNGE:
        mvprintw(5, 25, "EXPUNGE ITEM FROM GAME");
        expunge_item(i, d);
      case DESC:
        mvprintw(5, 25, "DESC OF ITEM");
        description_item(i, d);
      default:
        break;
      }
    }
    else if(sym == 27){
      fail_code = 0;
    }
    
    io_display(d);
  }while(fail_code);
  
  io_display(d);
}

/** PC Equip List **/
void display_equip(dungeon *d){
  uint32_t i;
  string equip_index = "abcdefghijkl";
  for(i = 0; i < EQUIP_NUM; i++){
    mvprintw(6+i, 20, "%c  ", equip_index[i]);
    if(d->PC->equip[i]){
      attron(COLOR_PAIR(d->PC->equip[i]->get_color()));
      mvprintw(6+i, 23, "%c  ", d->PC->equip[i]->get_symbol());
      attroff(COLOR_PAIR(d->PC->equip[i]->get_color()));
      mvprintw(6+i, 26, "%s", d->PC->equip[i]->get_name());
    }
    else{
      mvprintw(6+i, 23, "                ");
    }
  }
  mvprintw(6+i, 20, "Press ESC to exit");
}

void remove_item(uint32_t index, dungeon *d){
  uint32_t i = 0;
  object *obj = d->PC->equip[index];
  d->PC->equip[index] = NULL;
  if(obj){
    while(i < CARRY_NUM){
      if(!d->PC->carry[i]){ //if spot is empty
        d->PC->carry[i] = obj;
        return;
      }
      i++;
    }
    //else it will drop on ground
    d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = obj;
  }
}

void equip_list(dungeon *d, item_action ia){
  uint32_t fail_code, i;
  char sym;
  
  do{
    display_equip(d);
    sym = getch();
    fail_code = 1;
    if(sym >= 'a' && sym <= 'l'){
      i = sym - 'a';
      switch(ia){
      case LIST:
        mvprintw(5, 25, "EQUIPMENT LIST");
        break;
      case REMOVE:
        mvprintw(5, 25, "TAKE OFF AN ITEM");
        remove_item(i, d);
      default:
        break;
      }
    }
    else if(sym == 27){
      fail_code = 0;
    }
    
    io_display(d);
  }while(fail_code);
  
  io_display(d);
}



uint32_t pc_is_alive(dungeon_t *d)
{
  return d->PC && d->PC->alive;
}

void place_pc(dungeon_t *d)
{
  d->PC->position[dim_y] = rand_range(d->rooms->position[dim_y],
                                     (d->rooms->position[dim_y] +
                                      d->rooms->size[dim_y] - 1));
  d->PC->position[dim_x] = rand_range(d->rooms->position[dim_x],
                                     (d->rooms->position[dim_x] +
                                      d->rooms->size[dim_x] - 1));

  pc_init_known_terrain(d->PC);
  pc_observe_terrain(d->PC, d);

  io_display(d);
}

void config_pc(dungeon_t *d)
{
  static dice pc_dice(0, 1, 4);
  uint32_t i;

  d->PC = new pc;

  d->PC->symbol = '@';

  place_pc(d);

  d->PC->speed = PC_SPEED;
  d->PC->alive = 1;
  d->PC->sequence_number = 0;
  d->PC->kills[kill_direct] = d->PC->kills[kill_avenged] = 0;
  d->PC->color.push_back(COLOR_WHITE);
  d->PC->damage = &pc_dice;
  d->PC->name = "Isabella Garcia-Shapiro";
  for(i = 0; i < CARRY_NUM; i++){ d->PC->carry[i] = NULL; }
  for(i = 0; i < EQUIP_NUM; i++){ d->PC->equip[i] = NULL; }

  d->character_map[character_get_y(d->PC)][character_get_x(d->PC)] = d->PC;

  dijkstra(d);
  dijkstra_tunnel(d);
}

uint32_t pc_next_pos(dungeon_t *d, pair_t dir)
{
  static uint32_t have_seen_corner = 0;
  static uint32_t count = 0;

  dir[dim_y] = dir[dim_x] = 0;

  if (in_corner(d, d->PC)) {
    if (!count) {
      count = 1;
    }
    have_seen_corner = 1;
  }

  /* First, eat anybody standing next to us. */
  if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x], d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = 1;
  } else if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y])) {
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y])) {
    dir[dim_x] = 1;
  } else if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x], d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = 1;
  } else if (!have_seen_corner || count < 250) {
    /* Head to a corner and let most of the NPCs kill each other off */
    if (count) {
      count++;
    }
    if (!against_wall(d, d->PC) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir_nearest_wall(d, d->PC, dir);
    }
  }else {
    /* And after we've been there, let's head toward the center of the map. */
    if (!against_wall(d, d->PC) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir[dim_x] = ((d->PC->position[dim_x] > DUNGEON_X / 2) ? -1 : 1);
      dir[dim_y] = ((d->PC->position[dim_y] > DUNGEON_Y / 2) ? -1 : 1);
    }
  }

  /* Don't move to an unoccupied location if that places us next to a monster */
  if (!charxy(d->PC->position[dim_x] + dir[dim_x],
              d->PC->position[dim_y] + dir[dim_y]) &&
      ((charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y]) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y]) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x],
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x],
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x],
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x],
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y]) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y]) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)))) {
    dir[dim_x] = dir[dim_y] = 0;
  }

  return 0;
}

uint32_t pc_in_room(dungeon_t *d, uint32_t room)
{
  if ((room < d->num_rooms)                                     &&
      (d->PC->position[dim_x] >= d->rooms[room].position[dim_x]) &&
      (d->PC->position[dim_x] < (d->rooms[room].position[dim_x] +
                                d->rooms[room].size[dim_x]))    &&
      (d->PC->position[dim_y] >= d->rooms[room].position[dim_y]) &&
      (d->PC->position[dim_y] < (d->rooms[room].position[dim_y] +
                                d->rooms[room].size[dim_y]))) {
    return 1;
  }

  return 0;
}

void pc_learn_terrain(pc *p, pair_t pos, terrain_type_t ter)
{
  p->known_terrain[pos[dim_y]][pos[dim_x]] = ter;
  p->visible[pos[dim_y]][pos[dim_x]] = 1;
}

void pc_reset_visibility(pc *p)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      p->visible[y][x] = 0;
    }
  }
}

terrain_type_t pc_learned_terrain(pc *p, int16_t y, int16_t x)
{
  if (y < 0 || y >= DUNGEON_Y || x < 0 || x >= DUNGEON_X) {
    io_queue_message("Invalid value to %s: %d, %d", __FUNCTION__, y, x);
  }

  return p->known_terrain[y][x];
}

void pc_init_known_terrain(pc *p)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      p->known_terrain[y][x] = ter_unknown;
      p->visible[y][x] = 0;
    }
  }
}

void pc_observe_terrain(pc *p, dungeon_t *d)
{
  pair_t where;
  int16_t y_min, y_max, x_min, x_max;

  y_min = p->position[dim_y] - PC_VISUAL_RANGE;
  if (y_min < 0) {
    y_min = 0;
  }
  y_max = p->position[dim_y] + PC_VISUAL_RANGE;
  if (y_max > DUNGEON_Y - 1) {
    y_max = DUNGEON_Y - 1;
  }
  x_min = p->position[dim_x] - PC_VISUAL_RANGE;
  if (x_min < 0) {
    x_min = 0;
  }
  x_max = p->position[dim_x] + PC_VISUAL_RANGE;
  if (x_max > DUNGEON_X - 1) {
    x_max = DUNGEON_X - 1;
  }

  for (where[dim_y] = y_min; where[dim_y] <= y_max; where[dim_y]++) {
    where[dim_x] = x_min;
    can_see(d, p->position, where, 1, 1);
    where[dim_x] = x_max;
    can_see(d, p->position, where, 1, 1);
  }
  /* Take one off the x range because we alreay hit the corners above. */
  for (where[dim_x] = x_min - 1; where[dim_x] <= x_max - 1; where[dim_x]++) {
    where[dim_y] = y_min;
    can_see(d, p->position, where, 1, 1);
    where[dim_y] = y_max;
    can_see(d, p->position, where, 1, 1);
  }       
}

int32_t is_illuminated(pc *p, int16_t y, int16_t x)
{
  return p->visible[y][x];
}

void pc_see_object(character *the_pc, object *o)
{
  if (o) {
    o->has_been_seen();
  }
}
